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


#define new EFFNEW

EFF3D_BEGIN

EFF3DResourceManager::EFF3DResourceManager()
{
	m_ulCurrentId = 0;
	m_bLastIdIsRecovered = effFALSE;
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




effVOID EFF3DResourceManager::AddFirstCreateResource(EFF3DIResource * res)
{
	res->SetObjectID(m_ulCurrentId);
	CalculateNextId();
	effString strName = res->GetOrigin();
	effINT nPos = strName.rfind('\\');
	if ( nPos != -1 )
	{
		strName = strName.substr(nPos,strName.length()-nPos);
		//strName += res->GetObjectID();
	}
	res->SetName(strName);
	res->SetResourceManager(this);
	m_dwMemoryUsage += res->CalculateSize();;
	AddResource(res);
}

effVOID EFF3DResourceManager::AddResource(EFF3DIResource * res)
{
	//ClassLevelLockable<EFF3DResourceManager>::Lock lock;
	m_mapResources[res->GetName()] = res;
	m_mapResourcesById[res->GetObjectID()] = res;
}

effVOID EFF3DResourceManager::CalculateNextId()
{
	if ( m_aryRecoveredId.size() == 0 )
	{
		m_ulCurrentId++;
		m_bLastIdIsRecovered = effTRUE;
		return;
	}
	if ( m_bLastIdIsRecovered )
	{
		m_aryRecoveredId.push_back(m_ulCurrentId);
	}

	m_bLastIdIsRecovered = effFALSE;
	m_ulCurrentId = m_aryRecoveredId[0];
	m_aryRecoveredId.erase(m_aryRecoveredId.begin());
}


EFF3DIResource * EFF3DResourceManager::AsyncCreateFromFile(const effString & strFilePath,EFF3DRESOURCETYPE resourceType,EFF3DDevice * pDevice)
{

	ResourceMap::iterator it = m_mapResources.find(strFilePath);
	if ( it != m_mapResources.end() )
	{
		it->second->AddRef();
		return it->second;
	}

	EFF3DIResource * pRes = pDevice->CreateEmptyResource(resourceType);
	if ( pRes == NULL )
	{
		return NULL;
	}

	effHRESULT hr;
	pDevice->GetAsyncLoader()->AddWorkItem(pRes,&hr);


	AddFirstCreateResource(pRes);
	

	return pRes;
}

EFF3D_END