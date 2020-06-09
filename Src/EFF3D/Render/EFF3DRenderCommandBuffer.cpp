/******************************************************************************
created:	2018-03-06   23:33
file path:	d:\EFF\Src\EFF3D\EFF3DRenderCommandBuffer.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "../EFF3DPCH.h"
#include "EFF3DRenderCommandBuffer.h"
#include "EFF3DDevice.h"
#include "EFF3DVertexDeclaration.h"

EFF3D_BEGIN

EFF3DRenderCommandBuffer::EFF3DRenderCommandBuffer()
{
    device = EFF3DGetDevice();
}

EFF3DRenderCommandBuffer::~EFF3DRenderCommandBuffer()
{

}

EFFId EFF3DRenderCommandBuffer::CreateResourceFromMemory(effVOID * data, effUINT size, EFF3DResourceType resourceType)
{
    return EFFId();
}

EFF3DTextureHandle EFF3DRenderCommandBuffer::CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT flag,
    EFF3DFormat format, EFF3DResourceType resourceType)
{
    return EFF3DTextureHandle();
}



EFF3D_END