/******************************************************************************
	created:	2008-12-10   0:56
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFIUnknown.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFComponent.h"






EFFBASE_BEGIN


RTTI_IMPLEMENT(EFFComponent, 0)


EFFComponent::EFFComponent()
{
	SetEnabled(effTRUE);
	object = NULL;
}

EFFProperty * EFFComponent::GetProperty(const effString & name)
{
	EFFClass * componentClass = GetRuntimeClass();
	auto prperties = componentClass->GetProperties();
	for ( effUINT i = 0; i < prperties.size(); i++ )
	{
		if ( prperties[i]->GetName() == name )
		{
			return prperties[i];
		}
	}

	return NULL;
}

EFFBASE_END