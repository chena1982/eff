#include "EFFD3D12PCH.h"

#include "EFFD3D12UploadBuffer.h"
#include "EFFD3D12Device.h"



EFFD3D12UploadBuffer::EFFD3D12UploadBuffer(effUINT32 pageSize)
	: pageSize(pageSize)
{
}

EFFD3D12UploadBuffer::~EFFD3D12UploadBuffer()
{
}

EFFD3D12UploadBuffer::Allocation EFFD3D12UploadBuffer::Allocate(effUINT32 sizeInBytes, effUINT32 alignment)
{
	if (sizeInBytes > pageSize)
	{
		throw std::bad_alloc();
	}

	// If there is no current page, or the requested allocation exceeds the
	// remaining space in the current page, request a new page.
	if (!currentPage || !currentPage->HasSpace(sizeInBytes, alignment))
	{
		currentPage = RequestPage();
	}

	return currentPage->Allocate(sizeInBytes, alignment);
}

EFFD3D12UploadBuffer::Page * EFFD3D12UploadBuffer::RequestPage()
{
	Page * page;

	if (!availablePages.empty())
	{
		page = availablePages.front();
		availablePages.pop_front();
	}
	else
	{
		page = EFFNEW Page(pageSize);
		pagePool.push_back(page);
	}

	return page;
}

effVOID EFFD3D12UploadBuffer::Reset()
{
	currentPage = nullptr;
	// Reset all available pages.
	availablePages = pagePool;

	for (auto page : availablePages)
	{
		// Reset the page for new allocations.
		page->Reset();
	}
}

EFFD3D12UploadBuffer::Page::Page(effUINT32 sizeInBytes)
	: pageSize(sizeInBytes)
	, offset(0)
	, CPUPtr(nullptr)
	, GPUPtr(D3D12_GPU_VIRTUAL_ADDRESS(0))
{
	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();

	DX_CHECK(d3d12Device->GetD3D12Device()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(pageSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&d3d12Resource)
	));

	GPUPtr = d3d12Resource->GetGPUVirtualAddress();
	d3d12Resource->Map(0, nullptr, &CPUPtr);
}

EFFD3D12UploadBuffer::Page::~Page()
{
	d3d12Resource->Unmap(0, nullptr);
	CPUPtr = nullptr;
	GPUPtr = D3D12_GPU_VIRTUAL_ADDRESS(0);
}

effBOOL EFFD3D12UploadBuffer::Page::HasSpace(effUINT32 sizeInBytes, effUINT32 alignment) const
{
	effUINT32 alignedSize = AlignUp(sizeInBytes, alignment);
	effUINT32 alignedOffset = AlignUp(offset, alignment);

	return alignedOffset + alignedSize <= pageSize;
}

EFFD3D12UploadBuffer::Allocation EFFD3D12UploadBuffer::Page::Allocate(effUINT32 sizeInBytes, effUINT32 alignment)
{
	if (!HasSpace(sizeInBytes, alignment))
	{
		// Can't allocate space from page.
		return Allocation();
	}

	effUINT32 alignedSize = AlignUp(sizeInBytes, alignment);
	offset = AlignUp(offset, alignment);

	Allocation allocation;
	allocation.CPU = (effBYTE *)CPUPtr + offset;
	allocation.GPU = GPUPtr + offset;

	offset += alignedSize;

	return allocation;
}

effVOID EFFD3D12UploadBuffer::Page::Reset()
{
	offset = 0;
}