/******************************************************************************
	created:	2008-12-14   21:59
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9VertexBuffer.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D9PCH.h"
#include "EFFD3D9VertexBuffer.h"

//#define new EFFNEW
//RTTI_IMPLEMENT(EFFD3D9VertexBuffer, 0)

EFFD3D9VertexBuffer::EFFD3D9VertexBuffer()
{
    d3d9VertexBuffer = NULL;
}

EFFD3D9VertexBuffer::~EFFD3D9VertexBuffer()
{
	SF_RELEASE(d3d9VertexBuffer);
}

effVOID EFFD3D9VertexBuffer::CreateFromMemory(effUINT size, effVOID * data, effUINT flag)
{

}

effVOID EFFD3D9VertexBuffer::Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard/* = effFALSE*/)
{
	if (d3d9VertexBuffer != NULL)
	{
		effVOID * address = NULL;
		DX_CHECK(d3d9VertexBuffer->Lock(offset, size, &address, discard ? D3DLOCK_DISCARD : 0));

		memcpy(address, data, size);

		DX_CHECK(d3d9VertexBuffer->Unlock());
	}
}