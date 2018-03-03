/******************************************************************************
	created:	2008-12-17   0:51
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DResourceManager.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"

#include "EFF3DDevice.h"
#include "EFF3DResource.h"
#include "EFF3DAsyncLoader.h"
#include "EFF3DSceneManager.h"
#include "EFF3DResourceManager.h"

//#define new EFFNEW

EFF3D_BEGIN

RTTI_IMPLEMENT_PURE(EFF3DResourceManager, 0)

EFF3DResourceManager::EFF3DResourceManager()
{
	memoryUsed = 0;
	videoMemoryUsed = 0;
	AGPMemroyUsed = 0;
}

EFF3DResourceManager::~EFF3DResourceManager()
{
}


EFFId EFF3DResourceManager::CreateFromFile(const effString & filePath, EFF3DResourceType resourceType)
{

    EFF3DResource * resource = GetResource(filePath);

    if (resource != NULL)
    {
        return resource->id;
    }

    resource = EFF3DGetDevice()->CreateEmptyResource(resourceType);
    AddResource(resource);

    CreateFromFileImpl(filePath, resource, resourceType);


    return resource->id;
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



EFFId EFF3DResourceManager::AsyncCreateFromFile(const effString & filePath, EFF3DResourceType resourceType)
{
	
	//BOOST_ASSERT(Class->IsKindOf(EFF3DResource::GetThisClass()));

	EFF3DResource * resource = GetResource(filePath);

	if ( resource != NULL )
	{
		return resource->id;
	}

    EFF3DDevice * device = EFF3DGetDevice();

	resource = device->CreateEmptyResource(resourceType);
	AddResource(resource);

	effBOOL hr;
    device->GetSceneManager()->GetAsyncLoader()->AddWorkItem(resource, &hr);

	return resource->id;
}

effVOID EFF3DResourceManager::ForEach(boost::function<effVOID(EFF3DResource *, effVOID *)> visitor, effVOID * userData)
{
    EFF3DResource * resource = indices.GetFirst();
    while (resource != NULL)
    {
        visitor(resource, userData);

        resource = indices.GetNext();
    }
}


effVOID EFF3DResourceManager::AddResource(EFF3DResource * res)
{
    effString originPath = res->originPath;
    effSIZE pos = originPath.rfind('\\');
    if (pos != -1)
    {
        originPath = originPath.substr(pos, originPath.length() - pos);
    }

    res->name = originPath;
    //res->SetName(originPath);
    res->manager = this;
    //res->SetResourceManager(this);
    res->CalculateSize();
    memoryUsed += res->memorySize;

    resources[res->name] = res;
}

EFF3DResource * EFF3DResourceManager::GetResource(const effString & filePath)
{
    effString name;
    effSIZE pos = filePath.rfind('\\');
    if (pos != -1)
    {
        name = filePath.substr(pos, filePath.length() - pos);
    }

    ResourceMap::iterator it = resources.find(name);
    if (it != resources.end())
    {
        it->second->AddRef();
        return it->second;
    }

    return NULL;
}

EFF3D_END