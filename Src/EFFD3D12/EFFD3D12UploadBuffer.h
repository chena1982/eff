#pragma once

class EFFD3D12UploadBuffer
{
public:
	// Use to upload data to the GPU
	struct Allocation
	{
		effVOID * CPU;
		D3D12_GPU_VIRTUAL_ADDRESS GPU;

	public:
		Allocation()
		{
			CPU = nullptr;
			GPU = D3D12_GPU_VIRTUAL_ADDRESS(0);
		}
	};

	/**
	 * @param pageSize The size to use to allocate new pages in GPU memory.
	 */
	explicit EFFD3D12UploadBuffer(effUINT32 pageSize = _2MB);

	virtual ~EFFD3D12UploadBuffer();

	/**
	 * The maximum size of an allocation is the size of a single page.
	 */
	effUINT32 GetPageSize() const { return pageSize; }

	/**
	 * Allocate memory in an Upload heap.
	 * An allocation must not exceed the size of a page.
	 * Use a memcpy or similar method to copy the
	 * buffer data to CPU pointer in the Allocation structure returned from
	 * this function.
	 */
	Allocation Allocate(effUINT32 sizeInBytes, effUINT32 alignment);

	/**
	 * Release all allocated pages. This should only be done when the command list
	 * is finished executing on the CommandQueue.
	 */
	effVOID Reset();

private:
	// A single page for the allocator.
	struct Page
	{
		Page(effUINT32 sizeInBytes);
		~Page();

		// Check to see if the page has room to satisfy the requested
		// allocation.
		effBOOL HasSpace(effUINT32 sizeInBytes, effUINT32 alignment) const;

		// Allocate memory from the page.
		// Throws std::bad_alloc if the the allocation size is larger
		// that the page size or the size of the allocation exceeds the 
		// remaining space in the page.
		Allocation Allocate(effUINT32 sizeInBytes, effUINT32 alignment);

		// Reset the page for reuse.
		void Reset();

	private:

		ComPtr<ID3D12Resource> d3d12Resource;

		// Base pointer.
		effVOID * CPUPtr;
		D3D12_GPU_VIRTUAL_ADDRESS GPUPtr;

		// Allocated page size.
		effUINT32 pageSize;
		// Current allocation offset in bytes.
		effUINT32 offset;
	};

	// A pool of memory pages.
	using PagePool = DEQUE<Page *>;

	// Request a page from the pool of available pages
	// or create a new page if there are no available pages.
	Page * RequestPage();

	PagePool pagePool;
	PagePool availablePages;

	Page * currentPage;

	// The size of each page of memory.
	effUINT32 pageSize;

};