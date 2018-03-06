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


EFF3DVertexElement::EFF3DVertexElement(effWORD stream, effWORD offset, EFF3DVertexElementType type, EFF3DVertexElementSemantic semantic, effBYTE index /* = 0 */, EFF3DVertexElementMethod method /* = VEM_DEFAULT */)
: stream(stream), offset(offset), type(type), semantic(semantic), semanticIndex(index), method(method)
{
}

EFF3DVertexElement::EFF3DVertexElement()
{
}

effUINT EFF3DVertexElement::GetSize() const
{
	return GetTypeSize((EFF3DVertexElementType)type);
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
    needRebuild = effTRUE;
}

EFF3DVertexDeclaration::EFF3DVertexDeclaration(const EFF3DVertexDeclaration & rhs)
{
    elements = rhs.elements;
    needRebuild = rhs.needRebuild;
}

EFF3DVertexDeclaration::~EFF3DVertexDeclaration()
{
}

const EFF3DVertexDeclaration::VertexElementArray & EFF3DVertexDeclaration::GetElements() const
{
	return elements;
}


const EFF3DVertexElement & EFF3DVertexDeclaration::AddElement(effWORD source,effWORD offset,EFF3DVertexElementType theType,EFF3DVertexElementSemantic semantic,effBYTE index /* = 0 */,EFF3DVertexElementMethod method /* = VEM_DEFAULT */)
{
    needRebuild = effTRUE;

    elements.push_back(EFF3DVertexElement(source,offset,theType,semantic,index,method));
	return elements[elements.size() - 1];
}

const EFF3DVertexElement & EFF3DVertexDeclaration::InsertElement(effWORD atPosition,effWORD source,effWORD offset,EFF3DVertexElementType theType,EFF3DVertexElementSemantic semantic,effBYTE index /* = 0*/)
{
	needRebuild = effTRUE;

	if ( atPosition >= (effWORD)elements.size() )
	{
		return AddElement(source,offset,theType,semantic,index);
	}

	VertexElementArray::iterator it = elements.begin();
	it += atPosition;
    elements.insert(it,EFF3DVertexElement(source,offset,theType,semantic,index));
	return elements[atPosition+1];

}

const EFF3DVertexElement * EFF3DVertexDeclaration::GetElement(effWORD index)
{
	assert(index < (effWORD)elements.size());
	return &elements[index];
}

effVOID EFF3DVertexDeclaration::RemoveElement(effWORD elemIndex)
{
	assert(elemIndex < (effWORD)elements.size());
    needRebuild = effTRUE;

	VertexElementArray::iterator it  = elements.begin();
	it += elemIndex;
    elements.erase(it);
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
	assert(elemIndex < (effWORD)elements.size());
    needRebuild = effTRUE;
    elements[elemIndex] = EFF3DVertexElement(source, offset, theType, semantic, index);
}





effUINT EFF3DVertexDeclaration::GetVertexSize(effWORD stream)
{
	return CalculateVertexSize(elements, stream);
}

effUINT CalculateVertexSize(VECTOR<EFF3DVertexElement> & aryElement,effWORD stream)
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
	for ( effUINT i = 0; i < elements.size(); i++ )
	{
		if (elements[i].GetStream() > ret )
		{
			ret = elements[i].GetStream();
		}
	}
	return ret;
}


EFF3D_END