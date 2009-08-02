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

//不直接使用全局静态变量，因为无法保证全局静态变量的初始化顺序，如果别的静态变量比mapEFFRunTimeTypeInfo先初始化，而且这个静态
//在构造函数里调用了EFFRegisterClass，那么程序会Crash
std::map<ClassID,EFFClass *> & GetRuntimeTypeInfoMap()
{
	static std::map<ClassID,EFFClass *>		mapEFFRunTimeTypeInfo;
	return mapEFFRunTimeTypeInfo;
}



void EFFRegisterClass(EFFClass * pClass)
{
	GetRuntimeTypeInfoMap().insert(std::make_pair(pClass->GetID(),pClass));
}

void EFFUnRegisterClass(EFFClass * pClass)
{
	std::map<ClassID,EFFClass *>::iterator it = GetRuntimeTypeInfoMap().find(pClass->GetID());
	if ( it != GetRuntimeTypeInfoMap().end() )
	{
		GetRuntimeTypeInfoMap().erase(it);
	}
}

void * EFFCreateObject(const char * pszClassName)
{
	return EFFCreateObject(ClassIDFromString(pszClassName));
}

void * EFFCreateObject(const ClassID & classID)
{
	std::map<ClassID,EFFClass *>::iterator it = GetRuntimeTypeInfoMap().find(classID);
	if ( it != GetRuntimeTypeInfoMap().end() )
	{
		return it->second->CreateObject();
	}
	return NULL;
}

ClassID ClassIDFromString(const char * szClassName)
{

	MD5 context;
	unsigned int len = strlen(szClassName);

	context.update((unsigned char *)szClassName,len);
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

	//本来应该在pDigset分别内存的cpp文件里释放，但是md5的实现里没有实现释放的代码，所以在这里释放，因为用的是同一个new，也不会出错。
	SFT_DELETE(pDigset);

	return classId;
}


EFFBASE_END