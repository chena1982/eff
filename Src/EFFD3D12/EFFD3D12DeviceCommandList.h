#pragma once



class EFFD3D12VertexBuffer;
class EFFD3D12UploadBuffer;
class EFFD3D12ResourceStateManager;
class EFFD3D12DynamicDescriptorHeap;


class EFFD3D12DeviceCommandList
{
public:
	EFFD3D12DeviceCommandList(D3D12_COMMAND_LIST_TYPE type);
	~EFFD3D12DeviceCommandList();






public:
	effVOID TrackResource(ComPtr<ID3D12Object> object);

	effVOID BindDescriptorHeaps();

	/**
	 * Transition a resource to a particular state.
	 *
	 * @param resource The resource to transition.
	 * @param stateAfter The state to transition the resource to. The before state is resolved by the resource state tracker.
	 * @param subresource The subresource to transition. By default, this is D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES which indicates that all subresources are transitioned to the same state.
	 * @param flushBarriers Force flush any barriers. Resource barriers need to be flushed before a command (draw, dispatch, or copy) that expects the resource to be in a particular state can run.
	 */
	template<typename ResourceType>
	effVOID TransitionBarrier(const ResourceType * resource, D3D12_RESOURCE_STATES stateAfter, effUINT32 subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, effBOOL flushBarriers = effFALSE);
	effVOID TransitionBarrier(ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES stateAfter, effUINT32 subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, effBOOL flushBarriers = effFALSE);




	/***************************************************************************
	 * Methods defined below are only intended to be used by internal classes. *
	 ***************************************************************************/

	 /**
	  * Close the command list.
	  * Used by the command queue.
	  *
	  * @param pendingCommandList The command list that is used to execute pending
	  * resource barriers (if any) for this command list.
	  *
	  * @return true if there are any pending resource barriers that need to be
	  * processed.
	  */
	effBOOL Close(EFFD3D12DeviceCommandList * pendingCommandList);
	// Just close the command list. This is useful for pending command lists.
	effVOID Close();

	/**
	 * Reset the command list. This should only be called by the CommandQueue
	 * before the command list is returned from CommandQueue::GetCommandList.
	 */
	void Reset();


	/**
	 * Release tracked objects. Useful if the swap chain needs to be resized.
	 */
	void ReleaseTrackedObjects();

	/**
	 * Set the currently bound descriptor heap.
	 * Should only be called by the DynamicDescriptorHeap class.
	 */
	effVOID SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, ID3D12DescriptorHeap* heap);

	EFFD3D12DeviceCommandList * GetGenerateMipsCommandList() const
	{
		return computeCommandList;
	}

	ID3D12GraphicsCommandList2 *			GetD3D12CommandList() { return d3d12CommandList.Get(); }
protected:


	D3D12_COMMAND_LIST_TYPE					d3d12CommandListType;
	ComPtr<ID3D12GraphicsCommandList2>		d3d12CommandList;
	ComPtr<ID3D12CommandAllocator>			d3d12CommandAllocator;

	// For copy queues, it may be necessary to generate mips while loading textures.
	// Mips can't be generated on copy queues but must be generated on compute or
	// direct queues. In this case, a Compute command list is generated and executed 
	// after the copy queue is finished uploading the first sub resource.
	EFFD3D12DeviceCommandList * computeCommandList;

	// Keep track of the currently bound root signatures to minimize root
	// signature changes.
	ID3D12RootSignature * rootSignature;

	// Resource created in an upload heap. Useful for drawing of dynamic geometry
	// or for uploading constant buffer data that changes every draw call.
	EFFD3D12UploadBuffer * uploadBuffer;

	// Resource state tracker is used by the command list to track (per command list)
	// the current state of a resource. The resource state tracker also tracks the 
	// global state of a resource in order to minimize resource state transitions.
	EFFD3D12ResourceStateManager * resourceStateManager;

	// The dynamic descriptor heap allows for descriptors to be staged before
	// being committed to the command list. Dynamic descriptors need to be
	// committed before a Draw or Dispatch.
	EFFD3D12DynamicDescriptorHeap * dynamicDescriptorHeap[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

	// Keep track of the currently bound descriptor heaps. Only change descriptor 
	// heaps if they are different than the currently bound descriptor heaps.
	ID3D12DescriptorHeap* descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

	using TrackedObjects = VECTOR<ComPtr<ID3D12Object>>;

	// Objects that are being tracked by a command list that is "in-flight" on 
	// the command-queue and cannot be deleted. To ensure objects are not deleted 
	// until the command list is finished executing, a reference to the object
	// is stored. The referenced objects are released when the command list is 
	// reset.
	TrackedObjects trackedObjects;
};