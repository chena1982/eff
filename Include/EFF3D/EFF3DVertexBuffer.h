/******************************************************************************
	created:	2008-12-13   0:04
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DVertexBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DVertexBuffer_2008_12_13__
#define __EFF3DVertexBuffer_2008_12_13__

#include "EFF3DResource.h"
#include "EFF3DResourceManager.h"

EFF3D_BEGIN

class EFF3D_API EFF3DVertexBuffer : public EFF3DResource
{
	RTTI_DECLARE_PURE(EFF3DVertexBuffer, EFF3DResource)
public:
	EFF3DVertexBuffer() {}
	virtual ~EFF3DVertexBuffer() {}
public:
    virtual effBOOL					LoadDataFromFile(const effString & filePath);

    virtual effVOID                 CreateFromMemory(effUINT size, effVOID * data, effUINT flag) = 0;
    virtual effVOID                 Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard = effFALSE) = 0;



};

class EFF3D_API EFF3DVertexBufferManager : public EFF3DResourceManager
{
    friend class EFF3DDevice;
protected:
    EFF3DVertexBufferManager();
    virtual ~EFF3DVertexBufferManager();

public:
    virtual effBOOL CreateFromFileImpl(const effString & filePath, EFF3DResource * resource, EFF3DResourceType resourceType);
};

EFF3D_END

#endif