/******************************************************************************
	created:	2008-12-14   21:55
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9IndexBuffer.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D9PCH.h"
#include "EFFD3D9IndexBuffer.h"

//#define new EFFNEW

EFFD3D9IndexBuffer::EFFD3D9IndexBuffer()
{
    d3d9IndexBuffer = NULL;
}

EFFD3D9IndexBuffer::~EFFD3D9IndexBuffer()
{
	SF_RELEASE(d3d9IndexBuffer);
}


effVOID EFFD3D9IndexBuffer::CreateFromMemory(effUINT size, effVOID * data, effUINT flag)
{

}

effVOID EFFD3D9IndexBuffer::Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard/* = effFALSE*/)
{
	if (d3d9IndexBuffer != NULL)
	{
		effVOID * address = NULL;
		DX_CHECK(d3d9IndexBuffer->Lock(offset, size, &address, discard ? D3DLOCK_DISCARD : 0));

		memcpy(address, data, size);

		DX_CHECK(d3d9IndexBuffer->Unlock());
	}
}
