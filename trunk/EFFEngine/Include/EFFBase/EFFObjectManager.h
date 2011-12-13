/******************************************************************************
	created:	2008-12-10   22:57
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObjectManager.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFObjectManager_2008_12_10__
#define __EFFObjectManager_2008_12_10__

#include "EFFRtti.h"

EFFBASE_BEGIN

class EFFObject;



class EFFObjectManager
{
public:
	EFFObjectManager();
	~EFFObjectManager();
public:
	EFFObject *			CreateObject(ClassID & classId);
	void				ReleaseObject(EFFObject * object);
	EFFObject *			GetObject(ClassID & classId, effULONG objectId);
protected:
	std::map<ClassID, std::map<effULONG, EFFObject *>>		objectMap;
};

EFFBASE_END

#endif