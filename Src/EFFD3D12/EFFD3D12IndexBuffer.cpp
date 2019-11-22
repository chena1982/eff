/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\Src\EFFD3D12\EFFD3D12IndexBuffer.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D12PCH.h"
#include "EFFD3D12IndexBuffer.h"
#include "EFFD3D12Device.h"
#include "EFFD3D12DeviceCommandQueue.h"
#include "EFFD3D12DeviceCommandList.h"
#include "EFFD3D12ResourceStateManager.h"

//#define new EFFNEW

EFFD3D12IndexBuffer::EFFD3D12IndexBuffer()
{
    //d3d9IndexBuffer = NULL;
}

EFFD3D12IndexBuffer::~EFFD3D12IndexBuffer()
{
	//SF_RELEASE(d3d9IndexBuffer);
}


effVOID EFFD3D12IndexBuffer::CreateFromMemory(effUINT size, effVOID * data, effUINT flag)
{

}

effVOID EFFD3D12IndexBuffer::Update(effUINT32 offset, effUINT32 size, effVOID * data, effBOOL discard/* = effFALSE*/)
{
	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();

	d3d12Device->UpdateBuffer(d3d12Resource, offset, size, data, D3D12_RESOURCE_FLAG_NONE);

	CreateViews(size / 2, 2);
}


effVOID EFFD3D12IndexBuffer::CreateViews(effUINT numElements, effUINT elementSize)
{
	assert(elementSize == 2 || elementSize == 4 && "Indices must be 16, or 32-bit integers.");

	count = numElements;
	format = (elementSize == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

	indexBufferView.BufferLocation = d3d12Resource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = static_cast<effUINT>(numElements * elementSize);
	indexBufferView.Format = (DXGI_FORMAT)format;
}