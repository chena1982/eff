/******************************************************************************
	created:	2008-12-10   23:03
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObjectManager.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFIUnknown.h"
#include "EFFObjectManager.h"


#define new EFFNEW

EFFBASE_BEGIN

EFFObjectManager::EFFObjectManager()
{
}

EFFObjectManager::~EFFObjectManager()
{
}


effBOOL EFFObjectManager::AddObject(EFFIUnknown * pObject)
{
	/*effULONG ulObjectId = pObject->GetObjectID();
	std::map<ClassID,std::map<effULONG,EFFIUnknown *>>::iterator it = m_mapObject.find(pObject->GetThisClass()->GetID());
	if ( it != m_mapObject.end() )
	{
		std::map<effULONG,EFFIUnknown *>::iterator itObject = it->second.find(ulObjectId);
		if ( itObject == it->second.end() )
		{
			it->second[ulObjectId] = pObject;
			return effTRUE;
		}
		else
		{
			return effFALSE;
		}
	}
	std::map<effULONG,EFFIUnknown *> mapObject;
	mapObject[ulObjectId] = pObject;
	m_mapObject[pObject->GetThisClass()->GetID()] = mapObject;*/
	return effTRUE;
}

EFFIUnknown * EFFObjectManager::GetObject(ClassID & classId,effULONG ulObjectId)
{
	std::map<ClassID,std::map<effULONG,EFFIUnknown *>>::iterator it = m_mapObject.find(classId);
	if ( it != m_mapObject.end() )
	{
		std::map<effULONG,EFFIUnknown *>::iterator itObject = it->second.find(ulObjectId);
		if ( itObject != it->second.end() )
		{
			return itObject->second;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

EFFBASE_END