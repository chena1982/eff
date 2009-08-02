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

class EFFIUnknown;


//远程调用时代理对象的管理器
class EFFObjectManager
{
public:
	EFFObjectManager();
	~EFFObjectManager();
public:
	effBOOL					AddObject(EFFIUnknown * pObject);
	EFFIUnknown *			GetObject(ClassID & classId,effULONG objectId);
protected:
	std::map<ClassID,std::map<effULONG,EFFIUnknown *>>		m_mapObject;
};

EFFBASE_END

#endif