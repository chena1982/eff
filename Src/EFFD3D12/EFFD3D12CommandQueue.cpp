
#include "EFFD3D12PCH.h"
#include "EFFD3D12CommandQueue.h"
#include "EFFD3D12Device.h"



EFFD3D12CommandQueue::EFFD3D12CommandQueue(D3D12_COMMAND_LIST_TYPE type)
	: fenceValue(0)
	, commandListType(type)
{
	EFFD3D12Device * device = (EFFD3D12Device *)EFF3DGetDevice();

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = type;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	ThrowIfFailed(device->GetD3D12Device()->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12CommandQueue)));
	ThrowIfFailed(device->GetD3D12Device()->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&d3d12Fence)));

	fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent && "Failed to create fence event handle.");
}

EFFD3D12CommandQueue::~EFFD3D12CommandQueue()
{
}

effUINT64 EFFD3D12CommandQueue::Signal()
{
	effUINT64 fenceValueTemp = fenceValue;

	d3d12CommandQueue->Signal(d3d12Fence.Get(), fenceValueTemp);
	fenceValue++;

	return fenceValueTemp;
}

effBOOL EFFD3D12CommandQueue::IsFenceComplete(effUINT64 fenceValue)
{
	return d3d12Fence->GetCompletedValue() >= fenceValue;
}

effVOID EFFD3D12CommandQueue::WaitForFenceValue(effUINT64 fenceValue)
{
	if (!IsFenceComplete(fenceValue))
	{
		d3d12Fence->SetEventOnCompletion(fenceValue, fenceEvent);
		::WaitForSingleObject(fenceEvent, DWORD_MAX);
	}
}

effVOID EFFD3D12CommandQueue::Flush()
{
	WaitForFenceValue(Signal());
}

ComPtr<ID3D12CommandAllocator> EFFD3D12CommandQueue::CreateCommandAllocator()
{
	EFFD3D12Device * device = (EFFD3D12Device *)EFF3DGetDevice();

	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ThrowIfFailed(device->GetD3D12Device()->CreateCommandAllocator(commandListType, IID_PPV_ARGS(&commandAllocator)));

	return commandAllocator;
}

ComPtr<ID3D12GraphicsCommandList2> EFFD3D12CommandQueue::CreateCommandList(ComPtr<ID3D12CommandAllocator> allocator)
{
	EFFD3D12Device * device = (EFFD3D12Device *)EFF3DGetDevice();

	ComPtr<ID3D12GraphicsCommandList2> commandList;
	ThrowIfFailed(device->GetD3D12Device()->CreateCommandList(0, commandListType, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

	return commandList;
}

ComPtr<ID3D12GraphicsCommandList2> EFFD3D12CommandQueue::GetCommandList()
{
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList2> commandList;

	if (!commandAllocatorQueue.empty() && IsFenceComplete(commandAllocatorQueue.front().fenceValue))
	{
		commandAllocator = commandAllocatorQueue.front().commandAllocator;
		commandAllocatorQueue.pop();

		ThrowIfFailed(commandAllocator->Reset());
	}
	else
	{
		commandAllocator = CreateCommandAllocator();
	}

	if (!commandListQueue.empty())
	{
		commandList = commandListQueue.front();
		commandListQueue.pop();

		ThrowIfFailed(commandList->Reset(commandAllocator.Get(), nullptr));
	}
	else
	{
		commandList = CreateCommandList(commandAllocator);
	}

	// Associate the command allocator with the command list so that it can be
	// retrieved when the command list is executed.
	ThrowIfFailed(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()));

	return commandList;
}

// Execute a command list.
// Returns the fence value to wait for for this command list.
effUINT64 EFFD3D12CommandQueue::ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList2> commandList)
{
	commandList->Close();

	ID3D12CommandAllocator* commandAllocator;
	effUINT32 dataSize = sizeof(commandAllocator);
	ThrowIfFailed(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));

	ID3D12CommandList* const ppCommandLists[] = {
		commandList.Get()
	};

	d3d12CommandQueue->ExecuteCommandLists(1, ppCommandLists);
	effUINT64 fenceValue = Signal();

	commandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator });
	commandListQueue.push(commandList);

	// The ownership of the command allocator has been transferred to the ComPtr
	// in the command allocator queue. It is safe to release the reference 
	// in this temporary COM pointer here.
	commandAllocator->Release();

	return fenceValue;
}

ComPtr<ID3D12CommandQueue> EFFD3D12CommandQueue::GetD3D12CommandQueue() const
{
	return d3d12CommandQueue;
}
