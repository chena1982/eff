/******************************************************************************
	created:	2008-12-10   0:53
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObject.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFObject.h"



EFFBASE_BEGIN

RTTI_IMPLEMENT(EFFObject, 0)

EFFObject::EFFObject()
{
	id = 0;
}

EFFObject::~EFFObject()
{
	components.clear();
}

//这里暂时有个问题，如果一个Object包含了两个动态组件，如果这两个动态组件都支持同一个接口，那么
//无法判断返回哪个接口
EFFComponent * EFFObject::GetComponent(const ClassID & classID)
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




EFFComponent * EFFObject::AddComponent(const ClassID & classID)
{
	EFFComponent * component = static_cast<EFFComponent *>(EFFCreateObject(classID));
	component->SetObject(this);
	components.push_back(component);
	return component;
}

EFFBASE_END