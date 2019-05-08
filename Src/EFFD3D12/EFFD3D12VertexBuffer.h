/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\EFFEngine\Src\EFFD3D12\EFFD3D12VertexBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D12VertexBuffer_2019_03_23__
#define __EFFD3D12VertexBuffer_2019_03_23__


class EFFD3D12VertexBuffer : public EFF3DVertexBuffer
{
	friend class EFFD3D12Device;
	//RTTI_DECLARE(EFFD3D9VertexBuffer, EFF3DVertexBuffer)
public:
	EFFD3D12VertexBuffer();
	virtual ~EFFD3D12VertexBuffer();
public:
    virtual effVOID                 CreateFromMemory(effUINT size, effVOID * data, effUINT flag);
    virtual effVOID                 Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard = effFALSE);
protected:
	//LPDIRECT3DVERTEXBUFFER9		    d3d9VertexBuffer;

    effBYTE * data;
    effUINT size;
    EFF3DVertexDeclarationHandle vertexDeclHandle;
};

#endif