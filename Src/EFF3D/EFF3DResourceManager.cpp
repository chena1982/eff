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
#include "EFF3DSceneManager.h"
#include "EFF3DResourceManager.h"

//#define new EFFNEW

EFF3D_BEGIN

RTTI_IMPLEMENT(EFF3DResourceManager, 0)

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




effVOID EFF3DResourceManager::AddResource(EFF3DResource * res)
{
	effString originPath = res->GetOriginPath();
	effINT pos = originPath.rfind('\\');
	if ( pos != -1 )
	{
		originPath = originPath.substr(pos, originPath.length() - pos);
	}
	res->SetName(originPath);
	res->SetResourceManager(this);
	res->CalculateSize();
	memoryUsed += res->GetMemorySize();

	resources[res->GetName()] = res;
}

EFF3DResource * EFF3DResourceManager::GetResource(const effString & filePath)
{
	effString name;
	effINT pos = filePath.rfind('\\');
	if ( pos != -1 )
	{
		name = filePath.substr(pos, filePath.length() - pos);
	}

	ResourceMap::iterator it = resources.find(name);
	if ( it != resources.end() )
	{
		it->second->AddRef();
		return it->second;
	}

	return NULL;
}


EFF3DResource * EFF3DResourceManager::AsyncCreateFromFile(const effString & filePath, EFF3DRESOURCETYPE resourceType, EFF3DDevice * device)
{
	
	//BOOST_ASSERT(Class->IsKindOf(EFF3DResource::GetThisClass()));

	EFF3DResource * resource = GetResource(filePath);

	if ( resource != NULL )
	{
		return resource;
	}

	resource = device->CreateEmptyResource(resourceType);
	if ( resource == NULL )
	{
		return NULL;
	}

	AddResource(resource);

	effBOOL hr;
	device->GetSceneManager()->GetAsyncLoader()->AddWorkItem(resource, &hr);

	return resource;
}

EFF3D_END