#pragma once

#include <queue>



class EFFD3D12CommandQueue
{
public:
	EFFD3D12CommandQueue(D3D12_COMMAND_LIST_TYPE type);
	virtual ~EFFD3D12CommandQueue();

	// Get an available command list from the command queue.
	ComPtr<ID3D12GraphicsCommandList2> GetCommandList();

	// Execute a command list.
	// Returns the fence value to wait for for this command list.
	effUINT64 ExecuteCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList);

	effUINT64 Signal();
	effBOOL IsFenceComplete(effUINT64 fenceValue);
	effVOID WaitForFenceValue(effUINT64 fenceValue);
	effVOID Flush();

	ComPtr<ID3D12CommandQueue> GetD3D12CommandQueue() const;
protected:

	ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
	ComPtr<ID3D12GraphicsCommandList2> CreateCommandList(ComPtr<ID3D12CommandAllocator> allocator);

private:
	// Keep track of command allocators that are "in-flight"
	struct CommandAllocatorEntry
	{
		effUINT64 fenceValue;
		ComPtr<ID3D12CommandAllocator> commandAllocator;
	};

	using CommandAllocatorQueue = std::queue<CommandAllocatorEntry>;
	using CommandListQueue = std::queue<ComPtr<ID3D12GraphicsCommandList2> >;

	D3D12_COMMAND_LIST_TYPE			commandListType;
	ComPtr<ID3D12CommandQueue>		d3d12CommandQueue;
	ComPtr<ID3D12Fence>				d3d12Fence;
	effHANDLE						fenceEvent;
	effUINT64						fenceValue;

	CommandAllocatorQueue			commandAllocatorQueue;
	CommandListQueue				commandListQueue;
};
