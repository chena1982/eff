/******************************************************************************
	created:	2008-12-15   1:56
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DRenderQueueGroupSolid.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DRenderQueueGroupSolid_2008_12_15__
#define __EFF3DRenderQueueGroupSolid_2008_12_15__

#include "EFF3DRenderQueue.h"

EFF3D_BEGIN

class EFF3DMaterial;
class EFF3DDevice;
class EFF3DRenderable;

//这是默认的不透明物体的渲染队列
class EFF3DRenderQueueSolid : public EFF3DRenderQueue
{
public:
	EFF3DRenderQueueSolid();
	~EFF3DRenderQueueSolid();
public:
	virtual effVOID					AddRenderable(EFF3DRenderable * renderable);
	virtual effVOID					RemoveRenderable(EFF3DRenderable * renderable);
	virtual effVOID					Render(EFF3DDevice * device);
protected:

	//默认按照材质排序，如果有不同的需求，可以自己实现一个RenderQueueGroup

};

EFF3D_END

#endif