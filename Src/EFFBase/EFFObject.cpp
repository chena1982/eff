/******************************************************************************
	created:	2008-12-10   0:53
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObject.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFObject.h"



EFFBASE_BEGIN


RTTI_IMPLEMENT_BASE(EFFReferenceCount, 0)
RTTI_IMPLEMENT(EFFObject, 0)



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



EFFObject::EFFObject()
{
	id = 0;

}

EFFObject::~EFFObject()
{

}




EFFProperty * EFFObject::GetProperty(const effString & propertyName)
{
	EFFClass * objectClass = GetRuntimeClass();
	auto prperties = objectClass->GetProperties();
	for ( effUINT i = 0; i < prperties.size(); i++ )
	{
		if ( prperties[i]->GetName() == propertyName )
		{
			return prperties[i];
		}
	}

	return NULL;
}

EFFBASE_END