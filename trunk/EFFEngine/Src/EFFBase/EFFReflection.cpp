/******************************************************************************
	created:	2008-12-1   23:17
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFReflection.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFReflection.h"
#include "EFFRtti.h"

#define new EFFNEW

EFFBASE_BEGIN

std::string getClassNameFromTypeName(const char * pszTypeName)
{
	std::string strClassName = pszTypeName;
	int nPos = strClassName.find("class ");
	if ( nPos != -1 )
	{
		strClassName.erase(0,6);
	}
	return strClassName;
}

/*template<class T>
__register_property__::__register_property__(unsigned long ulOffset, unsigned long ulSize, const char * pszName, EFFClass * pClass, const T & defaultValue)
{
	__real_property__<T> * pProperty = new __real_property__<T>(ulOffset, ulSize, pszName, defaultValue);
	pClass->AddProperty(pProperty);
};*/


/*template<class T>
__register_property_minmax__<T>::__register_property_minmax__(unsigned long ulOffset, unsigned long ulSize, const char * pszName, EFFClass * pClass, const T & defaultValue, const T & minValue, const T & maxValue)
{
	__real_property__<T> * pProperty = new __real_property__<T>(ulOffset, ulSize, pszName, defaultValue, minValue, maxValue);
	pClass->AddProperty(pProperty);
};*/

EFFBASE_END