/******************************************************************************
	created:	2008-12-10   0:48
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFObject.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFObject_2008_12_10__
#define __EFFObject_2008_12_10__

#include "EFFIUnknown.h"


EFFBASE_BEGIN

class EFFBASE_API EFFObject : public EFFIUnknown
{

	/*RTTI_DECLARE(EFFObject,EFFIUnknown)
	{

	}
public:
	BEGIN_PROPERTY_MAP
	END_PROPERTY_MAP


	BEGIN_METHOD_MAP
	END_METHOD_MAP*/
public:
	EFFObject();
public:
	virtual effHRESULT __stdcall		QueryInterface(ClassID & classID,EFFIUnknown ** ppComponent);
	virtual effULONG __stdcall			AddRef();
	virtual effULONG __stdcall			Release();

	virtual effULONG 	__stdcall		GetObjectID() { return m_ulObjectId; }
protected:
	effULONG				m_ulObjectId;
	effULONG				m_ulRefCount;
};


EFFBASE_END

#endif

