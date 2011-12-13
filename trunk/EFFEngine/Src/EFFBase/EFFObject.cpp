/******************************************************************************
	created:	2008-12-10   0:53
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObject.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFObject.h"

#define new EFFNEW



EFFBASE_BEGIN

RTTI_IMPLEMENT_BASE(EFFObject, 0)

EFFObject::EFFObject()
{
	refCount = 0;
	objectId = 0;
}

EFFObject::~EFFObject()
{
	componentArray.clear();
}

//这里暂时有个问题，如果一个Object包含了两个动态组件，如果这两个动态组件都支持同一个接口，那么
//无法判断返回哪个接口
EFFComponent * EFFObject::GetComponent(ClassID & classID)
{	
	std::vector<EFFComponent *>::iterator it = componentArray.begin();

	for ( ; it != componentArray.end(); it++ )
	{
		if ( (*it)->GetRuntimeClass()->IsKindOf(classID) )
		{
			return *it;
		}
	}
	return NULL;
}

effULONG EFFObject::AddRef()
{
	return ++refCount;
}

effULONG EFFObject::Release()
{
	effULONG ulRefCount = refCount--;
	if ( ulRefCount == 0 )
	{
		delete this;
	}
	return ulRefCount;
}


EFFComponent * EFFObject::AddComponent(ClassID & classID)
{
	EFFComponent * pComponent = static_cast<EFFComponent *>(EFFCreateObject(classID));

	componentArray.push_back(pComponent);
	return pComponent;
}

EFFBASE_END