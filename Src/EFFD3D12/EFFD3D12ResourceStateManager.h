#pragma once


class EFFD3D12DeviceCommandList;

class EFFD3D12ResourceStateManager
{
public:
	EFFD3D12ResourceStateManager();
	virtual ~EFFD3D12ResourceStateManager();

	/**
	 * Push a resource barrier to the resource state tracker.
	 *
	 * @param barrier The resource barrier to push to the resource state tracker.
	 */
	effVOID ResourceBarrier(const D3D12_RESOURCE_BARRIER & barrier);

	/**
	 * Push a transition resource barrier to the resource state tracker.
	 *
	 * @param resource The resource to transition.
	 * @param stateAfter The state to transition the resource to.
	 * @param subResource The subresource to transition. By default, this is D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES
	 * which indicates that all subresources should be transitioned to the same state.
	 */
	effVOID TransitionResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateAfter, effUINT32 subResource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
	effVOID TransitionResource(const EFF3DResource * resource, D3D12_RESOURCE_STATES stateAfter, effUINT32 subResource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

	/**
	 * Push a UAV resource barrier for the given resource.
	 *
	 * @param resource The resource to add a UAV barrier for. Can be NULL which
	 * indicates that any UAV access could require the barrier.
	 */
	effVOID UAVBarrier(const EFF3DResource * resource = nullptr);

	/**
	 * Push an aliasing barrier for the given resource.
	 *
	 * @param beforeResource The resource currently occupying the space in the heap.
	 * @param afterResource The resource that will be occupying the space in the heap.
	 *
	 * Either the beforeResource or the afterResource parameters can be NULL which
	 * indicates that any placed or reserved resource could cause aliasing.
	 */
	effVOID AliasBarrier(const EFF3DResource * resourceBefore = nullptr, const EFF3DResource * resourceAfter = nullptr);

	/**
	 * Flush any pending resource barriers to the command list.
	 *
	 * @return The number of resource barriers that were flushed to the command list.
	 */
	effUINT32 FlushPendingResourceBarriers(EFFD3D12DeviceCommandList * commandList);

	/**
	 * Flush any (non-pending) resource barriers that have been pushed to the resource state
	 * tracker.
	 */
	effVOID FlushResourceBarriers(EFFD3D12DeviceCommandList * commandList);

	/**
	 * Commit final resource states to the global resource state map.
	 * This must be called when the command list is closed.
	 */
	effVOID CommitFinalResourceStates();

	/**
	 * Reset state tracking. This must be done when the command list is reset.
	 */
	effVOID Reset();


	ID3D12Resource * GetD3D12Resource(const EFF3DResource * resource);

	/**
	 * The global state must be locked before flushing pending resource barriers
	 * and committing the final resource state to the global resource state.
	 * This ensures consistency of the global resource state between command list
	 * executions.
	 */
	static effVOID Lock();

	/**
	 * Unlocks the global resource state after the final states have been committed
	 * to the global resource state array.
	 */
	static effVOID Unlock();

	/**
	 * Add a resource with a given state to the global resource state array (map).
	 * This should be done when the resource is created for the first time.
	 */
	static effVOID AddGlobalResourceState(ID3D12Resource* resource, D3D12_RESOURCE_STATES state);

	/**
	 * Remove a resource from the global resource state array (map).
	 * This should only be done when the resource is destroyed.
	 */
	static effVOID RemoveGlobalResourceState(ID3D12Resource* resource);

protected:

private:
	// An array (vector) of resource barriers.
	using ResourceBarriers = VECTOR<D3D12_RESOURCE_BARRIER>;

	// Pending resource transitions are committed before a command list
	// is executed on the command queue. This guarantees that resources will
	// be in the expected state at the beginning of a command list.
	ResourceBarriers pendingResourceBarriers;

	// Resource barriers that need to be committed to the command list.
	ResourceBarriers resourceBarriers;

	// Tracks the state of a particular resource and all of its subresources.
	struct ResourceState
	{
		// Initialize all of the subresources within a resource to the given state.
		explicit ResourceState(D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON)
			: state(state)
		{}

		// Set a subresource to a particular state.
		void SetSubresourceState(UINT subresource, D3D12_RESOURCE_STATES state)
		{
			if (subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES)
			{
				this->state = state;
				subresourceState.clear();
			}
			else
			{
				subresourceState[subresource] = state;
			}
		}

		// Get the state of a (sub)resource within the resource.
		// If the specified subresource is not found in the SubresourceState array (map)
		// then the state of the resource (D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES) is
		// returned.
		D3D12_RESOURCE_STATES GetSubresourceState(UINT subresource) const
		{
			D3D12_RESOURCE_STATES result = state;
			const auto iter = subresourceState.find(subresource);
			if (iter != subresourceState.end())
			{
				result = iter->second;
			}
			return result;
		}

		// If the SubresourceState array (map) is empty, then the State variable defines 
		// the state of all of the subresources.
		D3D12_RESOURCE_STATES state;
		MAP<effUINT, D3D12_RESOURCE_STATES> subresourceState;
	};

	using ResourceStateMap = UNORDERED_MAP<ID3D12Resource *, ResourceState>;

	// The final (last known state) of the resources within a command list.
	// The final resource state is committed to the global resource state when the 
	// command list is closed but before it is executed on the command queue.
	ResourceStateMap finalResourceState;

	// The global resource state array (map) stores the state of a resource
	// between command list execution.
	static ResourceStateMap globalResourceState;

	// The mutex protects shared access to the GlobalResourceState map.
	static EFFMutex globalMutex;
	static effBOOL isLocked;
};
