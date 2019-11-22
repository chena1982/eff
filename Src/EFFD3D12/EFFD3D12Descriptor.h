#pragma once


class EFFD3D12DescriptorAllocatorPage;

class EFFD3D12DescriptorAllocation
{
public:
	// Creates a NULL descriptor.
	EFFD3D12DescriptorAllocation();

	EFFD3D12DescriptorAllocation(D3D12_CPU_DESCRIPTOR_HANDLE descriptor, effUINT32 numHandles, effUINT32 descriptorSize, EFFD3D12DescriptorAllocatorPage * page);

	// The destructor will automatically free the allocation.
	~EFFD3D12DescriptorAllocation();

	// Copies are not allowed.
	EFFD3D12DescriptorAllocation(const EFFD3D12DescriptorAllocation &) = delete;
	EFFD3D12DescriptorAllocation & operator = (const EFFD3D12DescriptorAllocation &) = delete;

	// Move is allowed.
	EFFD3D12DescriptorAllocation(EFFD3D12DescriptorAllocation && allocation);
	EFFD3D12DescriptorAllocation & operator = (EFFD3D12DescriptorAllocation && other);

	// Check if this a valid descriptor.
	effBOOL IsNull() const;

	// Get a descriptor at a particular offset in the allocation.
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle(effUINT32 offset = 0) const;

	// Get the number of (consecutive) handles for this allocation.
	effUINT32 GetNumHandles() const;

	// Get the heap that this allocation came from.
	// (For internal use only).
	EFFD3D12DescriptorAllocatorPage * GetDescriptorAllocatorPage() const;

private:
	// Free the descriptor back to the heap it came from.
	effVOID Free();

	// The base descriptor.
	D3D12_CPU_DESCRIPTOR_HANDLE descriptor;
	// The number of descriptors in this allocation.
	effUINT numHandles;
	// The offset to the next descriptor.
	effUINT32 descriptorSize;

	// A pointer back to the original page where this allocation came from.
	EFFD3D12DescriptorAllocatorPage * page;
};

class EFFD3D12DescriptorAllocatorPage
{
public:
	EFFD3D12DescriptorAllocatorPage(D3D12_DESCRIPTOR_HEAP_TYPE type, effUINT32 numDescriptors);

	D3D12_DESCRIPTOR_HEAP_TYPE GetHeapType() const;

	/**
	* Check to see if this descriptor page has a contiguous block of descriptors
	* large enough to satisfy the request.
	*/
	effBOOL HasSpace(effUINT32 numDescriptors) const;

	/**
	* Get the number of available handles in the heap.
	*/
	effUINT32 NumFreeHandles() const;

	/**
	* Allocate a number of descriptors from this descriptor heap.
	* If the allocation cannot be satisfied, then a NULL descriptor
	* is returned.
	*/
	EFFD3D12DescriptorAllocation Allocate(effUINT32 numDescriptors);

	/**
	* Return a descriptor back to the heap.
	* @param frameNumber Stale descriptors are not freed directly, but put
	* on a stale allocations queue. Stale allocations are returned to the heap
	* using the DescriptorAllocatorPage::ReleaseStaleAllocations method.
	*/
	effVOID Free(EFFD3D12DescriptorAllocation && descriptorHandle, effUINT64 frameNumber);

	/**
	* Returned the stale descriptors back to the descriptor heap.
	*/
	effVOID ReleaseStaleDescriptors(effUINT64 frameNumber);

protected:

	// Compute the offset of the descriptor handle from the start of the heap.
	effUINT32 ComputeOffset(D3D12_CPU_DESCRIPTOR_HANDLE handle);

	// Adds a new block to the free list.
	effVOID AddNewBlock(effUINT32 offset, effUINT32 numDescriptors);

	// Free a block of descriptors.
	// This will also merge free blocks in the free list to form larger blocks
	// that can be reused.
	void FreeBlock(effUINT32 offset, effUINT32 numDescriptors);

private:
	// The offset (in descriptors) within the descriptor heap.
	using OffsetType = effUINT32;
	// The number of descriptors that are available.
	using SizeType = effUINT32;

	struct FreeBlockInfo;
	// A map that lists the free blocks by the offset within the descriptor heap.
	using FreeListByOffset = MAP<OffsetType, FreeBlockInfo>;

	// A map that lists the free blocks by size.
	// Needs to be a multimap since multiple blocks can have the same size.
	using FreeListBySize = MULTIMAP<SizeType, FreeListByOffset::iterator>;

	struct FreeBlockInfo
	{
		FreeBlockInfo(SizeType size)
			: Size(size)
		{}

		SizeType Size;
		FreeListBySize::iterator FreeListBySizeIt;
	};

	struct StaleDescriptorInfo
	{
		StaleDescriptorInfo(OffsetType offset, SizeType size, uint64_t frame)
			: Offset(offset)
			, Size(size)
			, FrameNumber(frame)
		{}

		// The offset within the descriptor heap.
		OffsetType Offset;
		// The number of descriptors
		SizeType Size;
		// The frame number that the descriptor was freed.
		effUINT64 FrameNumber;
	};

	// Stale descriptors are queued for release until the frame that they were freed
	// has completed.
	using StaleDescriptorQueue = QUEUE<StaleDescriptorInfo>;

	FreeListByOffset freeListByOffset;
	FreeListBySize freeListBySize;
	StaleDescriptorQueue staleDescriptors;

	ComPtr<ID3D12DescriptorHeap> d3d12DescriptorHeap;
	D3D12_DESCRIPTOR_HEAP_TYPE heapType;
	CD3DX12_CPU_DESCRIPTOR_HANDLE baseDescriptor;
	effUINT32 descriptorHandleIncrementSize;
	effUINT32 numDescriptorsInHeap;
	effUINT32 numFreeHandles;

	EFFMutex allocationMutex;
};





class EFFD3D12DescriptorAllocator
{
public:
	EFFD3D12DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type, effUINT32 numDescriptorsPerHeap = 256);
	virtual ~EFFD3D12DescriptorAllocator();

	/**
	 * Allocate a number of contiguous descriptors from a CPU visible descriptor heap.
	 *
	 * @param numDescriptors The number of contiguous descriptors to allocate.
	 * Cannot be more than the number of descriptors per descriptor heap.
	 */
	EFFD3D12DescriptorAllocation Allocate(effUINT32 numDescriptors = 1);

	/**
	 * When the frame has completed, the stale descriptors can be released.
	 */
	effVOID ReleaseStaleDescriptors(effUINT64 frameNumber);

private:
	using DescriptorHeapPool = VECTOR<EFFD3D12DescriptorAllocatorPage *>;

	// Create a new heap with a specific number of descriptors.
	EFFD3D12DescriptorAllocatorPage * CreateAllocatorPage();

	D3D12_DESCRIPTOR_HEAP_TYPE heapType;
	effUINT32 numDescriptorsPerHeap;

	DescriptorHeapPool heapPool;
	// Indices of available heaps in the heap pool.
	SET<effUINT64> availableHeaps;

	EFFMutex allocationMutex;
};


