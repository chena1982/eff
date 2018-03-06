/******************************************************************************
created:	2018-03-05   0:21
file path:	d:\EFF\Include\EFF3D\EFF3DRenderBaseDefines.cpp
author:		ChenA

purpose:
******************************************************************************/


#include "EFF3DPCH.h"
#include "EFF3DRenderBaseDefines.h"

EFF3D_BEGIN


static const effBYTE s_attribTypeSizeD3D9[EFF3DVertexAttribTypeCount][4] =
{
    { 4,  4,  4,  4 }, // Uint8
    { 4,  4,  4,  4 }, // Uint10
    { 4,  4,  8,  8 }, // Int16
    { 4,  4,  8,  8 }, // Half
    { 4,  8, 12, 16 }, // Float
};

static const effBYTE s_attribTypeSizeD3D1x[EFF3DVertexAttribTypeCount][4] =
{
    { 1,  2,  4,  4 }, // Uint8
    { 4,  4,  4,  4 }, // Uint10
    { 2,  4,  8,  8 }, // Int16
    { 2,  4,  8,  8 }, // Half
    { 4,  8, 12, 16 }, // Float
};

static const effBYTE s_attribTypeSizeGl[EFF3DVertexAttribTypeCount][4] =
{
    { 1,  2,  4,  4 }, // Uint8
    { 4,  4,  4,  4 }, // Uint10
    { 2,  4,  6,  8 }, // Int16
    { 2,  4,  6,  8 }, // Half
    { 4,  8, 12, 16 }, // Float
};

static const effBYTE(*s_attribTypeSize[])[EFF3DVertexAttribTypeCount][4] =
{
    &s_attribTypeSizeD3D9,  // Noop
    &s_attribTypeSizeD3D9,  // Direct3D9
    &s_attribTypeSizeD3D1x, // Direct3D11
    &s_attribTypeSizeD3D1x, // Direct3D12
    &s_attribTypeSizeD3D1x, // Gnm
    &s_attribTypeSizeGl,    // Metal
    &s_attribTypeSizeGl,    // OpenGLES
    &s_attribTypeSizeGl,    // OpenGL
    &s_attribTypeSizeD3D1x, // Vulkan
    &s_attribTypeSizeD3D9,  // Count
};

EFF3DVertexDecl::EFF3DVertexDecl()
{
    stride = 0;
}

EFF3DVertexDecl & EFF3DVertexDecl::Begin()
{
    hash = 0;
    stride = 0;

    memset(attributes, 0xff, sizeof(attributes));
    memset(offset, 0, sizeof(offset));

    return *this;
}

effVOID EFF3DVertexDecl::End()
{

}

EFF3DVertexDecl & EFF3DVertexDecl::Add(EFF3DVertexAttrib attrib, effUINT num, EFF3DVertexAttribType type, effBOOL normalized, effBOOL asInt)
{
    EFF3DVertexAttributeValue value;
    value.valueBits.normalized = normalized;
    value.valueBits.type = type;
    value.valueBits.num = num;
    value.valueBits.asInt = (asInt & (!!"\x1\x1\x1\x0\x0"[type]));

    attributes[attrib] = value.valueUINT16;

    offset[attrib] = stride;

    effUINT deviceType = 0;

    stride += (*s_attribTypeSize[deviceType])[type][num - 1];

    return *this;
}

EFF3DVertexDecl & EFF3DVertexDecl::Skip(effUINT num)
{
    stride += num;

    return *this;
}

effVOID EFF3DVertexDecl::Decode(EFF3DVertexAttrib attrib, effUINT & num, EFF3DVertexAttribType & type, effBOOL & normalized, effBOOL & asInt) const
{
    EFF3DVertexAttributeValue value;
    value.valueUINT16 = attributes[attrib];

    num = value.valueBits.num;
    type = (EFF3DVertexAttribType)value.valueBits.type;
    normalized = value.valueBits.normalized;
    asInt = value.valueBits.asInt;
}

EFF3D_END