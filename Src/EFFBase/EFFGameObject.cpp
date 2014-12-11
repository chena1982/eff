/******************************************************************************
	created:	2012-10-20   15:33
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFGameObject.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFGameObject.h"



EFFBASE_BEGIN

RTTI_IMPLEMENT(EFFGameObject, 0)

EFFGameObject::EFFGameObject()
{
}


EFFGameObject::~EFFGameObject()
{
	components.clear();
}

//这里暂时有个问题，如果一个Object包含了两个动态组件，如果这两个动态组件都支持同一个接口，那么
//无法判断返回哪个接口
EFFComponent * EFFGameObject::GetComponent(const ClassID & classID)
{	
	std::vector<EFFComponent *>::iterator it = components.begin();

	for ( ; it != components.end(); it++ )
	{
		if ( (*it)->GetRuntimeClass()->IsKindOf(classID) )
		{
			return *it;
		}
	}
	return NULL;
}

EFFComponent * EFFGameObject::GetComponent(effINT index)
{
	if ( index < 0 || index >= (effINT)components.size() )
	{
		return NULL;
	}

	return components[index];
}




EFFComponent * EFFGameObject::AddComponent(const ClassID & classID)
{
	EFFComponent * component = static_cast<EFFComponent *>(EFFCreateObject(classID));
	component->SetObject(this);
	components.push_back(component);
	return component;
}

effVOID	EFFGameObject::SaveComponentsToFile(EFFFile * file, effBOOL isBinary)
{
	for ( effUINT i = 0; i < components.size(); i++ )
	{
		components[i]->SaveToFile(file, isBinary);
	}
}
EFFBASE_END