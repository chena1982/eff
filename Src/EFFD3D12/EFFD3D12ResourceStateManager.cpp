#include "EFFD3D12PCH.h"

#include "EFFD3D12ResourceStateManager.h"
#include "EFFD3D12Device.h"
#include "EFFD3D12VertexBuffer.h"
#include "EFFD3D12DeviceCommandList.h"

EFFMutex EFFD3D12ResourceStateManager::globalMutex;
effBOOL EFFD3D12ResourceStateManager::isLocked = effFALSE;
EFFD3D12ResourceStateManager::ResourceStateMap EFFD3D12ResourceStateManager::globalResourceState;

EFFD3D12ResourceStateManager::EFFD3D12ResourceStateManager()
{
	isLocked = effFALSE;
}

EFFD3D12ResourceStateManager::~EFFD3D12ResourceStateManager()
{
}

effVOID EFFD3D12ResourceStateManager::ResourceBarrier(const D3D12_RESOURCE_BARRIER & barrier)
{
	if (barrier.Type == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION)
	{
		const D3D12_RESOURCE_TRANSITION_BARRIER & transitionBarrier = barrier.Transition;

		// First check if there is already a known "final" state for the given resource.
		// If there is, the resource has been used on the command list before and
		// already has a known state within the command list execution.
		const auto iter = finalResourceState.find(transitionBarrier.pResource);
		if (iter != finalResourceState.end())
		{
			auto & resourceState = iter->second;
			// If the known final state of the resource is different...
			if (transitionBarrier.Subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES &&
				!resourceState.subresourceState.empty())
			{
				// First transition all of the subresources if they are different than the StateAfter.
				for (auto subresourceState : resourceState.subresourceState)
				{
					if (transitionBarrier.StateAfter != subresourceState.second)
					{
						D3D12_RESOURCE_BARRIER newBarrier = barrier;
						newBarrier.Transition.Subresource = subresourceState.first;
						newBarrier.Transition.StateBefore = subresourceState.second;
						resourceBarriers.push_back(newBarrier);
					}
				}
			}
			else
			{
				auto finalState = resourceState.GetSubresourceState(transitionBarrier.Subresource);
				if (transitionBarrier.StateAfter != finalState)
				{
					// Push a new transition barrier with the correct before state.
					D3D12_RESOURCE_BARRIER newBarrier = barrier;
					newBarrier.Transition.StateBefore = finalState;
					resourceBarriers.push_back(newBarrier);
				}
			}
		}
		else // In this case, the resource is being used on the command list for the first time. 
		{
			// Add a pending barrier. The pending barriers will be resolved
			// before the command list is executed on the command queue.
			pendingResourceBarriers.push_back(barrier);
		}

		// Push the final known state (possibly replacing the previously known state for the subresource).
		finalResourceState[transitionBarrier.pResource].SetSubresourceState(transitionBarrier.Subresource, transitionBarrier.StateAfter);
	}
	else
	{
		// Just push non-transition barriers to the resource barriers array.
		resourceBarriers.push_back(barrier);
	}
}

effVOID EFFD3D12ResourceStateManager::TransitionResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateAfter, effUINT subResource)
{
	if (resource)
	{
		ResourceBarrier(CD3DX12_RESOURCE_BARRIER::Transition(resource, D3D12_RESOURCE_STATE_COMMON, stateAfter, subResource));
	}
}

ID3D12Resource * EFFD3D12ResourceStateManager::GetD3D12Resource(const EFF3DResource * resource)
{
	ID3D12Resource * d3d12Resource = nullptr;
	if (resource->type == EFF3DResourceType_Texture2D || resource->type == EFF3DResourceType_Texture3D ||
		resource->type == EFF3DResourceType_TextureCube || resource->type == EFF3DResourceType_RenderTarget)
	{

	}

	if (resource->type == EFF3DResourceType_VertexBuffer)
	{
		const EFFD3D12VertexBuffer * vertexBuffer = (const EFFD3D12VertexBuffer *)resource;
		d3d12Resource = vertexBuffer->d3d12Resource.Get();

	}

	if (resource->type == EFF3DResourceType_IndexBuffer)
	{

	}

	if (resource->type == EFF3DResourceType_TimeQuery)
	{

	}

	return d3d12Resource;
}

effVOID EFFD3D12ResourceStateManager::TransitionResource(const EFF3DResource * resource, D3D12_RESOURCE_STATES stateAfter, UINT subResource)
{
	TransitionResource(GetD3D12Resource(resource), stateAfter, subResource);
}

effVOID EFFD3D12ResourceStateManager::UAVBarrier(const EFF3DResource * resource)
{
	ResourceBarrier(CD3DX12_RESOURCE_BARRIER::UAV(GetD3D12Resource(resource)));
}

