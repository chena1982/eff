/******************************************************************************
	created:	2008-12-17   0:51
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DResourceManager.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DDevice.h"
#include "EFF3DResource.h"
#include "EFF3DAsyncLoader.h"
#include "EFF3DResourceManager.h"


//#define new EFFNEW

EFF3D_BEGIN

EFF3DResourceManager::EFF3DResourceManager()
{
}

EFF3DResourceManager::~EFF3DResourceManager()
{
}

/*#define IMPL_CREATE_FROM_FILE(N)\
template<__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __NOTHING__)>\
EFF3DIResource * EFF3DResourceManager::CreateFromFile(const effString & strFilePath,__REPEAT(N, __ARG__, __COMMA__, __NOTHING__))\
{\
	ResourceMap::iterator it = m_mapResources.find(strFilePath);\
	if ( it != m_mapResources.end() )\
	{\
		it->second->AddRef();\
		return it->second;\
	}\
	EFF3DIResource * ret = CreateFromFileImpl(strFilePath, t0);\
	if ( ret != NULL )\
	{\
		AddFirstCreateResource(ret);\
	}\
	return ret;\
}

IMPL_CREATE_FROM_FILE(1)

IMPL_CREATE_FROM_FILE(2)*/




effVOID EFF3DResourceManager::AddFirstCreateResource(EFF3DResource * res)
{
	//res->SetObjectID(m_ulCurrentId);
	CalculateNextId();
	effString originPath = res->GetOriginPath();
	effINT pos = originPath.rfind('\\');
	if ( pos != -1 )
	{
		originPath = originPath.substr(pos, originPath.length() - pos);
		//strName += res->GetObjectID();
	}
	res->SetName(originPath);
	res->SetResourceManager(this);
	res->CalculateSize();
	memoryUsed += res->GetMemorySize();
	AddResource(res);
}

effVOID EFF3DResourceManager::AddResource(EFF3DResource * res)
{
	//ClassLevelLockable<EFF3DResourceManager>::Lock lock;
	resources[res->GetName()] = res;
	//resourcesId[res->GetObjectID()] = res;
}




EFF3DResource * EFF3DResourceManager::AsyncCreateFromFile(const effString & filePath, EFF3DRESOURCETYPE resourceType, EFF3DDevice * device)
{

	ResourceMap::iterator it = resources.find(filePath);
	if ( it != resources.end() )
	{
		it->second->AddRef();
		return it->second;
	}

	EFF3DResource * res = device->CreateEmptyResource(resourceType);
	if ( res == NULL )
	{
		return NULL;
	}

	effBOOL hr;
	device->GetAsyncLoader()->AddWorkItem(res, &hr);


	AddFirstCreateResource(res);
	

	return res;
}

EFF3D_END