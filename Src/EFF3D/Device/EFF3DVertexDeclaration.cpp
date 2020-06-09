/******************************************************************************
	created:	2008-12-14   22:24
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DVertexDeclaration.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "../EFF3DPCH.h"
#include "EFF3DVertexDeclaration.h"
#include "EFF3DRenderQueue.h"
#include "murmur3/murmur3.h"

//#define new EFFNEW

EFF3D_BEGIN

EFF3DVertexDeclaration QuadVertex::vertexDecl;

static const effBYTE s_attribTypeSizeD3D9[EFF3DVertexAttribType::Count][4] =
{
    { 4,  4,  4,  4 }, // Uint8
    { 4,  4,  4,  4 }, // Uint10
    { 4,  4,  8,  8 }, // Int16
    { 4,  4,  8,  8 }, // Half
    { 4,  8, 12, 16 }, // Float
};

static const effBYTE s_attribTypeSizeD3D1x[EFF3DVertexAttribType::Count][4] =
{
    { 1,  2,  4,  4 }, // Uint8
    { 4,  4,  4,  4 }, // Uint10
    { 2,  4,  8,  8 }, // Int16
    { 2,  4,  8,  8 }, // Half
    { 4,  8, 12, 16 }, // Float
};

static const effBYTE s_attribTypeSizeGl[EFF3DVertexAttribType::Count][4] =
{
    { 1,  2,  4,  4 }, // Uint8
    { 4,  4,  4,  4 }, // Uint10
    { 2,  4,  6,  8 }, // Int16
    { 2,  4,  6,  8 }, // Half
    { 4,  8, 12, 16 }, // Float
};

static const effBYTE (*s_attribTypeSize[])[EFF3DVertexAttribType::Count][4] =
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

    //s_attribTypeSize[0] = s_attribTypeSize[1];
}

EFF3DVertexDeclaration::EFF3DVertexDeclaration(const EFF3DVertexDeclaration & rhs)
{
    hash = rhs.hash;
    stride = rhs.stride;

    for (effUINT i = 0; i < EFF3DVertexAttrib::Count; i++)
    {
        offset[i] = rhs.offset[i];
        attributes[i].valueUINT16 = rhs.attributes[i].valueUINT16;
    }

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
	MurmurHash3_x86_32(&stride, sizeof(EFF3DVertexDeclaration) - sizeof(effUINT), 9582, &hash);
}

EFF3DVertexDeclaration & EFF3DVertexDeclaration::AddElement(EFF3DVertexAttrib::Enum attrib, effBYTE num, EFF3DVertexAttribType::Enum type, effBOOL normalized, effBOOL asInt)
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

effVOID EFF3DVertexDeclaration::Decode(EFF3DVertexAttrib::Enum attrib, effBYTE & num, EFF3DVertexAttribType::Enum & type, effBOOL & normalized, effBOOL & asInt) const
{
    const EFF3DVertexAttributeValue & val = attributes[attrib];

    num = val.valueBits.num;
    type = (EFF3DVertexAttribType::Enum)val.valueBits.type;
    normalized = val.valueBits.normalized;
    asInt = val.valueBits.asInt;
}

EFF3DVertexDeclaration & EFF3DVertexDeclaration::Skip(effUINT16 size)
{
    stride += size;

    return *this;
}


EFF3DVertexDeclarationManager::EFF3DVertexDeclarationManager()
{

}

EFF3DVertexDeclarationManager::~EFF3DVertexDeclarationManager()
{

}

EFF3DVertexDeclarationHandle EFF3DVertexDeclarationManager::AddVertexDeclaration(const EFF3DVertexDeclaration & vertexDecl, EFF3DRenderQueue * renderQueue)
{
    auto it = vertexDeclsByHash.find(vertexDecl.hash);
    if (it != vertexDeclsByHash.end())
    {
        return it->second;
    }

	EFF3DVertexDeclarationHandle vbDeclHandle = idManager.Create();
	vertexDecls.Add(const_cast<EFF3DVertexDeclaration *>(&vertexDecl), vbDeclHandle.Index());
	vertexDeclsByHash[vertexDecl.hash] = vbDeclHandle;

	EFF3DCreateVertexDeclarationCommand * createVBDeclarationCommand = renderQueue->GetCommand<EFF3DCreateVertexDeclarationCommand>();
	createVBDeclarationCommand->vbDeclHandle = vbDeclHandle;
	createVBDeclarationCommand->vbDecl = &vertexDecl;

    return createVBDeclarationCommand->vbDeclHandle;
}


const EFF3DVertexDeclaration * EFF3DVertexDeclarationManager::GetVertexDeclaration(EFF3DVertexDeclarationHandle vertexDecalHandle)
{
    return vertexDecls[vertexDecalHandle.Index()];
}

EFF3D_END