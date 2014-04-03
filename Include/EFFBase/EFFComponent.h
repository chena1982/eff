/******************************************************************************
	created:	2008-12-1   23:33
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFComponent.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFComponent_2008_12_1__
#define __EFFComponent_2008_12_1__


#include "EFFObject.h"

EFFBASE_BEGIN


class EFFProperty;


class EFFBASE_API EFFComponent : public EFFObjectBase
{
	RTTI_DECLARE_CUSTOM_SAVE(EFFComponent, EFFObjectBase)
public:
	virtual ~EFFComponent() {}
public:
	inline effBOOL		GetEnabled() { return enabled; }
	inline effVOID		SetEnabled(effBOOL enabled) { this->enabled = enabled; }

	inline EFFObject *	GetObject() { return object; }
	inline effVOID		SetObject(EFFObject * object) { this->object = object; }
protected:
	EFFComponent();

protected:
	effBOOL			enabled;
	EFFObject *		object;
};


EFFBASE_END

#endif