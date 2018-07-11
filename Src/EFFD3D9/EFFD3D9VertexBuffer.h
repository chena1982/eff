/******************************************************************************
	created:	2008-12-14   21:55
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9VertexBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9VertexBuffer_2008_12_14__
#define __EFFD3D9VertexBuffer_2008_12_14__


class EFFD3D9VertexBuffer : public EFF3DVertexBuffer
{
	friend class EFFD3D9Device;
	//RTTI_DECLARE(EFFD3D9VertexBuffer, EFF3DVertexBuffer)
public:
	EFFD3D9VertexBuffer();
	virtual ~EFFD3D9VertexBuffer();
public:
    virtual effVOID                 CreateFromMemory(effUINT size, effVOID * data, effUINT flag) = 0;
    virtual effVOID                 Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard = effFALSE) = 0;
protected:
	LPDIRECT3DVERTEXBUFFER9		    d3d9VertexBuffer;

    effBYTE * data;
    effUINT size;
    EFF3DVertexDeclarationHandle vertexDeclHandle;
};

#endif