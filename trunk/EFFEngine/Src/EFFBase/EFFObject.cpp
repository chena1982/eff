/******************************************************************************
	created:	2008-12-10   0:53
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObject.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFObject.h"

#define new EFFNEW

//RTTI_IMPLEMENT(EFFObject,0)

EFFBASE_BEGIN

EFFObject::EFFObject()
{
	m_ulRefCount = 0;
	m_ulObjectId = 0;
}

effHRESULT EFFObject::QueryInterface(ClassID & classID,EFFIUnknown ** ppComponent)
{

	*ppComponent = this;

	return S_OK;
}

effULONG EFFObject::AddRef()
{
	return ++m_ulRefCount;
}

effULONG EFFObject::Release()
{
	effULONG ulRefCount = m_ulRefCount--;
	if ( ulRefCount == 0 )
	{
		delete this;
	}
	return ulRefCount;
}


EFFBASE_END