#include "EFFD3D12PCH.h"

#include "EFFD3D12Descriptor.h"
#include "EFFD3D12Device.h"


EFFD3D12DescriptorAllocation::EFFD3D12DescriptorAllocation()
	: descriptor{ 0 }
	, numHandles(0)
	, descriptorSize(0)
	, page(nullptr)
{}

EFFD3D12DescriptorAllocation::EFFD3D12DescriptorAllocation(D3D12_CPU_DESCRIPTOR_HANDLE descriptor, effUINT32 numHandles, effUINT32 descriptorSize, EFFD3D12DescriptorAllocatorPage * page)
	: descriptor(descriptor)
	, numHandles(numHandles)
	, descriptorSize(descriptorSize)
	, page(page)
{}


EFFD3D12DescriptorAllocation::~EFFD3D12DescriptorAllocation()
{
	Free();
}

EFFD3D12DescriptorAllocation::EFFD3D12DescriptorAllocation(EFFD3D12DescriptorAllocation && allocation)
	: descriptor(allocation.descriptor)
	, numHandles(allocation.numHandles)
	, descriptorSize(allocation.descriptorSize)
	, page(std::move(allocation.page))
{
	allocation.descriptor.ptr = 0;
	allocation.numHandles = 0;
	allocation.descriptorSize = 0;
}

EFFD3D12DescriptorAllocation & EFFD3D12DescriptorAllocation::operator=(EFFD3D12DescriptorAllocation && other)
{
	// Free this descriptor if it points to anything.
	Free();

	descriptor = other.descriptor;
	numHandles = other.numHandles;
	descriptorSize = other.descriptorSize;
	page = std::move(other.page);

	other.descriptor.ptr = 0;
	other.numHandles = 0;
	other.descriptorSize = 0;

	return *this;
}

effVOID EFFD3D12DescriptorAllocation::Free()
{
	if (!IsNull() && page != nullptr)
	{
		EFFD3D12Device * device = (EFFD3D12Device *)EFF3DGetDevice();
		page->Free(std::move(*this), device->GetFrame());

		descriptor.ptr = 0;
		numHandles = 0;
		descriptorSize = 0;
		page = nullptr;
	}
}

// Check if this a valid descriptor.
effBOOL EFFD3D12DescriptorAllocation::IsNull() const
{
	return descriptor.ptr == 0;
}

// Get a descriptor at a particular offset in the allocation.
D3D12_CPU_DESCRIPTOR_HANDLE EFFD3D12DescriptorAllocation::GetDescriptorHandle(effUINT32 offset) const
{
	assert(offset < numHandles);
	return { descriptor.ptr + (descriptorSize * offset) };
}

effUINT32 EFFD3D12DescriptorAllocation::GetNumHandles() const
{
	return numHandles;
}

EFFD3D12DescriptorAllocatorPage * EFFD3D12DescriptorAllocation::GetDescriptorAllocatorPage() const
{
	return page;
}



EFFD3D12DescriptorAllocatorPage::EFFD3D12DescriptorAllocatorPage(D3D12_DESCRIPTOR_HEAP_TYPE type, effUINT32 numDescriptors)
	: heapType(type)
	, numDescriptorsInHeap(numDescriptors)
{
	EFFD3D12Device * device = (EFFD3D12Device *)EFF3DGetDevice();
	ID3D12Device * d3d12Device = device->GetD3D12Device();

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = heapType;
	heapDesc.NumDescriptors = numDescriptorsInHeap;

	DX_CHECK(d3d12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&d3d12DescriptorHeap)));

	baseDescriptor = d3d12DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	descriptorHandleIncrementSize = d3d12Device->GetDescriptorHandleIncrementSize(heapType);
	numFreeHandles = numDescriptorsInHeap;

	// Initialize the free lists
	AddNewBlock(0, numFreeHandles);
}

D3D12_DESCRIPTOR_HEAP_TYPE EFFD3D12DescriptorAllocatorPage::GetHeapType() const
{
	return heapType;
}

effUINT32 EFFD3D12DescriptorAllocatorPage::NumFreeHandles() const
{
	return numFreeHandles;
}

