/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\Src\EFFD3D12\EFFD3D12VertexBuffer.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D12PCH.h"
#include "EFFD3D12VertexBuffer.h"

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
	/*if (d3d9VertexBuffer != NULL)
	{
		effVOID * address = NULL;
		DX_CHECK(d3d9VertexBuffer->Lock(offset, size, &address, discard ? D3DLOCK_DISCARD : 0));

		memcpy(address, data, size);

		DX_CHECK(d3d9VertexBuffer->Unlock());
	}*/
}