/******************************************************************************
	created:	2008-12-10   0:56
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFIUnknown.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFComponent.h"






EFFBASE_BEGIN

RTTI_IMPLEMENT_BASE(EFFReferenceCount, 0)
RTTI_IMPLEMENT(EFFComponent, 0)


effUINT EFFReferenceCount::AddRef()
{
	return ++refCount;
}

effVOID EFFReferenceCount::Release()
{
	refCount--;
	if ( refCount == 0 )
	{
		delete this;
	}
}

EFFBASE_END