effBOOL EFFD3D12DescriptorAllocatorPage::HasSpace(effUINT32 numDescriptors) const
{
	return freeListBySize.lower_bound(numDescriptors) != freeListBySize.end();
}

effVOID EFFD3D12DescriptorAllocatorPage::AddNewBlock(effUINT32 offset, effUINT32 numDescriptors)
{
	auto offsetIt = freeListByOffset.emplace(offset, numDescriptors);
	auto sizeIt = freeListBySize.emplace(numDescriptors, offsetIt.first);
	offsetIt.first->second.FreeListBySizeIt = sizeIt;
}

EFFD3D12DescriptorAllocation EFFD3D12DescriptorAllocatorPage::Allocate(effUINT32 numDescriptors)
{
	allocationMutex.Lock();
	ON_SCOPE_EXIT([&] { allocationMutex.Unlock(); })

	// There are less than the requested number of descriptors left in the heap.
	// Return a NULL descriptor and try another heap.
	if (numDescriptors > numFreeHandles)
	{
		return EFFD3D12DescriptorAllocation();
	}

	// Get the first block that is large enough to satisfy the request.
	auto smallestBlockIt = freeListBySize.lower_bound(numDescriptors);
	if (smallestBlockIt == freeListBySize.end())
	{
		// There was no free block that could satisfy the request.
		return EFFD3D12DescriptorAllocation();
	}

	// The size of the smallest block that satisfies the request.
	auto blockSize = smallestBlockIt->first;

	// The pointer to the same entry in the FreeListByOffset map.
	auto offsetIt = smallestBlockIt->second;

	// The offset in the descriptor heap.
	auto offset = offsetIt->first;

	// Remove the existing free block from the free list.
	freeListBySize.erase(smallestBlockIt);
	freeListByOffset.erase(offsetIt);

	// Compute the new free block that results from splitting this block.
	auto newOffset = offset + numDescriptors;
	auto newSize = blockSize - numDescriptors;

	if (newSize > 0)
	{
		// If the allocation didn't exactly match the requested size,
		// return the left-over to the free list.
		AddNewBlock(newOffset, newSize);
	}

	// Decrement free handles.
	numFreeHandles -= numDescriptors;

	return EFFD3D12DescriptorAllocation(
		CD3DX12_CPU_DESCRIPTOR_HANDLE(baseDescriptor, offset, descriptorHandleIncrementSize),
		numDescriptors, descriptorHandleIncrementSize, this);
}

effUINT32 EFFD3D12DescriptorAllocatorPage::ComputeOffset(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
	return static_cast<effUINT32>(handle.ptr - baseDescriptor.ptr) / descriptorHandleIncrementSize;
}

effVOID EFFD3D12DescriptorAllocatorPage::Free(EFFD3D12DescriptorAllocation && descriptor, effUINT64 frameNumber)
{
	// Compute the offset of the descriptor within the descriptor heap.
	auto offset = ComputeOffset(descriptor.GetDescriptorHandle());

	allocationMutex.Lock();
	ON_SCOPE_EXIT([&] { allocationMutex.Unlock(); })

	// Don't add the block directly to the free list until the frame has completed.
	staleDescriptors.emplace_back(offset, descriptor.GetNumHandles(), frameNumber);
}

