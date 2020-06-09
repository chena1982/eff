/******************************************************************************
created:	2018-07-14   0:31
file path:	d:\EFF\Src\EFF3D\EFF3DUniformBuffer.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "../EFF3DPCH.h"
#include "EFF3DUniformBuffer.h"

//#define new EFFNEW

EFF3D_BEGIN


EFF3DUniformBuffer::EFF3DUniformBuffer()
{
    flags = 0;
    usedFlags[0] = 0;
    usedFlags[1] = 0;
    dirty = effFALSE;
}


EFF3DUniformBuffer::~EFF3DUniformBuffer()
{

}

effBOOL	EFF3DUniformBuffer::LoadDataFromFile(const effString & filePath)
{
    return effTRUE;
}

effVOID EFF3DUniformBuffer::CreateFromMemory(effUINT size, effVOID * data, effUINT flag)
{

}

effVOID EFF3DUniformBuffer::Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard/* = effFALSE*/)
{

}

EFF3DUniformBufferManager::EFF3DUniformBufferManager()
{

}

EFF3DUniformBufferManager::~EFF3DUniformBufferManager()
{

}

effBOOL EFF3DUniformBufferManager::CreateFromFileImpl(const effString & strFilePath, EFF3DResource * resource, EFF3DResourceType resourceType)
{
    return effTRUE;
}

EFF3D_END