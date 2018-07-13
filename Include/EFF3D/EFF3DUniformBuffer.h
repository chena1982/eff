/******************************************************************************
	created:	2018-07-14   0:30
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DUniformBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DUniformBuffer_2018_07_14__
#define __EFF3DUniformBuffer_2018_07_14__


#include "EFF3DResource.h"
#include "EFF3DResourceManager.h"

EFF3D_BEGIN

class EFF3DUniformBuffer : public EFF3DResource
{
public:
    EFF3DUniformBuffer();

    virtual ~EFF3DUniformBuffer();

public:
    virtual effBOOL	LoadDataFromFile(const effString & filePath);
    virtual effVOID CreateFromMemory(effUINT size, effVOID * data, effUINT flag);
    virtual effVOID Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard = effFALSE);
public:
    effUINT flags;
    effUINT64 usedFlags[2];
    effBOOL dirty;
};

template<typename UniformBufferType>
class EFF3DTUniformBuffer : public EFF3DUniformBuffer
{
public:
    EFF3DTUniformBuffer()
    {
        data = (effBYTE *)(effVOID *)&uniformBuffer;
        dataSize = sizeof(UniformBufferType);
    }

	virtual ~EFF3DTUniformBuffer() {}
public:




public:

    UniformBufferType uniformBuffer;
};


class EFF3DUniformBufferManager : public EFF3DResourceManager
{
public:
    EFF3DUniformBufferManager();
    virtual ~EFF3DUniformBufferManager();

    virtual effBOOL CreateFromFileImpl(const effString & strFilePath, EFF3DResource * resource, EFF3DResourceType resourceType);
};

EFF3D_END

#endif