effVOID EFFD3D12DescriptorAllocatorPage::FreeBlock(effUINT32 offset, effUINT32 numDescriptors)
{
	// Find the first element whose offset is greater than the specified offset.
	// This is the block that should appear after the block that is being freed.
	auto nextBlockIt = freeListByOffset.upper_bound(offset);

	// Find the block that appears before the block being freed.
	auto prevBlockIt = nextBlockIt;
	// If it's not the first block in the list.
	if (prevBlockIt != freeListByOffset.begin())
	{
		// Go to the previous block in the list.
		--prevBlockIt;
	}
	else
	{
		// Otherwise, just set it to the end of the list to indicate that no
		// block comes before the one being freed.
		prevBlockIt = freeListByOffset.end();
	}

	// Add the number of free handles back to the heap.
	// This needs to be done before merging any blocks since merging
	// blocks modifies the numDescriptors variable.
	numFreeHandles += numDescriptors;

	if (prevBlockIt != freeListByOffset.end() &&
		offset == prevBlockIt->first + prevBlockIt->second.Size)
	{
		// The previous block is exactly behind the block that is to be freed.
		//
		// PrevBlock.Offset           Offset
		// |                          |
		// |<-----PrevBlock.Size----->|<------Size-------->|
		//

		// Increase the block size by the size of merging with the previous block.
		offset = prevBlockIt->first;
		numDescriptors += prevBlockIt->second.Size;

		// Remove the previous block from the free list.
		freeListBySize.erase(prevBlockIt->second.FreeListBySizeIt);
		freeListByOffset.erase(prevBlockIt);
	}

	if (nextBlockIt != freeListByOffset.end() &&
		offset + numDescriptors == nextBlockIt->first)
	{
		// The next block is exactly in front of the block that is to be freed.
		//
		// Offset               NextBlock.Offset 
		// |                    |
		// |<------Size-------->|<-----NextBlock.Size----->|

		// Increase the block size by the size of merging with the next block.
		numDescriptors += nextBlockIt->second.Size;

		// Remove the next block from the free list.
		freeListBySize.erase(nextBlockIt->second.FreeListBySizeIt);
		freeListByOffset.erase(nextBlockIt);
	}

	// Add the freed block to the free list.
	AddNewBlock(offset, numDescriptors);
}

effVOID EFFD3D12DescriptorAllocatorPage::ReleaseStaleDescriptors(effUINT64 frameNumber)
{
	allocationMutex.Lock();
	ON_SCOPE_EXIT([&] { allocationMutex.Unlock(); })

	while (!staleDescriptors.empty() && staleDescriptors.front().FrameNumber <= frameNumber)
	{
		auto& staleDescriptor = staleDescriptors.front();

		// The offset of the descriptor in the heap.
		auto offset = staleDescriptor.Offset;
		// The number of descriptors that were allocated.
		auto numDescriptors = staleDescriptor.Size;

		FreeBlock(offset, numDescriptors);

		staleDescriptors.pop();
	}
}




EFFD3D12DescriptorAllocator::EFFD3D12DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptorsPerHeap)
	: heapType(type)
	, numDescriptorsPerHeap(numDescriptorsPerHeap)
{
}

EFFD3D12DescriptorAllocator::~EFFD3D12DescriptorAllocator()
{}

EFFD3D12DescriptorAllocatorPage * EFFD3D12DescriptorAllocator::CreateAllocatorPage()
{
	auto newPage = EFFNEW EFFD3D12DescriptorAllocatorPage(heapType, numDescriptorsPerHeap);

	heapPool.emplace_back(newPage);
	availableHeaps.insert(heapPool.size() - 1);

	return newPage;
}

EFFD3D12DescriptorAllocation EFFD3D12DescriptorAllocator::Allocate(effUINT32 numDescriptors)
{
	allocationMutex.Lock();
	ON_SCOPE_EXIT([&] { allocationMutex.Unlock(); })

	EFFD3D12DescriptorAllocation allocation;

	for (auto iter = availableHeaps.begin(); iter != availableHeaps.end(); ++iter)
	{
		auto allocatorPage = heapPool[*iter];

		allocation = allocatorPage->Allocate(numDescriptors);

		if (allocatorPage->NumFreeHandles() == 0)
		{
			iter = availableHeaps.erase(iter);
		}

		// A valid allocation has been found.
		if (!allocation.IsNull())
		{
			break;
		}
	}

	// No available heap could satisfy the requested number of descriptors.
	if (allocation.IsNull())
	{
		numDescriptorsPerHeap = MAX(numDescriptorsPerHeap, numDescriptors);
		auto newPage = CreateAllocatorPage();

		allocation = newPage->Allocate(numDescriptors);
	}

	return allocation;
}

effVOID EFFD3D12DescriptorAllocator::ReleaseStaleDescriptors(uint64_t frameNumber)
{
	allocationMutex.Lock();
	ON_SCOPE_EXIT([&] { allocationMutex.Unlock(); })

	for (effUINT32 i = 0; i < heapPool.size(); ++i)
	{
		auto page = heapPool[i];

		page->ReleaseStaleDescriptors(frameNumber);

		if (page->NumFreeHandles() > 0)
		{
			availableHeaps.insert(i);
		}
	}
}


