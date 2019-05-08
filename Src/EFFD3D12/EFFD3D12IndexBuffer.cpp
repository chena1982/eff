/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\Src\EFFD3D12\EFFD3D12IndexBuffer.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D12PCH.h"
#include "EFFD3D12IndexBuffer.h"

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

effVOID EFFD3D12IndexBuffer::Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard/* = effFALSE*/)
{
	/*if (d3d9IndexBuffer != NULL)
	{
		effVOID * address = NULL;
		DX_CHECK(d3d9IndexBuffer->Lock(offset, size, &address, discard ? D3DLOCK_DISCARD : 0));

		memcpy(address, data, size);

		DX_CHECK(d3d9IndexBuffer->Unlock());
	}*/
}
