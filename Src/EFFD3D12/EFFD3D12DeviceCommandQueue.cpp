#include "EFFD3D12PCH.h"

#include "EFFD3D12DeviceCommandQueue.h"
#include "EFFD3D12DeviceCommandList.h"
#include "EFFD3D12Device.h"
#include "EFFD3D12ResourceStateManager.h"

EFFD3D12DeviceCommandQueue::EFFD3D12DeviceCommandQueue(D3D12_COMMAND_LIST_TYPE type)
	: currentFenceValue(0)
	, commandListType(type)
	, processInFlightCommandLists(true)
{
	EFFD3D12Device * device = (EFFD3D12Device *)EFF3DGetDevice();
	ID3D12Device * d3d12Device = device->GetD3D12Device();

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = type;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	DX_CHECK(d3d12Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12CommandQueue)));
	DX_CHECK(d3d12Device->CreateFence(currentFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&d3d12Fence)));

	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_COPY:
		d3d12CommandQueue->SetName(L"Copy Command Queue");
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		d3d12CommandQueue->SetName(L"Compute Command Queue");
		break;
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		d3d12CommandQueue->SetName(L"Direct Command Queue");
		break;
	}

	//processInFlightCommandListsThread = std::thread(&CommandQueue::ProccessInFlightCommandLists, this);
}

EFFD3D12DeviceCommandQueue::~EFFD3D12DeviceCommandQueue()
{
	processInFlightCommandLists = effFALSE;
	//processInFlightCommandListsThread.join();
}

effUINT64 EFFD3D12DeviceCommandQueue::Signal()
{
	effUINT64 result = currentFenceValue;
	d3d12CommandQueue->Signal(d3d12Fence.Get(), currentFenceValue);
	currentFenceValue++;

	return result;
}

effBOOL EFFD3D12DeviceCommandQueue::IsFenceComplete(effUINT64 fenceValue)
{
	return d3d12Fence->GetCompletedValue() >= fenceValue;
}

effVOID EFFD3D12DeviceCommandQueue::WaitForFenceValue(effUINT64 fenceValue)
{
	if (!IsFenceComplete(fenceValue))
	{
		auto event = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(event && "Failed to create fence event handle.");

		// Is this function thread safe?
		d3d12Fence->SetEventOnCompletion(fenceValue, event);
		::WaitForSingleObject(event, DWORD_MAX);

		::CloseHandle(event);
	}
}

effVOID EFFD3D12DeviceCommandQueue::Flush()
{
	processInFlightCommandListsThreadMutex.Lock();
	//processInFlightCommandListsThreadCV.wait(lock, [this] { return inFlightCommandLists.Empty(); });

	// In case the command queue was signaled directly 
	// using the CommandQueue::Signal method then the 
	// fence value of the command queue might be higher than the fence
	// value of any of the executed command lists.
	WaitForFenceValue(currentFenceValue);
}

EFFD3D12DeviceCommandList * EFFD3D12DeviceCommandQueue::GetCommandList()
{
	EFFD3D12DeviceCommandList * commandList;

	// If there is a command list on the queue.
	if (!availableCommandLists.empty())
	{
		commandList = availableCommandLists.front();
		availableCommandLists.pop();
	}
	else
	{
		// Otherwise create a new command list.
		commandList = EFFNEW EFFD3D12DeviceCommandList(commandListType);
	}

	return commandList;
}

// Execute a command list.
// Returns the fence value to wait for for this command list.
effUINT64 EFFD3D12DeviceCommandQueue::ExecuteCommandList(EFFD3D12DeviceCommandList * commandList)
{
	return ExecuteCommandLists(VECTOR<EFFD3D12DeviceCommandList *>({ commandList }));
}

effUINT64 EFFD3D12DeviceCommandQueue::ExecuteCommandLists(const VECTOR<EFFD3D12DeviceCommandList *> & commandLists)
{
	EFFD3D12Device * device = (EFFD3D12Device *)EFF3DGetDevice();
	EFFD3D12ResourceStateManager * resourceStateManager = device->GetResourceStateManager();


	resourceStateManager->Lock();

	// Command lists that need to put back on the command list queue.
	VECTOR<EFFD3D12DeviceCommandList *> toBeQueued;
	toBeQueued.reserve(commandLists.size() * 2);        // 2x since each command list will have a pending command list.

	// Generate mips command lists.
	VECTOR<EFFD3D12DeviceCommandList *> generateMipsCommandLists;
	generateMipsCommandLists.reserve(commandLists.size());

	// Command lists that need to be executed.
	VECTOR<ID3D12CommandList*> d3d12CommandLists;
	d3d12CommandLists.reserve(commandLists.size() * 2); // 2x since each command list will have a pending command list.

	for (auto commandList : commandLists)
	{
		auto pendingCommandList = GetCommandList();
		effBOOL hasPendingBarriers = commandList->Close(pendingCommandList);
		pendingCommandList->Close();

		// If there are no pending barriers on the pending command list, there is no reason to 
		// execute an empty command list on the command queue.
		if (hasPendingBarriers)
		{
			d3d12CommandLists.push_back(pendingCommandList->GetD3D12CommandList());
		}
		d3d12CommandLists.push_back(commandList->GetD3D12CommandList());

		toBeQueued.push_back(pendingCommandList);
		toBeQueued.push_back(commandList);

		auto generateMipsCommandList = commandList->GetGenerateMipsCommandList();
		if (generateMipsCommandList)
		{
			generateMipsCommandLists.push_back(generateMipsCommandList);
		}
	}

	effUINT32 numCommandLists = static_cast<effUINT32>(d3d12CommandLists.size());
	d3d12CommandQueue->ExecuteCommandLists(numCommandLists, d3d12CommandLists.data());
	effUINT64 fenceValue = Signal();

	resourceStateManager->Unlock();

	// Queue command lists for reuse.
	for (auto commandList : toBeQueued)
	{
		inFlightCommandLists.push({ fenceValue, commandList });
	}

	// If there are any command lists that generate mips then execute those
	// after the initial resource command lists have finished.
	if (generateMipsCommandLists.size() > 0)
	{
		EFFD3D12Device * device = (EFFD3D12Device *)EFF3DGetDevice();
		auto computeQueue = device->GetCommandQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE);
		computeQueue->Wait(*this);
		computeQueue->ExecuteCommandLists(generateMipsCommandLists);
	}

	return fenceValue;
}

effVOID EFFD3D12DeviceCommandQueue::Wait(const EFFD3D12DeviceCommandQueue & other)
{
	d3d12CommandQueue->Wait(other.d3d12Fence.Get(), other.currentFenceValue);
}

ComPtr<ID3D12CommandQueue> EFFD3D12DeviceCommandQueue::GetD3D12CommandQueue() const
{
	return d3d12CommandQueue;
}

effVOID EFFD3D12DeviceCommandQueue::Update()
{
	if (processInFlightCommandLists)
	{
		while (inFlightCommandLists.size() > 0)
		{
			CommandListEntry commandListEntry = inFlightCommandLists.front();
			auto fenceValue = std::get<0>(commandListEntry);
			auto commandList = std::get<1>(commandListEntry);

			if (currentFenceValue >= fenceValue)
			{
				commandList->Reset();
				availableCommandLists.push(commandList);
			}
			else
			{
				break;
			}
		}
	}
}