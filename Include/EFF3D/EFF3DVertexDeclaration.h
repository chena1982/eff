/******************************************************************************
	created:	2008-12-13   0:10
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DVertexDeclaration.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DVertexDeclaration_2008_12_13__
#define __EFF3DVertexDeclaration_2008_12_13__

#include "EFF3DResource.h"
#include "EFF3DResourceManager.h"

EFF3D_BEGIN

class EFF3DRenderQueue;

union EFF3DVertexAttributeValue
{
    struct OrderBitsDefine
    {
        effUINT16 normalized : 1,
            type : 3,
            num : 2,
            asInt : 1,
            padding : 9;
    };
    OrderBitsDefine valueBits;
    effUINT16 valueUINT16;
};


class EFF3D_API EFF3DVertexDeclaration
{
    friend class EFF3DVertexDeclarationManager;
public:
	EFF3DVertexDeclaration();
	EFF3DVertexDeclaration(const EFF3DVertexDeclaration & rhs);
	virtual ~EFF3DVertexDeclaration();
public:

public:

    EFF3DVertexDeclaration & Begin();
    effVOID End();
	
    EFF3DVertexDeclaration &        AddElement(EFF3DVertexAttrib::Enum attrib, effBYTE num, EFF3DVertexAttribType::Enum type, effBOOL normalized = effFALSE, effBOOL asInt = effFALSE);
    effVOID                         Decode(EFF3DVertexAttrib::Enum attrib, effBYTE & num, EFF3DVertexAttribType::Enum & type, effBOOL & normalized, effBOOL & asInt) const;
    EFF3DVertexDeclaration &        Skip(effUINT16 size);

    effBOOL                         Has(EFF3DVertexAttrib::Enum attrib) const { return UINT16_MAX != attributes[attrib].valueUINT16; }
    effUINT16                       GetOffset(EFF3DVertexAttrib::Enum attrib) const { return offset[attrib]; }
    effUINT16                       GetStride() const { return stride; }
    effUINT                         GetSize(effUINT num) const { return num * stride; }

public:
    effUINT hash;
    effUINT16 stride;
    effUINT16 offset[EFF3DVertexAttrib::Count];
    EFF3DVertexAttributeValue attributes[EFF3DVertexAttrib::Count];
};


class EFF3D_API EFF3DVertexDeclarationManager
{

public:
    EFF3DVertexDeclarationManager();
    ~EFF3DVertexDeclarationManager();

public:
    EFF3DVertexDeclarationHandle AddVertexDeclaration(const EFF3DVertexDeclaration & vertexDecl, EFF3DRenderQueue * rendererQueue);

    //const EFF3DVertexDeclaration * GetVertexDeclaration(effUINT hash) const;

    const EFF3DVertexDeclaration * GetVertexDeclaration(EFF3DVertexDeclarationHandle vertexDecalHandle);
public:
    MAP<effUINT, EFF3DVertexDeclarationHandle> vertexDeclsByHash;
	EFFIdManager idManager;
	EFFFastIdMap<EFF3DVertexDeclaration> vertexDecls;
};

struct QuadVertex
{
    effFLOAT			x, y, z, rhw;
    effFLOAT			u, v;
    //const static effUINT fvf = EFF3DFVF_XYZRHW | EFF3DFVF_TEX1;


    static effVOID InitVertexDecl()
    {
        vertexDecl.Begin()
            .AddElement(EFF3DVertexAttrib::Position, 3, EFF3DVertexAttribType::Float)
            .AddElement(EFF3DVertexAttrib::TexCoord0, 2, EFF3DVertexAttribType::Float)
            .End();
    }

    static EFF3DVertexDeclaration vertexDecl;
};

struct QuadColoredVertex
{
    effFLOAT			x, y, z, rhw;
    effUINT				color;

    //const static effUINT fvf = EFF3DFVF_XYZRHW | EFF3DFVF_DIFFUSE;
};


EFF3D_END

#endif