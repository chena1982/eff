#pragma once



class EFFD3D12RootSignature
{
public:
	// TODO: Add (deep) copy/move constructors and assignment operators!
	EFFD3D12RootSignature();
	EFFD3D12RootSignature(
		const D3D12_ROOT_SIGNATURE_DESC1& rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION rootSignatureVersion
	);

	virtual ~EFFD3D12RootSignature();

	effVOID Destroy();

	ComPtr<ID3D12RootSignature> GetRootSignature() const
	{
		return rootSignature;
	}

	effVOID SetRootSignatureDesc(
		const D3D12_ROOT_SIGNATURE_DESC1 & rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION rootSignatureVersion
	);

	const D3D12_ROOT_SIGNATURE_DESC1 & GetRootSignatureDesc() const
	{
		return rootSignatureDesc;
	}

	effUINT32 GetDescriptorTableBitMask(D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType) const;
	effUINT32 GetNumDescriptors(effUINT32 rootIndex) const;

protected:

private:
	D3D12_ROOT_SIGNATURE_DESC1 rootSignatureDesc;
	ComPtr<ID3D12RootSignature> rootSignature;

	// Need to know the number of descriptors per descriptor table.
	// A maximum of 32 descriptor tables are supported (since a 32-bit
	// mask is used to represent the descriptor tables in the root signature.
	effUINT32 numDescriptorsPerTable[32];

	// A bit mask that represents the root parameter indices that are 
	// descriptor tables for Samplers.
	effUINT32 samplerTableBitMask;
	// A bit mask that represents the root parameter indices that are 
	// CBV, UAV, and SRV descriptor tables.
	effUINT32 descriptorTableBitMask;
};