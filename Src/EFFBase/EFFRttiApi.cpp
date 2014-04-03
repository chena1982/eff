/******************************************************************************
	created:	2012-12-26   23:36
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFRttiApi.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFRttiApi.h"
#include "EFFProperty.h"
#include "EFFObjectManager.h"
//#include "EFFSerialize.h"

EFFBASE_BEGIN

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

//不直接使用全局静态变量，因为无法保证全局静态变量的初始化顺序，如果别的静态变量比mapEFFRunTimeTypeInfo先初始化，
//而且这个静态变量在构造函数里调用了EFFRegisterClass，那么程序会Crash
std::map<ClassID, EFFClass *> & GetRuntimeTypeInfoMap()
{
	static std::map<ClassID, EFFClass *> effRunTimeTypeInfo;
	return effRunTimeTypeInfo;
}

effVOID EFFRegisterClass(EFFClass * Class)
{
	GetRuntimeTypeInfoMap().insert(std::make_pair(Class->GetID(), Class));
}

effVOID EFFUnRegisterClass(EFFClass * Class)
{
	std::map<ClassID, EFFClass *>::iterator it = GetRuntimeTypeInfoMap().find(Class->GetID());
	if ( it != GetRuntimeTypeInfoMap().end() )
	{
		GetRuntimeTypeInfoMap().erase(it);
	}
}

effVOID * EFFCreateObject(EFFClass * Class)
{
	if ( Class == NULL )
	{
		return NULL;
	}

	if ( Class->IsPODType() )
	{
		return Class->CreateObject();
	}
	else
	{
		EFFObjectManager * objectManager = EFFGetObjectManager(Class);

		if ( objectManager == NULL )
		{
			return NULL;
		}

		return objectManager->CreateObject(Class);
	}
}

effVOID * EFFCreateObject(const effString & className)
{
	EFFClass * Class = EFFGetClass(className);

	return EFFCreateObject(Class);
}

effVOID * EFFCreateObject(const ClassID & classId)
{
	EFFClass * Class = EFFGetClass(classId);

	return EFFCreateObject(Class);
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


EFFClass::~EFFClass()
{
	for ( effUINT i = 0; i < properties.size(); i++ )
	{
		SF_DELETE(properties[i]);
	}
	properties.clear();
	EFFUnRegisterClass(this);
}

EFFProperty * EFFClass::GetProperty(const effString & name)
{
	for ( effUINT i = 0; i < properties.size(); i++ )
	{
		if ( properties[i]->GetNameHash() == EFFStringHash(name) )
		{
			return properties[i];
		}
	}

	return NULL;
}

effVOID SetProperty(EFFProperty * addedProperty, effLONG offset, effLONG size, const effString & name)
{
	addedProperty->SetOffset(offset);
	addedProperty->SetSize(size);
	addedProperty->SetName(name);
	//addedProperty->SetClass(propertyClass);
	EFFClass * propertyClass = addedProperty->GetClass();

	/*if ( propertyClass != NULL && propertyClass->IsPODType() && !addedProperty->GetIsSTLContainer() )
	{
		if ( propertyClass->GetNameHash() == EFFStringHash(_effT("effString")) )
		{
			addedProperty->SetSavePropertyFP(SaveStringProperty);
		}
		else if ( propertyClass->GetNameHash() == EFFStringHash(_effT("int")) )
		{
			addedProperty->SetSavePropertyFP(SavePODProperty);
		}

	}*/


}


EFFBASE_END