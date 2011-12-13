/******************************************************************************
	created:	2008-12-10   0:48
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFObject.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFObject_2008_12_10__
#define __EFFObject_2008_12_10__

#include "EFFComponent.h"


EFFBASE_BEGIN

class EFFBASE_API EFFObject
{
	friend class EFFObjectManager;
	RTTI_DECLARE_BASE(EFFObject)
public:
	virtual ~EFFObject();

private:
	EFFObject();
	effULONG			AddRef();
	effULONG			Release();
	void				SetObjectID(effULONG objectId) { this->objectId = objectId; }
public:
	EFFComponent *		GetComponent(ClassID & classID);
	EFFComponent *		AddComponent(ClassID & classID);
	effULONG			GetObjectID() { return objectId; }
	effULONG			GetRef() { return refCount; }

protected:
	effULONG						objectId;
	effULONG						refCount;
	std::vector<EFFComponent *>		componentArray;
};


EFFBASE_END

#endif

