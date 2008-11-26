#include "stdafx.h"

#include "EFFRtti.h"


#define new EFFNEW

static std::map<ClassID,EFFClass *>		g_mapEFFRunTimeTypeInfo;


void EFFRegisterClass(EFFClass * pClass)
{
	g_mapEFFRunTimeTypeInfo.insert(std::make_pair(pClass->GetID(),pClass));
}

void EFFUnRegisterClass(EFFClass * pClass)
{
	std::map<ClassID,EFFClass *>::iterator it = g_mapEFFRunTimeTypeInfo.find(pClass->GetID());
	if ( it != g_mapEFFRunTimeTypeInfo.end() )
	{
		g_mapEFFRunTimeTypeInfo.erase(it);
	}
}

void * EFFCreateObject(char * pszClassName)
{
	std::map<ClassID,EFFClass *>::iterator it = g_mapEFFRunTimeTypeInfo.find(ClassIDFromString(pszClassName));
	if ( it != g_mapEFFRunTimeTypeInfo.end() )
	{
		return it->second->CreateObject();
	}
	return NULL;
}

ClassID ClassIDFromString(char *string)
{

	MD5 context;
	unsigned int len = strlen(string);

	context.update((unsigned char *)string,len);
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
