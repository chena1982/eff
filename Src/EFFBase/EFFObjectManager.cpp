/******************************************************************************
	created:	2008-12-10   23:03
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObjectManager.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFObject.h"
#include "EFFObjectManager.h"
#include "EFFGlobal.h"


EFFBASE_BEGIN

RTTI_IMPLEMENT_BASE_NO_SAVE(EFFObjectManager, 0)

MAP<EFFClass *, EFFObjectManager *> objectManagers;

effVOID EFFRegisterObjectManager(EFFClass * Class, EFFObjectManager * objectManager)
{
	MAP<EFFClass *, EFFObjectManager *>::iterator it = objectManagers.begin();

	effBOOL registered = effFALSE;
	for ( ; it != objectManagers.end(); it++ )
	{
		if ( Class->IsKindOf(it->first) )
		{
			registered = effTRUE;
			break;
		}
	}

	if ( !registered )
	{
		objectManagers[Class] = objectManager;
	}
}


EFFObjectManager * EFFGetObjectManager(EFFClass * Class)
{
	MAP<EFFClass *, EFFObjectManager *>::iterator it = objectManagers.begin();
	for ( ; it != objectManagers.end(); it++ )
	{
		if ( Class->GetID() == it->first->GetID() )
		{
			return it->second;
		}
	}

	if ( Class->IsKindOf(EFFObject::GetThisClass()) )
	{
		EFFContext * context = GetEFFContext();
		return context->GetDefaultObjectManager();
	}

	return NULL;
}

EFFObjectManager::EFFObjectManager() : objects(100)
{
	//currentId = 0;
}

EFFObjectManager::~EFFObjectManager()
{
}


EFFObject * EFFObjectManager::CreateObject(EFFClass * Class)
{
	BOOST_ASSERT(Class != NULL);
	BOOST_ASSERT(Class->IsKindOf(EFFObject::GetThisClass()));

	EFFObject * object = static_cast<EFFObject *>(Class->CreateObject());

	//CalculateNextId();

	object->AddRef();

	//BOOST_ASSERT(objects.find(currentId) == objects.end());

	effINT id = objects.Add(object);
	object->SetID(id);


	return object;
}

void EFFObjectManager::ReleaseObject(EFFObject * object)
{
	BOOST_ASSERT(object != NULL);

	effUINT objectId = object->GetID();
	EFFObject * foundObject = objects[objectId];
	if ( foundObject != NULL )
	{
		effUINT refCount = object->GetRef();
		object->Release();

		if ( refCount == 1 )
		{
			objects.Remove(objectId);
		}
	}
	
}

EFFObject * EFFObjectManager::GetObject(effINT objectId)
{
	return objects[objectId];
}

/*effVOID EFFObjectManager::CalculateNextId()
{
	if ( recycledIds.size() == 0 )
	{
		currentId++;
		return;
	}

	currentId = recycledIds[0];
	recycledIds.erase(recycledIds.begin());
}*/

effVOID EFFObjectManager::SaveToFile(EFFFile * file, effBOOL isBinary)
{
}

effVOID EFFObjectManager::SaveToFile(const effString & filePath, effBOOL isBinary)
{
}

EFFBASE_END