/******************************************************************************
	created:	2008-12-1   23:19
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFRtti.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFRtti.h"
#include "EFFProperty.h"
#include "EFFSerialize.h"

EFFBASE_BEGIN




RTTI_IMPLEMENT_POD(effString)
RTTI_IMPLEMENT_POD(effINT)
RTTI_IMPLEMENT_POD(effUINT)
RTTI_IMPLEMENT_POD(effFLOAT)
RTTI_IMPLEMENT_POD(effBOOL)
RTTI_IMPLEMENT_PURE_POD(effVOID)








/*effString EFFClass::getPODTypeClassName(const effCHAR * propertyTypeName)
{
	if ( strcmp(propertyTypeName, "int") == 0 )
	{
		return effString(_effT("effINT"));
	}
	else if ( strcmp(propertyTypeName, "float") == 0 )
	{
		return effString(_effT("effFLOAT"));
	}
	return effString(_effT("unkown pod type"));
}*/



EFFBASE_END