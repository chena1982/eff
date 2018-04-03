/******************************************************************************
	created:	2008-12-14   22:24
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DVertexDeclaration.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DVertexDeclaration.h"

//#define new EFFNEW

EFF3D_BEGIN



static const effBYTE s_attribTypeSizeD3D9[EFF3DVertexType::Count][4] =
{
    { 4,  4,  4,  4 }, // Uint8
    { 4,  4,  4,  4 }, // Uint10
    { 4,  4,  8,  8 }, // Int16
    { 4,  4,  8,  8 }, // Half
    { 4,  8, 12, 16 }, // Float
};

static const effBYTE s_attribTypeSizeD3D1x[EFF3DVertexType::Count][4] =
{
    { 1,  2,  4,  4 }, // Uint8
    { 4,  4,  4,  4 }, // Uint10
    { 2,  4,  8,  8 }, // Int16
    { 2,  4,  8,  8 }, // Half
    { 4,  8, 12, 16 }, // Float
};

static const effBYTE s_attribTypeSizeGl[EFF3DVertexType::Count][4] =
{
    { 1,  2,  4,  4 }, // Uint8
    { 4,  4,  4,  4 }, // Uint10
    { 2,  4,  6,  8 }, // Int16
    { 2,  4,  6,  8 }, // Half
    { 4,  8, 12, 16 }, // Float
};

static const effBYTE (*s_attribTypeSize[])[EFF3DVertexType::Count][4] =
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


EFF3DVertexDeclaration::EFF3DVertexDeclaration()
{
    stride = 0;

    s_attribTypeSize[0] = s_attribTypeSize[1];
}


EFF3DVertexDeclaration::~EFF3DVertexDeclaration()
{
}

EFF3DVertexDeclaration & EFF3DVertexDeclaration::Begin()
{

    stride = 0;
    memset(attributes, 0xff, sizeof(attributes));
    memset(offset, 0, sizeof(offset));

    return *this;
}

effVOID EFF3DVertexDeclaration::End()
{
    //HashMurmur2A murmur;
    //murmur.begin();
    //murmur.add(attributes, sizeof(attributes));
    //murmur.add(offset, sizeof(offset));
    //murmur.add(stride);
    //hash = murmur.end();
}

EFF3DVertexDeclaration & EFF3DVertexDeclaration::AddElement(EFF3DVertexAttrib::Enum attrib, effBYTE num, EFF3DVertexType::Enum type, effBOOL normalized, effBOOL asInt)
{

    const effUINT16 encodeAsInt = (asInt & (!!"\x1\x1\x1\x0\x0"[type])) << 8;
    
    EFF3DVertexAttributeValue & val = attributes[attrib];

    val.valueBits.num = num;
    val.valueBits.type = type;
    val.valueBits.normalized = normalized;
    val.valueBits.asInt = encodeAsInt;


    offset[attrib] = stride;
    stride += (*s_attribTypeSize[0])[type][num - 1];

    return *this;
}

effVOID EFF3DVertexDeclaration::Decode(EFF3DVertexAttrib::Enum attrib, effBYTE & num, EFF3DVertexType::Enum & type, effBOOL & normalized, effBOOL & asInt) const
{
    const EFF3DVertexAttributeValue & val = attributes[attrib];

    num = val.valueBits.num;
    type = (EFF3DVertexType::Enum)val.valueBits.type;
    normalized = val.valueBits.normalized;
    asInt = val.valueBits.asInt;
}

EFF3DVertexDeclaration & EFF3DVertexDeclaration::Skip(effUINT16 size)
{
    stride += size;

    return *this;
}

EFF3D_END