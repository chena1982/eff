/******************************************************************************
	created:	2008-12-15   1:22
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DRenderQueueManager.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DRenderQueueManager.h"
#include "EFF3DRenderable.h"
#include "EFF3DRenderQueue.h"
#include "EFF3DRenderQueueSolid.h"
//#define new EFFNEW


EFF3D_BEGIN

EFF3DRenderQueueManager::EFF3DRenderQueueManager()
{
}

EFF3DRenderQueueManager::~EFF3DRenderQueueManager()
{
}

/*effVOID EFF3DRenderQueue::AddRenderable(EFF3DRenderable * renderable)
{
	for ( effINT i = 0; i < renderable->GetRenderQueueGroupCount(); i++ )
	{
		EFF3DRenderQueueGroup * group = GetRenderQueueGroup(renderable, i);
		if ( group != NULL)
		{
			group->AddRenderable(renderable);
		}
	}
}*/

EFF3DRenderQueue * EFF3DRenderQueueManager::CreateRenderQueue(effUINT renderQueueOrder, RENDER_QUEUE_TYPE renderQueueType, const effString & renderQueueName)
{
	MAP<effUINT, EFF3DRenderQueue *>::iterator it = renderQueues.find(renderQueueOrder);

	if ( it != renderQueues.end() )
	{
		return it->second;
	}
	
	//RQG还没有创建，创建一个并加到m_mapRQG里
	EFF3DRenderQueue * renderQueue = NULL;
	switch(renderQueueType)
	{
	case Solid:
		renderQueue = EFFNEW EFF3DRenderQueueSolid();
		break;
	default:
		return NULL;
	}


	renderQueues[renderQueueOrder] = renderQueue;
	return renderQueue;
}

/*effVOID EFF3DRenderQueue::RemoveRenderable(EFF3DRenderable * renderable)
{
	for ( effINT i = 0; i < renderable->GetRenderQueueGroupCount(); i++ )
	{
		EFF3DRenderQueueGroup * group = GetRenderQueueGroup(renderable,i);
		if ( group != NULL)
		{
			group->RemoveRenderable(renderable);
		}
	}
}*/

effVOID EFF3DRenderQueueManager::Render(EFF3DDevice * device)
{
	MAP<effUINT, EFF3DRenderQueue *>::iterator it = renderQueues.begin();
	for ( ; it != renderQueues.end(); it++ )
	{
		EFF3DRenderQueue * queue = it->second;
		queue->Render(device);
	}
}

EFF3DRenderQueue * EFF3DRenderQueueManager::GetRenderQueue(effUINT renderQueueOrder)
{
	MAP<effUINT, EFF3DRenderQueue *>::iterator it = renderQueues.find(renderQueueOrder);
	if ( it != renderQueues.end() )
	{
		return it->second;
	}

	return NULL;
}

EFF3D_END