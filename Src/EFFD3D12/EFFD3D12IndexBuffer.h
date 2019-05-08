/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\EFFEngine\Src\EFFD3D12\EFFD3D12IndexBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D12IndexBuffer_2019_03_23__
#define __EFFD3D12IndexBuffer_2019_03_23__


class EFFD3D12IndexBuffer : public EFF3DIndexBuffer
{
	friend class EFFD3D12Device;
public:
	EFFD3D12IndexBuffer();
	virtual ~EFFD3D12IndexBuffer();
public:
    virtual effVOID             CreateFromMemory(effUINT size, effVOID * data, effUINT flag);
    virtual effVOID             Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard = effFALSE);
protected:
	//LPDIRECT3DINDEXBUFFER9		d3d9IndexBuffer;
};

#endif