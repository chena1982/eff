/******************************************************************************
	created:	2008-12-15   1:22
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DRenderQueue.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DRenderQueue.h"
#include "EFF3DIRenderable.h"
#include "EFF3DRenderQueueGroup.h"

#define new EFFNEW


EFF3D_BEGIN

EFF3DRenderQueue::EFF3DRenderQueue()
{
}

EFF3DRenderQueue::~EFF3DRenderQueue()
{
}

effVOID EFF3DRenderQueue::AddRenderable(EFF3DIRenderable * pRenderable)
{
	for ( effINT i = 0; i < pRenderable->GetRenderQueueGroupCount(); i++ )
	{
		EFF3DRenderQueueGroup * pGroup = GetRenderQueueGroup(pRenderable,i);
		if ( pGroup != NULL)
		{
			pGroup->AddRenderable(pRenderable);
		}
	}
}

EFF3DRenderQueueGroup * EFF3DRenderQueue::GetRenderQueueGroup(EFF3DIRenderable * pRenderable,effUINT uiRGQIndex)
{
	std::map<effUINT,EFF3DRenderQueueGroup *>::iterator it = m_mapRQG.find(pRenderable->GetRenderQueueGroupId(uiRGQIndex));

	//RQG已经创建过了
	if ( it != m_mapRQG.end() )
	{
		return it->second;
	}
	
	//RQG还没有创建，创建一个并加到m_mapRQG里
	EFF3DRenderQueueGroup * pGroup = (EFF3DRenderQueueGroup *)EFFCreateObject(pRenderable->GetRenderQueueGroupClassName(uiRGQIndex));
	if ( pGroup != NULL )
	{
		m_mapRQG[pRenderable->GetRenderQueueGroupId(uiRGQIndex)] = pGroup;
		return pGroup;
	}

	return NULL;
}

effVOID EFF3DRenderQueue::RemoveRenderable(EFF3DIRenderable * pRenderable)
{
	for ( effINT i = 0; i < pRenderable->GetRenderQueueGroupCount(); i++ )
	{
		EFF3DRenderQueueGroup * pGroup = GetRenderQueueGroup(pRenderable,i);
		if ( pGroup != NULL)
		{
			pGroup->RemoveRenderable(pRenderable);
		}
	}
}

effVOID EFF3DRenderQueue::Render(EFF3DDevice *pDevice, EFF3DAutoParamDataSource *pDataSource)
{
	std::map<effUINT,EFF3DRenderQueueGroup *>::iterator it = m_mapRQG.begin();
	for ( ; it != m_mapRQG.end(); it++ )
	{
		EFF3DRenderQueueGroup * pGroup = it->second;
		pGroup->Render(pDevice,pDataSource);
	}
}

EFF3D_END