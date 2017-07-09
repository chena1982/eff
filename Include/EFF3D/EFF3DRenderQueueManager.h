/******************************************************************************
	created:	2008-12-15   0:48
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DRenderQueueManager.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DRenderQueueManager_2008_12_15__
#define __EFF3DRenderQueueManager_2008_12_15__

EFF3D_BEGIN

class EFF3DRenderQueue;
class EFF3DRenderable;
class EFF3DDevice;

class EFF3DRenderQueueManager
{
	enum RENDER_QUEUE_TYPE
	{
		Solid,
		AlphaTest,
		AlphaBlend,
	};
public:
	EFF3DRenderQueueManager();
	~EFF3DRenderQueueManager();
public:
	//effVOID							AddRenderable(EFF3DRenderable * renderable);
	//effVOID							RemoveRenderable(EFF3DRenderable * renderable);
	effVOID						Render(EFF3DDevice * device);
	EFF3DRenderQueue *			GetRenderQueue(effUINT renderQueueOrder);
protected:
	EFF3DRenderQueue *			CreateRenderQueue(effUINT renderQueueOrder, RENDER_QUEUE_TYPE renderQueueType, const effString & renderQueueName = _effT(""));
protected:
	MAP<effUINT, EFF3DRenderQueue *>		renderQueues;
};

EFF3D_END

#endif