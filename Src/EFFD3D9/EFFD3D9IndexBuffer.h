/******************************************************************************
	created:	2008-12-14   21:55
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9IndexBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9IndexBuffer_2008_12_14__
#define __EFFD3D9IndexBuffer_2008_12_14__


class EFFD3D9IndexBuffer : public EFF3DIndexBuffer
{
	friend class EFFD3D9Device;
public:
	EFFD3D9IndexBuffer();
	virtual ~EFFD3D9IndexBuffer();
public:
    virtual effVOID             CreateFromMemory(effUINT size, effVOID * data, effUINT flag);
    virtual effVOID             Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard = effFALSE);
protected:
	LPDIRECT3DINDEXBUFFER9		d3d9IndexBuffer;
};

#endif