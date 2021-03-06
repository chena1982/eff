/******************************************************************************
	created:	2008-12-13   0:07
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DIndexBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DIndexBuffer_2008_12_13__
#define __EFF3DIndexBuffer_2008_12_13__


#include "EFF3DResource.h"
#include "EFF3DResourceManager.h"

EFF3D_BEGIN

class EFF3D_API EFF3DIndexBuffer : public EFF3DResource
{
public:
	EFF3DIndexBuffer();
	virtual ~EFF3DIndexBuffer() {}
public:
    virtual effBOOL					LoadDataFromFile(const effString & filePath);

    virtual effVOID                 CreateFromMemory(effUINT size, effVOID * data, effUINT flags) = 0;
    virtual effVOID                 Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard = effFALSE) = 0;


protected:
    effUINT32 count;
    effUINT32 format;
	effUINT32 flags;

};

class EFF3D_API EFF3DIndexBufferManager : public EFF3DResourceManager
{
    friend class EFF3DDevice;
protected:
    EFF3DIndexBufferManager();
    virtual ~EFF3DIndexBufferManager();

public:
    virtual effBOOL CreateFromFileImpl(const effString & filePath, EFF3DResource * resource, EFF3DResourceType resourceType);
};


EFF3D_END

#endif