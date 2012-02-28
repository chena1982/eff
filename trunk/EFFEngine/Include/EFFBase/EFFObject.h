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

class EFFBASE_API EFFObject : public EFFReferenceCount
{
	friend class EFFObjectManager;
	RTTI_DECLARE(EFFObject, EFFReferenceCount)
public:
	virtual ~EFFObject();

protected:
	EFFObject();
	void				SetObjectID(effULONG objectId) { this->objectId = objectId; }
public:
	EFFComponent *		GetComponent(ClassID & classID);
	EFFComponent *		GetComponent(effINT index);
	effINT				GetComponentCount() { return components.size(); }

	EFFComponent *		AddComponent(ClassID & classID);
	effULONG			GetObjectID() { return objectId; }


protected:
	effULONG						objectId;
	std::vector<EFFComponent *>		components;
};


EFFBASE_END

#endif

