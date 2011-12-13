/******************************************************************************
	created:	2008-12-10   23:03
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObjectManager.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFObject.h"
#include "EFFObjectManager.h"


#define new EFFNEW

EFFBASE_BEGIN

EFFObjectManager::EFFObjectManager()
{
}

EFFObjectManager::~EFFObjectManager()
{
}


EFFObject * EFFObjectManager::CreateObject(ClassID & classId)
{
	EFFClass * wantCreateClass = EFFGetClass(classId);
	if ( wantCreateClass == NULL )
	{
		return NULL;
	}

	if ( !wantCreateClass->IsKindOf(EFFObject::GetThisClass()) )
	{
		return NULL;
	}

	EFFObject * object = static_cast<EFFObject *>(EFFCreateObject(classId));

	//直接根据指针地址计算id，暂时先这样
	object->SetObjectID(reinterpret_cast<effULONG>(object) / sizeof(object));
	object->AddRef();

	std::map<ClassID, std::map<effULONG, EFFObject *>>::iterator it = objectMap.find(classId);
	if ( it == objectMap.end() )
	{
		std::map<effULONG, EFFObject *> realObjectMap;
		realObjectMap[object->GetObjectID()] = object;
		objectMap[classId] = realObjectMap;
	}
	else
	{
		it->second[object->GetObjectID()] = object;
	}

	return NULL;
}

void EFFObjectManager::ReleaseObject(EFFObject * object)
{
	if ( object == NULL )
	{
		return;
	}

	std::map<ClassID, std::map<effULONG, EFFObject *>>::iterator it = objectMap.find(object->GetRuntimeClass()->GetID());
	if ( it != objectMap.end() )
	{
		std::map<effULONG, EFFObject *>::iterator itObject = it->second.find(object->GetObjectID());
		if ( itObject != it->second.end() )
		{
			effINT refCount = itObject->second->GetRef();
			itObject->second->Release();
			it->second.erase(itObject);

		}
	}
}

EFFObject * EFFObjectManager::GetObject(ClassID & classId, effULONG objectId)
{
	std::map<ClassID, std::map<effULONG, EFFObject *>>::iterator it = objectMap.find(classId);
	if ( it != objectMap.end() )
	{
		std::map<effULONG, EFFObject *>::iterator itObject = it->second.find(objectId);
		if ( itObject != it->second.end() )
		{
			//只有通过GetObject接口得到EFFObject指针时才增加引用
			//itObject->second->AddRef();
			return itObject->second;
		}
	}
	return NULL;
}

EFFBASE_END