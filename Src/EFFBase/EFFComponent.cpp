/******************************************************************************
	created:	2008-12-10   0:56
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFIUnknown.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFComponent.h"
#include "EFFEntity.h"





EFFBASE_BEGIN


RTTI_IMPLEMENT_BASE_CUSTOM_SAVE(EFFComponent, 0)


EFFComponent::EFFComponent()
{
	SetEnabled(effTRUE);
}

effVOID EFFComponent::SaveToFile(EFFFile * file, effBOOL isBinary)
{
	//EFFObjectBase::SaveToFile(file, isBinary);
}

effVOID EFFComponent::SaveToFile(const effString & filePath, effBOOL isBinary)
{
	/*if ( isBinary )
	{
		EFFSTLFile file;
		if (!file.Open(filePath, _effT("wb")))
		{
			return;
		}
		SaveToFile(&file, isBinary);
	}
	else
	{
		EFFSTLFile file;
		if (!file.Open(filePath, _effT("wt")))
		{
			return;
		}

		if (StartSaveToYAMLFile(file))
		{
			//BeginSaveObjectBaseToYAMLFile(this, 1);
			SaveToFile(NULL, isBinary);
			//EndSaveObjectBaseToYAMLFile(this);
			EndSaveToYAMLFile();
		}
	}*/
}

/*EFFProperty * EFFComponent::GetProperty(const effString & name)
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
}*/


EFFBASE_END