effVOID EFFD3D12ResourceStateManager::AliasBarrier(const EFF3DResource * resourceBefore, const EFF3DResource * resourceAfter)
{

	ResourceBarrier(CD3DX12_RESOURCE_BARRIER::Aliasing(GetD3D12Resource(resourceBefore), GetD3D12Resource(resourceAfter)));
}

effVOID EFFD3D12ResourceStateManager::FlushResourceBarriers(EFFD3D12DeviceCommandList * commandList)
{
	effUINT32 numBarriers = static_cast<effUINT32>(resourceBarriers.size());
	if (numBarriers > 0)
	{
		commandList->GetD3D12CommandList()->ResourceBarrier(numBarriers, resourceBarriers.data());
		resourceBarriers.clear();
	}
}

effUINT32 EFFD3D12ResourceStateManager::FlushPendingResourceBarriers(EFFD3D12DeviceCommandList * commandList)
{
	assert(isLocked);

	// Resolve the pending resource barriers by checking the global state of the 
	// (sub)resources. Add barriers if the pending state and the global state do
	//  not match.
	ResourceBarriers resourceBarriers;
	// Reserve enough space (worst-case, all pending barriers).
	resourceBarriers.reserve(pendingResourceBarriers.size());

	for (auto pendingBarrier : pendingResourceBarriers)
	{
		if (pendingBarrier.Type == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION)  // Only transition barriers should be pending...
		{
			auto pendingTransition = pendingBarrier.Transition;

			const auto & iter = globalResourceState.find(pendingTransition.pResource);
			if (iter != globalResourceState.end())
			{
				// If all subresources are being transitioned, and there are multiple
				// subresources of the resource that are in a different state...
				auto& resourceState = iter->second;
				if (pendingTransition.Subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES &&
					!resourceState.subresourceState.empty())
				{
					// Transition all subresources
					for (auto subresourceState : resourceState.subresourceState)
					{
						if (pendingTransition.StateAfter != subresourceState.second)
						{
							D3D12_RESOURCE_BARRIER newBarrier = pendingBarrier;
							newBarrier.Transition.Subresource = subresourceState.first;
							newBarrier.Transition.StateBefore = subresourceState.second;
							resourceBarriers.push_back(newBarrier);
						}
					}
				}
				else
				{
					// No (sub)resources need to be transitioned. Just add a single transition barrier (if needed).
					auto globalState = (iter->second).GetSubresourceState(pendingTransition.Subresource);
					if (pendingTransition.StateAfter != globalState)
					{
						// Fix-up the before state based on current global state of the resource.
						pendingBarrier.Transition.StateBefore = globalState;
						resourceBarriers.push_back(pendingBarrier);
					}
				}
			}
		}
	}

	effUINT32 numBarriers = static_cast<effUINT32>(resourceBarriers.size());
	if (numBarriers > 0)
	{	
		commandList->GetD3D12CommandList()->ResourceBarrier(numBarriers, resourceBarriers.data());
	}

	pendingResourceBarriers.clear();

	return numBarriers;
}

effVOID EFFD3D12ResourceStateManager::CommitFinalResourceStates()
{
	assert(isLocked);

	// Commit final resource states to the global resource state array (map).
	for (const auto& resourceState : finalResourceState)
	{
		globalResourceState[resourceState.first] = resourceState.second;
	}

	finalResourceState.clear();
}

effVOID EFFD3D12ResourceStateManager::Reset()
{
	// Reset the pending, current, and final resource states.
	pendingResourceBarriers.clear();
	resourceBarriers.clear();
	finalResourceState.clear();
}

effVOID EFFD3D12ResourceStateManager::Lock()
{
	globalMutex.Lock();
	isLocked = effTRUE;
}

effVOID EFFD3D12ResourceStateManager::Unlock()
{
	globalMutex.Unlock();
	isLocked = effFALSE;
}

effVOID EFFD3D12ResourceStateManager::AddGlobalResourceState(ID3D12Resource* resource, D3D12_RESOURCE_STATES state)
{
	if (resource != nullptr)
	{
		globalMutex.Lock();
		ON_SCOPE_EXIT([&] { globalMutex.Unlock(); })
		globalResourceState[resource].SetSubresourceState(D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, state);
	}
}

effVOID EFFD3D12ResourceStateManager::RemoveGlobalResourceState(ID3D12Resource* resource)
{
	if (resource != nullptr)
	{
		globalMutex.Lock();
		ON_SCOPE_EXIT([&] { globalMutex.Unlock(); })
		globalResourceState.erase(resource);
	}
}


