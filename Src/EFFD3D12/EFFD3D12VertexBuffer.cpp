/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\Src\EFFD3D12\EFFD3D12VertexBuffer.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D12PCH.h"
#include "EFFD3D12VertexBuffer.h"
#include "EFFD3D12Device.h"

//#define new EFFNEW
//RTTI_IMPLEMENT(EFFD3D9VertexBuffer, 0)

EFFD3D12VertexBuffer::EFFD3D12VertexBuffer()
{
    //d3d9VertexBuffer = NULL;
}

EFFD3D12VertexBuffer::~EFFD3D12VertexBuffer()
{
	//SF_RELEASE(d3d9VertexBuffer);
}

effVOID EFFD3D12VertexBuffer::CreateFromMemory(effUINT size, effVOID * data, effUINT flag)
{

}

effVOID EFFD3D12VertexBuffer::Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard/* = effFALSE*/)
{
	EFFD3D12Device * d3d12Device = (EFFD3D12Device *)EFF3DGetDevice();

	d3d12Device->UpdateBuffer(d3d12Resource, offset, size, data, D3D12_RESOURCE_FLAG_NONE);


	effUINT32 elementSize = d3d12Device->GetVertexDeclarationManager()->GetVertexDeclaration(vertexDeclHandle)->GetStride();

	CreateViews(size / elementSize, elementSize);
}

effVOID EFFD3D12VertexBuffer::CreateViews(effUINT numElements, effUINT elementSize)
{
	numVertices = numElements;
	vertexStride = elementSize;

	vertexBufferView.BufferLocation = d3d12Resource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = static_cast<effUINT32>(numVertices * vertexStride);
	vertexBufferView.StrideInBytes = static_cast<effUINT32>(vertexStride);
}