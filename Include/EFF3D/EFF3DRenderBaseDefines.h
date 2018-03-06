/******************************************************************************
created:	2018-03-05   0:21
file path:	d:\EFF\Include\EFF3D\EFF3DRenderBaseDefines.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFF3DRenderBaseDefines_2018_03_05__
#define __EFF3DRenderBaseDefines_2018_03_05__


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

struct EFF3DVertexDecl
{
    EFF3DVertexDecl();


    EFF3DVertexDecl & Begin();


    effVOID End();



    EFF3DVertexDecl & Add(EFF3DVertexAttrib attrib, effUINT num, EFF3DVertexAttribType type, effBOOL normalized = effFALSE, effBOOL asInt = effFALSE);


    EFF3DVertexDecl & Skip(effUINT num);


    effVOID Decode(EFF3DVertexAttrib attrib, effUINT & num, EFF3DVertexAttribType & type, effBOOL & normalized, effBOOL & asInt) const;

    effBOOL Has(EFF3DVertexAttrib attrib) const { return UINT16_MAX != attributes[attrib]; }

    effUINT16 GetOffset(EFF3DVertexAttrib attrib) const { return offset[attrib]; }

    effUINT16 GetStride() const { return stride; }

    effUINT GetSize(effUINT num) const { return num * stride; }

    effUINT hash;
    effUINT16 stride;
    effUINT16 offset[EFF3DVertexAttribCount];
    effUINT16 attributes[EFF3DVertexAttribCount];
};




EFF3D_END


#endif
