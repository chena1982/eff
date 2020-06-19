#pragma once

class EFFD3D12DeviceCommandList;

class EFFD3D12DeviceCommandQueue
{
public:
	EFFD3D12DeviceCommandQueue(D3D12_COMMAND_LIST_TYPE type, ComPtr<ID3D12Device2> d3d12Device);
	virtual ~EFFD3D12DeviceCommandQueue();

	// Get an available command list from the command queue.
	EFFD3D12DeviceCommandList * GetCommandList();

	// Execute a command list.
	// Returns the fence value to wait for for this command list.
	effUINT64 ExecuteCommandList(EFFD3D12DeviceCommandList * commandList);
	effUINT64 ExecuteCommandLists(const VECTOR<EFFD3D12DeviceCommandList *> & commandLists);

	effUINT64 Signal();
	effBOOL IsFenceComplete(effUINT64 fenceValue);
	effVOID WaitForFenceValue(effUINT64 fenceValue);
	effVOID Flush();

	// Wait for another command queue to finish.
	effVOID Wait(const EFFD3D12DeviceCommandQueue & other);

	ComPtr<ID3D12CommandQueue> GetD3D12CommandQueue() const;

	// Free any command lists that are finished processing on the command queue.
	effVOID Update();
private:


	// Keep track of command allocators that are "in-flight"
	// The first member is the fence value to wait for, the second is the 
	// a shared pointer to the "in-flight" command list.
	using CommandListEntry = std::tuple<effUINT64, EFFD3D12DeviceCommandList *>;

	D3D12_COMMAND_LIST_TYPE                         commandListType;
	ComPtr<ID3D12CommandQueue>						d3d12CommandQueue;
	ComPtr<ID3D12Fence>								d3d12Fence;
	effUINT64										currentFenceValue;

	QUEUE<CommandListEntry>							inFlightCommandLists;
	QUEUE<EFFD3D12DeviceCommandList *>				availableCommandLists;

	// A thread to process in-flight command lists.
	EFFThread	processInFlightCommandListsThread;
	effBOOL		processInFlightCommandLists;
	EFFMutex	processInFlightCommandListsThreadMutex;
	effBOOL		processInFlightCommandListsThreadCV;
};