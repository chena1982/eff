/******************************************************************************
	created:	2008-12-1   23:19
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFRtti.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFRtti.h"


#define new EFFNEW

EFFBASE_BEGIN

effCHAR AnsiUnicodeStringConvert::charBuffer[1024];
effWCHAR AnsiUnicodeStringConvert::wcharBuffer[1024];

const effCHAR * AnsiUnicodeStringConvert::W2A(const effWCHAR * str)
{
	effULONG length = WideCharToMultiByte(CP_OEMCP, NULL, str, -1, NULL, 0, NULL, FALSE);

	if( length > 1023 )
	{
		return NULL;
	}

	WideCharToMultiByte(CP_OEMCP, NULL, str, -1, &AnsiUnicodeStringConvert::charBuffer[0], length, NULL, FALSE);
	return charBuffer;
}

const effWCHAR * AnsiUnicodeStringConvert::A2W(const effCHAR * str)
{
	effULONG length = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
 
	if( length > 1023 )
	{
		return NULL;
	}

	MultiByteToWideChar(CP_ACP, 0, str, -1, &AnsiUnicodeStringConvert::wcharBuffer[0], length);
	return wcharBuffer;
}


RTTI_IMPLEMENT_POD(effString)
RTTI_IMPLEMENT_POD(effINT)
RTTI_IMPLEMENT_POD(effFLOAT)

//不直接使用全局静态变量，因为无法保证全局静态变量的初始化顺序，如果别的静态变量比mapEFFRunTimeTypeInfo先初始化，
//而且这个静态变量在构造函数里调用了EFFRegisterClass，那么程序会Crash
std::map<ClassID, EFFClass *> & GetRuntimeTypeInfoMap()
{
	static std::map<ClassID, EFFClass *> effRunTimeTypeInfo;
	return effRunTimeTypeInfo;
}

void EFFRegisterClass(EFFClass * pClass)
{
	GetRuntimeTypeInfoMap().insert(std::make_pair(pClass->GetID(), pClass));
}

void EFFUnRegisterClass(EFFClass * pClass)
{
	std::map<ClassID, EFFClass *>::iterator it = GetRuntimeTypeInfoMap().find(pClass->GetID());
	if ( it != GetRuntimeTypeInfoMap().end() )
	{
		GetRuntimeTypeInfoMap().erase(it);
	}
}

void * EFFCreateObject(const effString & className)
{
	return EFFCreateObject(ClassIDFromString(className));
}

void * EFFCreateObject(const ClassID & classId)
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






EFFBASE_END