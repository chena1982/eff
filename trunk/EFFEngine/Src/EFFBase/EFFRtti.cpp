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

//��ֱ��ʹ��ȫ�־�̬��������Ϊ�޷���֤ȫ�־�̬�����ĳ�ʼ��˳�������ľ�̬������mapEFFRunTimeTypeInfo�ȳ�ʼ�������������̬
//�ڹ��캯���������EFFRegisterClass����ô�����Crash
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

	//����Ӧ����pDigset�ֱ��ڴ��cpp�ļ����ͷţ�����md5��ʵ����û��ʵ���ͷŵĴ��룬�����������ͷţ���Ϊ�õ���ͬһ��new��Ҳ�������
	SFT_DELETE(pDigset);

	return classId;
}


EFFBASE_END