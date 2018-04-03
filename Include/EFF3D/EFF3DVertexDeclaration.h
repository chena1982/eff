/******************************************************************************
	created:	2008-12-13   0:10
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DVertexDeclaration.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DVertexDeclaration_2008_12_13__
#define __EFF3DVertexDeclaration_2008_12_13__



EFF3D_BEGIN



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
public:
	EFF3DVertexDeclaration();
	EFF3DVertexDeclaration(const EFF3DVertexDeclaration & rhs);
	virtual ~EFF3DVertexDeclaration();
public:

public:

    EFF3DVertexDeclaration & Begin();
    effVOID End();
	
    EFF3DVertexDeclaration &        AddElement(EFF3DVertexAttrib::Enum attrib, effBYTE num, EFF3DVertexType::Enum type, effBOOL normalized, effBOOL asInt);
    effVOID                         Decode(EFF3DVertexAttrib::Enum attrib, effBYTE & num, EFF3DVertexType::Enum & type, effBOOL & normalized, effBOOL & asInt) const;
    EFF3DVertexDeclaration &        Skip(effUINT16 size);

    effBOOL                         Has(EFF3DVertexAttrib::Enum attrib) const { return UINT16_MAX != attributes[attrib].valueUINT16; }
    effUINT16                       GetOffset(EFF3DVertexAttrib::Enum attrib) const { return offset[attrib]; }
    effUINT16                       GetStride() const { return stride; }
    effUINT                         GetSize(effUINT num) const { return num * stride; }

protected:
    effUINT32 hash;
    effUINT16 stride;
    effUINT16 offset[EFF3DVertexAttrib::Count];
    EFF3DVertexAttributeValue attributes[EFF3DVertexAttrib::Count];
};




EFF3D_END

#endif