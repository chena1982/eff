/******************************************************************************
	created:	2012-10-20   15:24
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFGameObject.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFGameObject_2012_10_20__
#define __EFFGameObject_2012_10_20__

#include "EFFComponent.h"

EFFBASE_BEGIN

class EFFBASE_API EFFGameObject : public EFFObject
{
	RTTI_DECLARE(EFFGameObject, EFFObject)
public:
	virtual ~EFFGameObject();
protected:
	EFFGameObject();

public:
	EFFComponent *		AddComponent(const ClassID & classID);
	EFFComponent *		GetComponent(const ClassID & classID);
	EFFComponent *		GetComponent(effINT index);
	effINT				GetComponentCount() { return components.size(); }

	virtual effVOID		SaveComponentsToFile(EFFFile * file, effBOOL isBinary, YAML::Emitter * textOut);
protected:
	std::vector<EFFComponent *>		components;
};

EFFBASE_END


#endif