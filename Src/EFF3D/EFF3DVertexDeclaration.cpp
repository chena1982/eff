/******************************************************************************
	created:	2008-12-14   22:24
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DVertexDeclaration.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DVertexDeclaration.h"

//#define new EFFNEW

EFF3D_BEGIN


EFF3DVertexElement::EFF3DVertexElement(effWORD stream,effWORD offset,EFF3DVertexElementType theType,EFF3DVertexElementSemantic semantic,effBYTE index /* = 0 */,EFF3DVertexElementMethod method /* = VEM_DEFAULT */)
: m_wStream(stream),m_wOffset(offset),m_Type(theType),m_Semantic(semantic),m_SemanticIndex(index),m_Method(method)
{
}

EFF3DVertexElement::EFF3DVertexElement()
{
}

effUINT EFF3DVertexElement::GetSize() const
{
	return GetTypeSize((EFF3DVertexElementType)m_Type);
}

effUINT EFF3DVertexElement::GetTypeSize(EFF3DVertexElementType etype)
{
	switch(etype)
	{
	case VET_FLOAT1:
		return sizeof(effFLOAT);
	case VET_FLOAT2:
		return sizeof(effFLOAT)*2;
	case VET_FLOAT3:
		return sizeof(effFLOAT)*3;
	case VET_FLOAT4:
		return sizeof(effFLOAT)*4;
	case VET_UC3DCOLOR:
		return sizeof(effUINT);
	case VET_UBYTE4:
		return sizeof(effBYTE)*4;
	case VET_SHORT2:
		return sizeof(effSHORT)*2;
	case VET_SHORT4:
		return sizeof(effSHORT)*4;
	case VET_UBYTE4N:
		return sizeof(effBYTE)*4;
	case VET_SHORT2N:
		return sizeof(effSHORT)*2;
	case VET_SHORT4N:
		return sizeof(effUSHORT)*4;
	case VET_USHORT2N:
		return sizeof(effUSHORT)*2;
	case VET_USHORT4N:
		return sizeof(effUSHORT)*4;
	case VET_UDEC3:
		return sizeof(effINT)*3;
	case VET_DEC3N:
		return sizeof(effINT)*3;
	case VET_FLOAT16_2:
		return sizeof(effFLOAT);
	case VET_FLOAT16_4:
		return sizeof(effFLOAT)*2;
	}
	return 0;
}

effWORD EFF3DVertexElement::GetTypeCount(EFF3DVertexElementType etype)
{
	switch (etype)
	{
	case VET_FLOAT1:
		return 1;
	case VET_FLOAT2:
		return 2;
	case VET_FLOAT3:
		return 3;
	case VET_FLOAT4:
		return 4;
	case VET_UC3DCOLOR:
		return 1;
	case VET_UBYTE4:
		return 4;
	case VET_SHORT2:
		return 2;
	case VET_SHORT4:
		return 4;
	case VET_UBYTE4N:
		return 4;
	case VET_SHORT2N:
		return 2;
	case VET_SHORT4N:
		return 4;
	case VET_USHORT2N:
		return 2;
	case VET_USHORT4N:
		return 4;
	case VET_UDEC3:
		return 3;
	case VET_DEC3N:
		return 3;
	case VET_FLOAT16_2:
		return 2;
	case VET_FLOAT16_4:
		return 4;
	default:
		break;
	}
	return 0;
}

