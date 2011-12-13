/******************************************************************************
	created:	2008-12-1   23:33
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFComponent.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFComponent_2008_12_1__
#define __EFFComponent_2008_12_1__

#include "EFFRtti.h"

EFFBASE_BEGIN



class EFFBASE_API EFFComponent
{
	RTTI_DECLARE_BASE_PURE(EFFComponent)

public:
	EFFComponent() {}
	virtual ~EFFComponent() {}
};




EFFBASE_END

#endif