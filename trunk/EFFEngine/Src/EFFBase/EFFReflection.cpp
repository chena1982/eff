#include "stdafx.h"
#include "EFFReflection.h"
#include "EFFRtti.h"


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

__register_property__::__register_property__(unsigned long ulOffset,unsigned long ulSize,const char * pszName,EFFClass * pClass)
{
	__property__ property(ulOffset,ulSize,pszName);
	pClass->AddProperty(property);
}