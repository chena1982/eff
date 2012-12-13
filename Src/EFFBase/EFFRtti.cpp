/******************************************************************************
	created:	2008-12-1   23:19
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFRtti.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFRtti.h"
#include "EFFProperty.h"


EFFBASE_BEGIN




RTTI_IMPLEMENT_POD(effString)
RTTI_IMPLEMENT_POD(effINT)
RTTI_IMPLEMENT_POD(effUINT)
RTTI_IMPLEMENT_POD(effFLOAT)
RTTI_IMPLEMENT_POD(effBOOL)
RTTI_IMPLEMENT_PURE_POD(effVOID)

//不直接使用全局静态变量，因为无法保证全局静态变量的初始化顺序，如果别的静态变量比mapEFFRunTimeTypeInfo先初始化，
//而且这个静态变量在构造函数里调用了EFFRegisterClass，那么程序会Crash
std::map<ClassID, EFFClass *> & GetRuntimeTypeInfoMap()
{
	static std::map<ClassID, EFFClass *> effRunTimeTypeInfo;
	return effRunTimeTypeInfo;
}

effVOID EFFRegisterClass(EFFClass * pClass)
{
	GetRuntimeTypeInfoMap().insert(std::make_pair(pClass->GetID(), pClass));
}

effVOID EFFUnRegisterClass(EFFClass * pClass)
{
	std::map<ClassID, EFFClass *>::iterator it = GetRuntimeTypeInfoMap().find(pClass->GetID());
	if ( it != GetRuntimeTypeInfoMap().end() )
	{
		GetRuntimeTypeInfoMap().erase(it);
	}
}

effVOID * EFFCreateObject(const effString & className)
{
	return EFFCreateObject(ClassIDFromString(className));
}

effVOID * EFFCreateObject(const ClassID & classId)
{
	std::map<ClassID, EFFClass *>::iterator it = GetRuntimeTypeInfoMap().find(classId);
	if ( it != GetRuntimeTypeInfoMap().end() )
	{
		return it->second->CreateObject();
	}
	return NULL;
}

EFFClass * EFFGetClass(const effString & className)
{
	return EFFGetClass(ClassIDFromString(className));
}

EFFClass * EFFGetClass(const ClassID & classId)
{
	std::map<ClassID, EFFClass *>::iterator it = GetRuntimeTypeInfoMap().find(classId);
	if ( it != GetRuntimeTypeInfoMap().end() )
	{
		return it->second;
	}
	return NULL;
}

ClassID ClassIDFromString(const effString & className)
{
	const char * ansiClassName = EFFSTRING2ANSI(className);



	MD5 context;
	unsigned int len = strlen(ansiClassName);


	
	context.update((unsigned char *)ansiClassName, len);
	context.finalize();

	unsigned char * pDigset = context.raw_digest();

	//char * sz = context.hex_digest();

	ClassID classId;
	unsigned char * pTemp = (unsigned char *)&classId;
	for ( int i = 0; i < 8; i++ )
	{
		*pTemp = pDigset[7-i];
		pTemp++;
	}

	for ( int i = 0; i < 8; i++ )
	{
		*pTemp = pDigset[15-i];
		pTemp++;
	}


	return classId;
}

effVOID SetProperty(EFFProperty * addedProperty, effLONG offset, effLONG size, const effString & name)
{
	addedProperty->SetOffset(offset);
	addedProperty->SetSize(size);
	addedProperty->SetName(name);
	//addedProperty->SetClass(propertyClass);
	EFFClass * propertyClass = addedProperty->GetClass();

	if ( propertyClass != NULL && propertyClass->IsPODType() )
	{
		if ( propertyClass->GetNameHash() == EFFStringHash(_effT("effString")) )
		{
			addedProperty->SetSavePropertyFP(SaveStringProperty);
		}
		else
		{
			addedProperty->SetSavePropertyFP(SavePODProperty);
		}
	}


}


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