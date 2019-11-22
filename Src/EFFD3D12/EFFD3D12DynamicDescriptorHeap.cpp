#include "EFFD3D12PCH.h"

#include "EFFD3D12DynamicDescriptorHeap.h"
#include "EFFD3D12DeviceCommandList.h"
#include "EFFD3D12Device.h"
#include "EFFD3D12RootSignature.h"

EFFD3D12DynamicDescriptorHeap::EFFD3D12DynamicDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, effUINT32 numDescriptorsPerHeap)
	: descriptorHeapType(heapType)
	, numDescriptorsPerHeap(numDescriptorsPerHeap)
	, descriptorTableBitMask(0)
	, staleDescriptorTableBitMask(0)
	, currentCPUDescriptorHandle(D3D12_DEFAULT)
	, currentGPUDescriptorHandle(D3D12_DEFAULT)
	, numFreeHandles(0)
{
	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();
	descriptorHandleIncrementSize = d3d12Device->GetD3D12Device()->GetDescriptorHandleIncrementSize(heapType);

	// Allocate space for staging CPU visible descriptors.
	descriptorHandleCache = EFFNEW D3D12_CPU_DESCRIPTOR_HANDLE[numDescriptorsPerHeap];
}

EFFD3D12DynamicDescriptorHeap::~EFFD3D12DynamicDescriptorHeap()
{
}

effVOID EFFD3D12DynamicDescriptorHeap::ParseRootSignature(const EFFD3D12RootSignature & rootSignature)
{
	// If the root signature changes, all descriptors must be (re)bound to the
	// command list.
	staleDescriptorTableBitMask = 0;

	const auto & rootSignatureDesc = rootSignature.GetRootSignatureDesc();

	// Get a bit mask that represents the root parameter indices that match the 
	// descriptor heap type for this dynamic descriptor heap.
	descriptorTableBitMask = rootSignature.GetDescriptorTableBitMask(descriptorHeapType);
	effUINT32 _descriptorTableBitMask = descriptorTableBitMask;

	effUINT32 currentOffset = 0;
	DWORD rootIndex;
	while (_BitScanForward(&rootIndex, _descriptorTableBitMask) && rootIndex < rootSignatureDesc.NumParameters)
	{
		effUINT32 numDescriptors = rootSignature.GetNumDescriptors(rootIndex);

		DescriptorTableCache & descriptorTableCache = descriptorTableCaches[rootIndex];
		descriptorTableCache.numDescriptors = numDescriptors;
		descriptorTableCache.baseDescriptor = descriptorHandleCache + currentOffset;

		currentOffset += numDescriptors;

		// Flip the descriptor table bit so it's not scanned again for the current index.
		_descriptorTableBitMask ^= (1 << rootIndex);
	}

	// Make sure the maximum number of descriptors per descriptor heap has not been exceeded.
	assert(currentOffset <= numDescriptorsPerHeap && "The root signature requires more than the maximum number of descriptors per descriptor heap. Consider increasing the maximum number of descriptors per descriptor heap.");
}

effVOID EFFD3D12DynamicDescriptorHeap::StageDescriptors(effUINT32 rootParameterIndex, effUINT32 offset, effUINT32 numDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptor)
{
	// Cannot stage more than the maximum number of descriptors per heap.
	// Cannot stage more than MaxDescriptorTables root parameters.
	if (numDescriptors > numDescriptorsPerHeap || rootParameterIndex >= MaxDescriptorTables)
	{
		return;
	}

	DescriptorTableCache & descriptorTableCache = descriptorTableCaches[rootParameterIndex];

	// Check that the number of descriptors to copy does not exceed the number
	// of descriptors expected in the descriptor table.
	if ((offset + numDescriptors) > descriptorTableCache.numDescriptors)
	{
		throw std::length_error("Number of descriptors exceeds the number of descriptors in the descriptor table.");
	}

	D3D12_CPU_DESCRIPTOR_HANDLE * dstDescriptor = (descriptorTableCache.baseDescriptor + offset);
	for (effUINT32 i = 0; i < numDescriptors; ++i)
	{
		dstDescriptor[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(srcDescriptor, i, descriptorHandleIncrementSize);
	}

	// Set the root parameter index bit to make sure the descriptor table 
	// at that index is bound to the command list.
	staleDescriptorTableBitMask |= (1 << rootParameterIndex);
}

effUINT32 EFFD3D12DynamicDescriptorHeap::ComputeStaleDescriptorCount() const
{
	effUINT32 numStaleDescriptors = 0;
	DWORD i;
	effUINT32 staleDescriptorsBitMask = staleDescriptorTableBitMask;

	while (_BitScanForward(&i, staleDescriptorsBitMask))
	{
		numStaleDescriptors += descriptorTableCaches[i].numDescriptors;
		staleDescriptorsBitMask ^= (1 << i);
	}

	return numStaleDescriptors;
}

ComPtr<ID3D12DescriptorHeap> EFFD3D12DynamicDescriptorHeap::RequestDescriptorHeap()
{
	ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	if (!availableDescriptorHeaps.empty())
	{
		descriptorHeap = availableDescriptorHeaps.front();
		availableDescriptorHeaps.pop();
	}
	else
	{
		descriptorHeap = CreateDescriptorHeap();
		descriptorHeapPool.push(descriptorHeap);
	}

	return descriptorHeap;
}

ComPtr<ID3D12DescriptorHeap> EFFD3D12DynamicDescriptorHeap::CreateDescriptorHeap()
{
	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.Type = descriptorHeapType;
	descriptorHeapDesc.NumDescriptors = numDescriptorsPerHeap;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	DX_CHECK(d3d12Device->GetD3D12Device()->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap)));

	return descriptorHeap;
}

