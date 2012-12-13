/******************************************************************************
	created:	2008-12-13   0:10
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DVertexDeclaration.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DVertexDeclaration_2008_12_13__
#define __EFF3DVertexDeclaration_2008_12_13__



EFF3D_BEGIN

enum EFF3DVertexElementSemantic
{
	VES_POSITION = 0,
	VES_BLENDWEIGHT,
	VES_BLENDINDICES,
	VES_NORMAL,
	VES_PSIZE,
	VES_TEXCOORD,
	VES_TANGENT,
	VES_BINORMAL,
	VES_TESSFACTOR,
	VES_POSITIONT,
	VES_COLOR,
	VES_FOG,
	VES_DEPTH,
	VES_SAMPLE,
} ;


enum EFF3DVertexElementMethod
{
	VEM_DEFAULT = 0,
	VEM_PARTIALU,
	VEM_PARTIALV,
	VEM_CROSSUV,    // Normal
	VEM_UV,
	VEM_LOOKUP,               // Lookup a displacement map
	VEM_LOOKUPPRESAMPLED,     // Lookup a pre-sampled displacement map
};

enum EFF3DVertexElementType
{
	VET_FLOAT1    =  0,  // 1D float expanded to (value, 0., 0., 1.)
	VET_FLOAT2    =  1,  // 2D float expanded to (value, value, 0., 1.)
	VET_FLOAT3    =  2,  // 3D float expanded to (value, value, value, 1.)
	VET_FLOAT4    =  3,  // 4D float
	VET_UC3DCOLOR  =  4,  // 4D packed unsigned bytes mapped to 0. to 1. range
	// Input is in UC3DCOLOR format (ARGB) expanded to (R, G, B, A)
	VET_UBYTE4    =  5,  // 4D unsigned byte
	VET_SHORT2    =  6,  // 2D signed short expanded to (value, value, 0., 1.)
	VET_SHORT4    =  7,  // 4D signed short

	// The following types are valid only with vertex shaders >= 2.0


	VET_UBYTE4N   =  8,  // Each of 4 bytes is normalized by dividing to 255.0
	VET_SHORT2N   =  9,  // 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
	VET_SHORT4N   = 10,  // 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
	VET_USHORT2N  = 11,  // 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
	VET_USHORT4N  = 12,  // 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
	VET_UDEC3     = 13,  // 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
	VET_DEC3N     = 14,  // 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
	VET_FLOAT16_2 = 15,  // Two 16-bit floating point values, expanded to (value, value, 0, 1)
	VET_FLOAT16_4 = 16,  // Four 16-bit floating point values
	VET_UNUSED    = 17,  // When the type field in a decl is unused.
};



class EFF3D_API EFF3DVertexElement
{
public:
	EFF3DVertexElement(effWORD stream, effWORD offset, EFF3DVertexElementType theType, EFF3DVertexElementSemantic semantic, effBYTE index = 0, EFF3DVertexElementMethod method = VEM_DEFAULT);
	EFF3DVertexElement();

	effWORD											GetStream() const { return m_wStream; }
	effWORD											GetOffset() const { return m_wOffset; }
	EFF3DVertexElementType					GetType() const { return (EFF3DVertexElementType)m_Type; }
	EFF3DVertexElementSemantic			GetSemantic() const { return (EFF3DVertexElementSemantic)m_Semantic; }
	effBYTE												GetSemanticIndex() const { return m_SemanticIndex; }
	EFF3DVertexElementMethod			GetMethod() const { return (EFF3DVertexElementMethod)m_Method; }
	effUINT										GetSize() const;
	static effUINT								GetTypeSize(EFF3DVertexElementType etype);
	static effWORD									GetTypeCount(EFF3DVertexElementType etype);
	static EFF3DVertexElementType		MultiplyTypeCount(EFF3DVertexElementType baseType, effWORD count);
	static EFF3DVertexElementType		GetBaseType(EFF3DVertexElementType multiType);

	inline effBOOL operator == (const EFF3DVertexElement & rhs) const
	{
		if ( m_Type != rhs.m_Type || 
			m_SemanticIndex != rhs.m_SemanticIndex ||
			m_wOffset != rhs.m_wOffset ||
			m_Semantic != rhs.m_Semantic ||
			m_wStream != rhs.m_wStream ||
			m_Method != rhs.m_Method)
		{
			return effFALSE;
		}
		else
		{
			return effTRUE;
		}

	}

	inline effVOID BaseVertexPointerToElement(effBYTE * pBase, effVOID** pElem) const
	{
		*pElem = (effVOID *)(pBase + m_wOffset);
	}

	inline effVOID BaseVertexPointerToElement(effBYTE * pBase, effFLOAT** pElem) const
	{

		*pElem = (effFLOAT *)(pBase + m_wOffset);
	}


	inline effVOID BaseVertexPointerToElement(effBYTE * pBase, effWORD** pElem) const
	{
		*pElem = (effWORD*)(pBase + m_wOffset);
	}

	inline effVOID BaseVertexPointerToElement(effBYTE * pBase, effUINT** pElem) const
	{
		*pElem = (effUINT*)(pBase + m_wOffset);
	}

public:
	effWORD													m_wStream;     // Stream index
	effWORD													m_wOffset;     // Offset in the stream in bytes
	effBYTE														m_Type;       // Data type
	effBYTE														m_Method;     // Processing method
	effBYTE														m_Semantic;      // Semantics
	effBYTE														m_SemanticIndex; // Semantic index
};

class EFF3D_API EFF3DVertexDeclaration
{
public:
	EFF3DVertexDeclaration();
	EFF3DVertexDeclaration(const EFF3DVertexDeclaration & rhs);
	virtual ~EFF3DVertexDeclaration();
public:
	typedef std::vector<EFF3DVertexElement>		VertexElementArray;
public:


	effUINT												GetElementCount() { return m_aryElement.size(); }
	const VertexElementArray &						GetElements() const;
	const EFF3DVertexElement *						GetElement(effWORD index);
	effWORD													GetMaxSource() const;
	effUINT												GetVertexSize(effWORD stream);
	effUINT												GetFVF();

	virtual const EFF3DVertexElement &			AddElement(effWORD source,effWORD offset,EFF3DVertexElementType theType,EFF3DVertexElementSemantic semantic,effBYTE index = 0,EFF3DVertexElementMethod method = VEM_DEFAULT);
	virtual const EFF3DVertexElement &			InsertElement(effWORD atPosition,effWORD source,effWORD offset,EFF3DVertexElementType theType,EFF3DVertexElementSemantic semantic,effBYTE index = 0);
	virtual effVOID											RemoveElement(effWORD elemIndex);
	virtual effVOID											ModifyElement(effWORD elemIndex,effWORD source,effWORD offset,EFF3DVertexElementType theType,EFF3DVertexElementSemantic semantic,effBYTE index = 0);



	inline effBOOL operator == (const EFF3DVertexDeclaration & rhs) const
	{
		if ( m_aryElement.size() != rhs.m_aryElement.size() )
		{
			return effFALSE;
		}


		for ( effUINT i = 0; i < m_aryElement.size(); i++ )
		{
			if ( !(m_aryElement[i] == rhs.m_aryElement[i]) )
			{
				return effFALSE;
			}
		}

		return effTRUE;
	}

	inline effBOOL operator != (const EFF3DVertexDeclaration & rhs) const
	{
		return !(*this == rhs);
	}
protected:
	VertexElementArray							m_aryElement;
	effBOOL											m_bNeedRebuild;
};


effUINT CalculateVertexSize(std::vector<EFF3DVertexElement> & aryElement,effWORD stream);


EFF3D_END

#endif