EFF3DVertexElementType EFF3DVertexElement::MultiplyTypeCount(EFF3DVertexElementType baseType, effWORD count)
{
	switch (baseType)
	{
	case VET_FLOAT1:
		switch(count)
		{
		case 1:
			return VET_FLOAT1;
		case 2:
			return VET_FLOAT2;
		case 3:
			return VET_FLOAT3;
		case 4:
			return VET_FLOAT4;
		default:
			break;
		}
		break;
	case VET_SHORT2:
		switch(count)
		{
		case 1:
			return VET_SHORT2;
		case 2:
			return VET_SHORT4;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return VET_UNUSED;
}

EFF3DVertexElementType EFF3DVertexElement::GetBaseType(EFF3DVertexElementType multiType)
{
	switch (multiType)
	{
	case VET_FLOAT1:
	case VET_FLOAT2:
	case VET_FLOAT3:
	case VET_FLOAT4:
		return VET_FLOAT1;
	case VET_UC3DCOLOR:
		return VET_UC3DCOLOR;
	case VET_SHORT2:
	case VET_SHORT4:
		return VET_SHORT2;
	case VET_UBYTE4:
		return VET_UBYTE4;
	};
	return VET_FLOAT1;
}




EFF3DVertexDeclaration::EFF3DVertexDeclaration()
{
	m_bNeedRebuild = effTRUE;
}

EFF3DVertexDeclaration::EFF3DVertexDeclaration(const EFF3DVertexDeclaration & rhs)
{
	m_aryElement = rhs.m_aryElement;
}

EFF3DVertexDeclaration::~EFF3DVertexDeclaration()
{
}

const EFF3DVertexDeclaration::VertexElementArray & EFF3DVertexDeclaration::GetElements() const
{
	return m_aryElement;
}


const EFF3DVertexElement & EFF3DVertexDeclaration::AddElement(effWORD source,effWORD offset,EFF3DVertexElementType theType,EFF3DVertexElementSemantic semantic,effBYTE index /* = 0 */,EFF3DVertexElementMethod method /* = VEM_DEFAULT */)
{
	m_bNeedRebuild = effTRUE;

	m_aryElement.push_back(EFF3DVertexElement(source,offset,theType,semantic,index,method));
	return m_aryElement[m_aryElement.size()-1];
}

const EFF3DVertexElement & EFF3DVertexDeclaration::InsertElement(effWORD atPosition,effWORD source,effWORD offset,EFF3DVertexElementType theType,EFF3DVertexElementSemantic semantic,effBYTE index /* = 0*/)
{
	m_bNeedRebuild = effTRUE;

	if ( atPosition >= (effWORD)m_aryElement.size() )
	{
		return AddElement(source,offset,theType,semantic,index);
	}

	VertexElementArray::iterator it = m_aryElement.begin();
	it += atPosition;
	m_aryElement.insert(it,EFF3DVertexElement(source,offset,theType,semantic,index));
	return m_aryElement[atPosition+1];

}

const EFF3DVertexElement * EFF3DVertexDeclaration::GetElement(effWORD index)
{
	assert(index < (effWORD)m_aryElement.size());
	return &m_aryElement[index];
}

effVOID EFF3DVertexDeclaration::RemoveElement(effWORD elemIndex)
{
	assert(elemIndex < (effWORD)m_aryElement.size());
	m_bNeedRebuild = effTRUE;

	VertexElementArray::iterator it  = m_aryElement.begin();
	it += elemIndex;
	m_aryElement.erase(it);
}

/*effVOID EFF3DVertexDeclaration::RemoveElement(EFF3DVertexElementSemantic semantic,effWORD index)
{
	VertexElementArray::it = m_aryElement.begin();
	for ( effUINT i = 0; i != m_aryElement.size(); i++,it++ )
	{
		if (m_aryElement[i].GetSemantic() == semantic && m_aryElement[i].GetSemanticIndex() == index)
		{
			m_aryElement.erase(it);
			break;
		}
	}
}*/

effVOID EFF3DVertexDeclaration::ModifyElement(effWORD elemIndex, 
											effWORD source, effWORD offset, EFF3DVertexElementType theType,
											EFF3DVertexElementSemantic semantic, effBYTE index /* = 0*/)
{
	assert(elemIndex < (effWORD)m_aryElement.size());
	m_bNeedRebuild = effTRUE;
	m_aryElement[elemIndex] = EFF3DVertexElement(source, offset, theType, semantic, index);
}





effUINT EFF3DVertexDeclaration::GetVertexSize(effWORD stream)
{
	return CalculateVertexSize(m_aryElement,stream);
}

effUINT CalculateVertexSize(std::vector<EFF3DVertexElement> & aryElement,effWORD stream)
{
	effUINT sz = 0;

	for ( effUINT i = 0; i < aryElement.size(); i++ )
	{
		if ( aryElement[i].GetStream() == stream )
		{
			sz += aryElement[i].GetSize();
		}
	}
	return sz;
}

effUINT EFF3DVertexDeclaration::GetFVF()
{
	return 0;
}


effWORD EFF3DVertexDeclaration::GetMaxSource(effVOID) const
{
	effWORD ret = 0;
	for ( effUINT i = 0; i < m_aryElement.size(); i++ )
	{
		if ( m_aryElement[i].GetStream() > ret )
		{
			ret = m_aryElement[i].GetStream();
		}
	}
	return ret;
}


EFF3D_END