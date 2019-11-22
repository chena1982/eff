#include "EFFD3D12PCH.h"

#include "EFFD3D12RootSignature.h"
#include "EFFD3D12Device.h"


EFFD3D12RootSignature::EFFD3D12RootSignature()
	: rootSignatureDesc{}
	, numDescriptorsPerTable{ 0 }
	, samplerTableBitMask(0)
	, descriptorTableBitMask(0)
{}

EFFD3D12RootSignature::EFFD3D12RootSignature(
	const D3D12_ROOT_SIGNATURE_DESC1& rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION rootSignatureVersion)
	: rootSignatureDesc{}
	, numDescriptorsPerTable{ 0 }
	, samplerTableBitMask(0)
	, descriptorTableBitMask(0)
{
	SetRootSignatureDesc(rootSignatureDesc, rootSignatureVersion);
}

EFFD3D12RootSignature::~EFFD3D12RootSignature()
{
	Destroy();
}

effVOID EFFD3D12RootSignature::Destroy()
{
	for (UINT i = 0; i < rootSignatureDesc.NumParameters; ++i)
	{
		const D3D12_ROOT_PARAMETER1& rootParameter = rootSignatureDesc.pParameters[i];
		if (rootParameter.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
		{
			delete[] rootParameter.DescriptorTable.pDescriptorRanges;
		}
	}

	delete[] rootSignatureDesc.pParameters;
	rootSignatureDesc.pParameters = nullptr;
	rootSignatureDesc.NumParameters = 0;

	delete[] rootSignatureDesc.pStaticSamplers;
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.NumStaticSamplers = 0;

	descriptorTableBitMask = 0;
	samplerTableBitMask = 0;

	memset(numDescriptorsPerTable, 0, sizeof(numDescriptorsPerTable));
}

effVOID EFFD3D12RootSignature::SetRootSignatureDesc(
	const D3D12_ROOT_SIGNATURE_DESC1 & _rootSignatureDesc,
	D3D_ROOT_SIGNATURE_VERSION rootSignatureVersion
)
{
	// Make sure any previously allocated root signature description is cleaned 
	// up first.
	Destroy();

	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();

	effUINT32 numParameters = _rootSignatureDesc.NumParameters;
	D3D12_ROOT_PARAMETER1* pParameters = numParameters > 0 ? EFFNEW D3D12_ROOT_PARAMETER1[numParameters] : nullptr;

	for (effUINT32 i = 0; i < numParameters; ++i)
	{
		const D3D12_ROOT_PARAMETER1& rootParameter = _rootSignatureDesc.pParameters[i];
		pParameters[i] = rootParameter;

		if (rootParameter.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
		{
			effUINT32 numDescriptorRanges = rootParameter.DescriptorTable.NumDescriptorRanges;
			D3D12_DESCRIPTOR_RANGE1* pDescriptorRanges = numDescriptorRanges > 0 ? new D3D12_DESCRIPTOR_RANGE1[numDescriptorRanges] : nullptr;

			memcpy(pDescriptorRanges, rootParameter.DescriptorTable.pDescriptorRanges,
				sizeof(D3D12_DESCRIPTOR_RANGE1) * numDescriptorRanges);

			pParameters[i].DescriptorTable.NumDescriptorRanges = numDescriptorRanges;
			pParameters[i].DescriptorTable.pDescriptorRanges = pDescriptorRanges;

			// Set the bit mask depending on the type of descriptor table.
			if (numDescriptorRanges > 0)
			{
				switch (pDescriptorRanges[0].RangeType)
				{
				case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
				case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
				case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
					descriptorTableBitMask |= (1 << i);
					break;
				case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
					samplerTableBitMask |= (1 << i);
					break;
				}
			}

			// Count the number of descriptors in the descriptor table.
			for (UINT j = 0; j < numDescriptorRanges; ++j)
			{
				numDescriptorsPerTable[i] += pDescriptorRanges[j].NumDescriptors;
			}
		}
	}

	rootSignatureDesc.NumParameters = numParameters;
	rootSignatureDesc.pParameters = pParameters;

	effUINT32 numStaticSamplers = _rootSignatureDesc.NumStaticSamplers;
	D3D12_STATIC_SAMPLER_DESC* pStaticSamplers = numStaticSamplers > 0 ? new D3D12_STATIC_SAMPLER_DESC[numStaticSamplers] : nullptr;

	if (pStaticSamplers)
	{
		memcpy(pStaticSamplers, _rootSignatureDesc.pStaticSamplers,
			sizeof(D3D12_STATIC_SAMPLER_DESC) * numStaticSamplers);
	}

	rootSignatureDesc.NumStaticSamplers = numStaticSamplers;
	rootSignatureDesc.pStaticSamplers = pStaticSamplers;

	D3D12_ROOT_SIGNATURE_FLAGS flags = _rootSignatureDesc.Flags;
	rootSignatureDesc.Flags = flags;

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC versionRootSignatureDesc;
	versionRootSignatureDesc.Init_1_1(numParameters, pParameters, numStaticSamplers, pStaticSamplers, flags);

	// Serialize the root signature.
	ComPtr<ID3DBlob> rootSignatureBlob;
	ComPtr<ID3DBlob> errorBlob;
	DX_CHECK(D3DX12SerializeVersionedRootSignature(&versionRootSignatureDesc,
		rootSignatureVersion, &rootSignatureBlob, &errorBlob));

	// Create the root signature.
	DX_CHECK(d3d12Device->GetD3D12Device()->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
		rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));
}

effUINT32 EFFD3D12RootSignature::GetDescriptorTableBitMask(D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType) const
{
	uint32_t descriptorTableBitMask = 0;
	switch (descriptorHeapType)
	{
	case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		descriptorTableBitMask = descriptorTableBitMask;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		descriptorTableBitMask = samplerTableBitMask;
		break;
	}

	return descriptorTableBitMask;
}

effUINT32 EFFD3D12RootSignature::GetNumDescriptors(effUINT32 rootIndex) const
{
	assert(rootIndex < 32);
	return numDescriptorsPerTable[rootIndex];
}
