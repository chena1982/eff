#include "EFFD3D12PCH.h"

#include "EFFD3D12DeviceCommandList.h"
#include "EFFD3D12Device.h"
#include "EFFD3D12ResourceStateManager.h"
#include "EFFD3D12UploadBuffer.h"
#include "EFFD3D12DynamicDescriptorHeap.h"

EFFD3D12DeviceCommandList::EFFD3D12DeviceCommandList(D3D12_COMMAND_LIST_TYPE type)
{
	d3d12CommandListType = type;
}


EFFD3D12DeviceCommandList::~EFFD3D12DeviceCommandList()
{

}

effVOID EFFD3D12DeviceCommandList::TransitionBarrier(ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES stateAfter, effUINT32 subresource, effBOOL flushBarriers)
{
	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();
	EFFD3D12ResourceStateManager * resourceStateManager = d3d12Device->GetResourceStateManager();
	if (resource)
	{
		// The "before" state is not important. It will be resolved by the resource state tracker.
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), D3D12_RESOURCE_STATE_COMMON, stateAfter, subresource);

		resourceStateManager->ResourceBarrier(barrier);
	}

	if (flushBarriers)
	{
		resourceStateManager->FlushResourceBarriers(this);
	}
}

template<typename ResourceType>
effVOID EFFD3D12DeviceCommandList::TransitionBarrier(const ResourceType * resource, D3D12_RESOURCE_STATES stateAfter, effUINT32 subresource, effBOOL flushBarriers)
{
	TransitionBarrier(resource->d3d12Resource, stateAfter, subresource, flushBarriers);
}


effVOID EFFD3D12DeviceCommandList::TrackResource(ComPtr<ID3D12Object> object)
{
	trackedObjects.push_back(object);
}



effVOID EFFD3D12DeviceCommandList::ReleaseTrackedObjects()
{
	trackedObjects.clear();
}


effBOOL EFFD3D12DeviceCommandList::Close(EFFD3D12DeviceCommandList * pendingCommandList)
{
	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();
	EFFD3D12ResourceStateManager * resourceStateManager = d3d12Device->GetResourceStateManager();

	// Flush any remaining barriers.
	resourceStateManager->FlushResourceBarriers(this);

	d3d12CommandList->Close();

	// Flush pending resource barriers.
	effUINT32 numPendingBarriers = resourceStateManager->FlushPendingResourceBarriers(pendingCommandList);
	// Commit the final resource state to the global state.
	resourceStateManager->CommitFinalResourceStates();

	return numPendingBarriers > 0;
}

effVOID EFFD3D12DeviceCommandList::Close()
{
	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();
	EFFD3D12ResourceStateManager * resourceStateManager = d3d12Device->GetResourceStateManager();

	resourceStateManager->FlushResourceBarriers(this);
	d3d12CommandList->Close();
}

effVOID EFFD3D12DeviceCommandList::Reset()
{
	DX_CHECK(d3d12CommandAllocator->Reset());
	DX_CHECK(d3d12CommandList->Reset(d3d12CommandAllocator.Get(), nullptr));

	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();
	EFFD3D12ResourceStateManager * resourceStateManager = d3d12Device->GetResourceStateManager();

	resourceStateManager->Reset();
	uploadBuffer->Reset();

	ReleaseTrackedObjects();

	for (int i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i)
	{
		dynamicDescriptorHeap[i]->Reset();
		descriptorHeaps[i] = nullptr;
	}

	rootSignature = nullptr;
	computeCommandList = nullptr;
}

effVOID EFFD3D12DeviceCommandList::SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, ID3D12DescriptorHeap* heap)
{
	if (descriptorHeaps[heapType] != heap)
	{
		descriptorHeaps[heapType] = heap;
		BindDescriptorHeaps();
	}
}

effVOID EFFD3D12DeviceCommandList::BindDescriptorHeaps()
{
	effUINT32 numDescriptorHeaps = 0;
	ID3D12DescriptorHeap* descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] = {};

	for (effUINT32 i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i)
	{
		ID3D12DescriptorHeap* descriptorHeap = descriptorHeaps[i];
		if (descriptorHeap)
		{
			descriptorHeaps[numDescriptorHeaps] = descriptorHeap;
			numDescriptorHeaps++;
		}
	}

	d3d12CommandList->SetDescriptorHeaps(numDescriptorHeaps, descriptorHeaps);
}