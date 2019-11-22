#pragma once



class EFFD3D12DeviceCommandList;
class EFFD3D12RootSignature;

class EFFD3D12DynamicDescriptorHeap
{
public:
	EFFD3D12DynamicDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		effUINT32 numDescriptorsPerHeap = 1024);

	virtual ~EFFD3D12DynamicDescriptorHeap();

	/**
	 * Stages a contiguous range of CPU visible descriptors.
	 * Descriptors are not copied to the GPU visible descriptor heap until
	 * the CommitStagedDescriptors function is called.
	 */
	effVOID StageDescriptors(effUINT32 rootParameterIndex, effUINT32 offset, effUINT32 numDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptors);

	/**
	 * Copy all of the staged descriptors to the GPU visible descriptor heap and
	 * bind the descriptor heap and the descriptor tables to the command list.
	 * The passed-in function object is used to set the GPU visible descriptors
	 * on the command list. Two possible functions are:
	 *   * Before a draw    : ID3D12GraphicsCommandList::SetGraphicsRootDescriptorTable
	 *   * Before a dispatch: ID3D12GraphicsCommandList::SetComputeRootDescriptorTable
	 *
	 * Since the DynamicDescriptorHeap can't know which function will be used, it must
	 * be passed as an argument to the function.
	 */

	typedef effVOID (ID3D12GraphicsCommandList::*SetFunc)(effUINT32, D3D12_GPU_DESCRIPTOR_HANDLE);

	effVOID CommitStagedDescriptors(EFFD3D12DeviceCommandList & commandList, SetFunc setFunc);
	effVOID CommitStagedDescriptorsForDraw(EFFD3D12DeviceCommandList & commandList);
	effVOID CommitStagedDescriptorsForDispatch(EFFD3D12DeviceCommandList & commandList);

	/**
	 * Copies a single CPU visible descriptor to a GPU visible descriptor heap.
	 * This is useful for the
	 *   * ID3D12GraphicsCommandList::ClearUnorderedAccessViewFloat
	 *   * ID3D12GraphicsCommandList::ClearUnorderedAccessViewUint
	 * methods which require both a CPU and GPU visible descriptors for a UAV
	 * resource.
	 *
	 * @param commandList The command list is required in case the GPU visible
	 * descriptor heap needs to be updated on the command list.
	 * @param cpuDescriptor The CPU descriptor to copy into a GPU visible
	 * descriptor heap.
	 *
	 * @return The GPU visible descriptor.
	 */
	D3D12_GPU_DESCRIPTOR_HANDLE CopyDescriptor(EFFD3D12DeviceCommandList & comandList, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptor);

	/**
	 * Parse the root signature to determine which root parameters contain
	 * descriptor tables and determine the number of descriptors needed for
	 * each table.
	 */
	effVOID ParseRootSignature(const EFFD3D12RootSignature & rootSignature);

	/**
	 * Reset used descriptors. This should only be done if any descriptors
	 * that are being referenced by a command list has finished executing on the
	 * command queue.
	 */
	effVOID Reset();

protected:

private:
	// Request a descriptor heap if one is available.
	ComPtr<ID3D12DescriptorHeap> RequestDescriptorHeap();
	// Create a new descriptor heap of no descriptor heap is available.
	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap();

	// Compute the number of stale descriptors that need to be copied
	// to GPU visible descriptor heap.
	effUINT32 ComputeStaleDescriptorCount() const;

	/**
	 * The maximum number of descriptor tables per root signature.
	 * A 32-bit mask is used to keep track of the root parameter indices that
	 * are descriptor tables.
	 */
	static const effUINT32 MaxDescriptorTables = 32;

	/**
	 * A structure that represents a descriptor table entry in the root signature.
	 */
	struct DescriptorTableCache
	{
		DescriptorTableCache()
			: numDescriptors(0)
			, baseDescriptor(nullptr)
		{}

		// Reset the table cache.
		effVOID Reset()
		{
			numDescriptors = 0;
			baseDescriptor = nullptr;
		}

		// The number of descriptors in this descriptor table.
		effUINT32 numDescriptors;
		// The pointer to the descriptor in the descriptor handle cache.
		D3D12_CPU_DESCRIPTOR_HANDLE* baseDescriptor;
	};

	// Describes the type of descriptors that can be staged using this 
	// dynamic descriptor heap.
	// Valid values are:
	//   * D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
	//   * D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER
	// This parameter also determines the type of GPU visible descriptor heap to 
	// create.
	D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType;

	// The number of descriptors to allocate in new GPU visible descriptor heaps.
	effUINT32 numDescriptorsPerHeap;

	// The increment size of a descriptor.
	effUINT32 descriptorHandleIncrementSize;

	// The descriptor handle cache.
	D3D12_CPU_DESCRIPTOR_HANDLE * descriptorHandleCache;

	// Descriptor handle cache per descriptor table.
	DescriptorTableCache descriptorTableCaches[MaxDescriptorTables];

	// Each bit in the bit mask represents the index in the root signature
	// that contains a descriptor table.
	effUINT32 descriptorTableBitMask;
	// Each bit set in the bit mask represents a descriptor table
	// in the root signature that has changed since the last time the 
	// descriptors were copied.
	effUINT32 staleDescriptorTableBitMask;

	using DescriptorHeapPool = QUEUE<ComPtr<ID3D12DescriptorHeap> >;

	DescriptorHeapPool descriptorHeapPool;
	DescriptorHeapPool availableDescriptorHeaps;

	ComPtr<ID3D12DescriptorHeap> currentDescriptorHeap;
	CD3DX12_GPU_DESCRIPTOR_HANDLE currentGPUDescriptorHandle;
	CD3DX12_CPU_DESCRIPTOR_HANDLE currentCPUDescriptorHandle;

	effUINT32 numFreeHandles;

};