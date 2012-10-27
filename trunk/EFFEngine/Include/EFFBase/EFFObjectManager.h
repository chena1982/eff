/******************************************************************************
	created:	2008-12-10   22:57
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObjectManager.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFObjectManager_2008_12_10__
#define __EFFObjectManager_2008_12_10__



EFFBASE_BEGIN

class EFFObject;



class EFFBASE_API EFFObjectManager
{
	RTTI_DECLARE_BASE(EFFObjectManager);
public:
	EFFObjectManager();
	virtual ~EFFObjectManager();
public:
	std::map<effUINT, EFFObject *>			GetObjects() { return objects; }
public:
	EFFObject *								CreateObject(EFFClass * Class);
	effVOID									ReleaseObject(EFFObject * object);
	EFFObject *								GetObject(effUINT objectId);
protected:
	effVOID									CalculateNextId();
protected:
	effUINT									currentId;
	std::vector<effUINT>					recycledIds;
	std::map<effUINT, EFFObject *>			objects;
};

EFFBASE_API effVOID EFFRegisterObjectManager(EFFClass * Class, EFFObjectManager * objectManager);
EFFBASE_API EFFObjectManager * EFFGetObjectManager(EFFClass * Class);

EFFBASE_END

#endif