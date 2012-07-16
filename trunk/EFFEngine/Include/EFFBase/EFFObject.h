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
	void				SetID(effULONG id) { this->id = id; }
public:
	EFFComponent *		GetComponent(const ClassID & classID);
	EFFComponent *		GetComponent(effINT index) { return components[index]; }
	effINT				GetComponentCount() { return components.size(); }

	EFFComponent *		AddComponent(const ClassID & classID);
	effULONG			GetID() { return id; }

	effString			GetName() { return name; }
	effVOID				SetName(const effString & name) { this->name = name; }


protected:
	effULONG						id;
	std::vector<EFFComponent *>		components;
	effString						name;
};


EFFBASE_END

#endif