effVOID EFFD3D12DynamicDescriptorHeap::CommitStagedDescriptors(EFFD3D12DeviceCommandList & commandList, SetFunc setFunc)
{
	// Compute the number of descriptors that need to be copied 
	effUINT32 numDescriptorsToCommit = ComputeStaleDescriptorCount();

	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();

	if (numDescriptorsToCommit > 0)
	{

		auto d3d12GraphicsCommandList = commandList.GetD3D12CommandList();
		assert(d3d12GraphicsCommandList != nullptr);

		if (!currentDescriptorHeap || numFreeHandles < numDescriptorsToCommit)
		{
			currentDescriptorHeap = RequestDescriptorHeap();
			currentCPUDescriptorHandle = currentDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			currentGPUDescriptorHandle = currentDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
			numFreeHandles = numDescriptorsPerHeap;

			commandList.SetDescriptorHeap(descriptorHeapType, currentDescriptorHeap.Get());

			// When updating the descriptor heap on the command list, all descriptor
			// tables must be (re)recopied to the new descriptor heap (not just
			// the stale descriptor tables).
			staleDescriptorTableBitMask = descriptorTableBitMask;
		}

		DWORD rootIndex;
		// Scan from LSB to MSB for a bit set in staleDescriptorsBitMask
		while (_BitScanForward(&rootIndex, staleDescriptorTableBitMask))
		{
			effUINT32 numSrcDescriptors = descriptorTableCaches[rootIndex].numDescriptors;
			D3D12_CPU_DESCRIPTOR_HANDLE * pSrcDescriptorHandles = descriptorTableCaches[rootIndex].baseDescriptor;

			D3D12_CPU_DESCRIPTOR_HANDLE pDestDescriptorRangeStarts[] =
			{
				currentCPUDescriptorHandle
			};

			effUINT32 pDestDescriptorRangeSizes[] =
			{
				numSrcDescriptors
			};

			// Copy the staged CPU visible descriptors to the GPU visible descriptor heap.
			d3d12Device->GetD3D12Device()->CopyDescriptors(1, pDestDescriptorRangeStarts, pDestDescriptorRangeSizes,
				numSrcDescriptors, pSrcDescriptorHandles, nullptr, descriptorHeapType);

			// Set the descriptors on the command list using the passed-in setter function.
			(d3d12GraphicsCommandList->*setFunc)(rootIndex, currentGPUDescriptorHandle);

			// Offset current CPU and GPU descriptor handles.
			currentCPUDescriptorHandle.Offset(numSrcDescriptors, descriptorHandleIncrementSize);
			currentGPUDescriptorHandle.Offset(numSrcDescriptors, descriptorHandleIncrementSize);
			numFreeHandles -= numSrcDescriptors;

			// Flip the stale bit so the descriptor table is not recopied again unless it is updated with a new descriptor.
			staleDescriptorTableBitMask ^= (1 << rootIndex);
		}
	}
}

effVOID EFFD3D12DynamicDescriptorHeap::CommitStagedDescriptorsForDraw(EFFD3D12DeviceCommandList & commandList)
{
	CommitStagedDescriptors(commandList, &ID3D12GraphicsCommandList::SetGraphicsRootDescriptorTable);
}

void EFFD3D12DynamicDescriptorHeap::CommitStagedDescriptorsForDispatch(EFFD3D12DeviceCommandList & commandList)
{
	CommitStagedDescriptors(commandList, &ID3D12GraphicsCommandList::SetComputeRootDescriptorTable);
}

D3D12_GPU_DESCRIPTOR_HANDLE EFFD3D12DynamicDescriptorHeap::CopyDescriptor(EFFD3D12DeviceCommandList & comandList, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptor)
{
	if (!currentDescriptorHeap || numFreeHandles < 1)
	{
		currentDescriptorHeap = RequestDescriptorHeap();
		currentCPUDescriptorHandle = currentDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		currentGPUDescriptorHandle = currentDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
		numFreeHandles = numDescriptorsPerHeap;

		comandList.SetDescriptorHeap(descriptorHeapType, currentDescriptorHeap.Get());

		// When updating the descriptor heap on the command list, all descriptor
		// tables must be (re)recopied to the new descriptor heap (not just
		// the stale descriptor tables).
		staleDescriptorTableBitMask = descriptorTableBitMask;
	}

	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();

	D3D12_GPU_DESCRIPTOR_HANDLE hGPU = currentGPUDescriptorHandle;
	d3d12Device->GetD3D12Device()->CopyDescriptorsSimple(1, currentCPUDescriptorHandle, cpuDescriptor, descriptorHeapType);

	currentCPUDescriptorHandle.Offset(1, descriptorHandleIncrementSize);
	currentGPUDescriptorHandle.Offset(1, descriptorHandleIncrementSize);
	numFreeHandles -= 1;

	return hGPU;
}

effVOID EFFD3D12DynamicDescriptorHeap::Reset()
{
	availableDescriptorHeaps = descriptorHeapPool;
	currentDescriptorHeap.Reset();
	currentCPUDescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(D3D12_DEFAULT);
	currentGPUDescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(D3D12_DEFAULT);
	numFreeHandles = 0;
	descriptorTableBitMask = 0;
	staleDescriptorTableBitMask = 0;

	// Reset the table cache
	for (int i = 0; i < MaxDescriptorTables; ++i)
	{
		descriptorTableCaches[i].Reset();
	}
}
