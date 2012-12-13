/******************************************************************************
	created:	2008-12-10   23:03
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObjectManager.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFObject.h"
#include "EFFObjectManager.h"



EFFBASE_BEGIN

RTTI_IMPLEMENT_BASE(EFFObjectManager, 0)

std::map<EFFClass *, EFFObjectManager *> objectManagers;

effVOID EFFRegisterObjectManager(EFFClass * Class, EFFObjectManager * objectManager)
{
	std::map<EFFClass *, EFFObjectManager *>::iterator it = objectManagers.begin();

	effBOOL alreadyHave = effFALSE;
	for ( ; it != objectManagers.end(); it++ )
	{
		if ( Class->IsKindOf(it->first) )
		{
			alreadyHave = effTRUE;
			break;
		}
	}

	if ( !alreadyHave )
	{
		objectManagers[Class] = objectManager;
	}
}


EFFObjectManager * EFFGetObjectManager(EFFClass * Class)
{
	std::map<EFFClass *, EFFObjectManager *>::iterator it = objectManagers.begin();
	for ( ; it != objectManagers.end(); it++ )
	{
		if ( Class->IsKindOf(it->first) )
		{
			return it->second;
		}
	}

	return NULL;
}

EFFObjectManager::EFFObjectManager()
{
	currentId = 0;
}

EFFObjectManager::~EFFObjectManager()
{
}


EFFObject * EFFObjectManager::CreateObject(EFFClass * Class)
{
	BOOST_ASSERT(Class != NULL);
	BOOST_ASSERT(Class->IsKindOf(EFFObject::GetThisClass()));

	EFFObject * object = static_cast<EFFObject *>(EFFCreateObject(Class->GetID()));

	CalculateNextId();
	object->SetID(currentId);
	object->AddRef();

	BOOST_ASSERT(objects.find(currentId) == objects.end());

	objects[currentId] = object;

	return object;
}

void EFFObjectManager::ReleaseObject(EFFObject * object)
{
	BOOST_ASSERT(object != NULL);


	std::map<effUINT, EFFObject *>::iterator it = objects.find(object->GetID());
	if ( it != objects.end() )
	{
		effUINT refCount = object->GetRef();
		effUINT objectId = object->GetID();
		object->Release();
		if ( refCount == 1 )
		{
			objects.erase(it);
			recycledIds.push_back(objectId);
		}
	}
	
}

EFFObject * EFFObjectManager::GetObject(effUINT objectId)
{

	std::map<effUINT, EFFObject *>::iterator it = objects.find(objectId);
	if ( it != objects.end() )
	{
		//it->second->AddRef();
		return it->second;
	}
	return NULL;
}

effVOID EFFObjectManager::CalculateNextId()
{
	if ( recycledIds.size() == 0 )
	{
		currentId++;
		return;
	}

	currentId = recycledIds[0];
	recycledIds.erase(recycledIds.begin());
}

EFFBASE_END