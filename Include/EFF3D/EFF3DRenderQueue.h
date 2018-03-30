/******************************************************************************
	created:	2008-12-15   1:21
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DRenderQueue.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DRenderQueue_2008_12_15__
#define __EFF3DRenderQueue_2008_12_15__

#include "EFF3DRenderCommand.h"

EFF3D_BEGIN

class EFF3DRenderable;
class EFF3DDevice;
class EFF3DCamera;
class EFF3DSceneManager;
class EFF3DMaterial;
class EFF3DRenderCommandBuffer;

class EFF3DRenderQueue
{
public:
	union OpaqueRenderOrder
	{
		struct OrderBitsDefine
		{
			effWORD fullscreenLayer : 2,
			camera : 3,
			renderLayer : 5,
			transparencyType : 2,
			shader : 16,
			texture : 16,
			pass : 4,
			depth : 16;
		};
		OrderBitsDefine orderBits; 
		effUINT64 orderUINT64;
	};

	union TransparentRenderOrder
	{
		struct OrderBitsDefine
		{

			effWORD fullscreenLayer : 2,
			camera : 3,
			renderLayer : 5,
			transparencyType : 2,
			depth : 16,
			shader : 16,
			texture : 16,
			pass : 4;
		};
		OrderBitsDefine orderBits;
		effUINT64 orderUINT64;
	};


	struct RenderQueueElement
	{
		EFF3DRenderable *	renderable;
		effUINT64			renderOrder;
		EFF3DCamera *		camera;
		effINT				renderLayer;
	};
public:
    EFF3DRenderQueue();
	virtual ~EFF3DRenderQueue() {}
public:
	virtual effVOID						AddRenderable(EFF3DRenderable * renderable);
	virtual effVOID						RemoveRenderable(EFF3DRenderable * renderable);
	//virtual EFF3DIRenderable *	GetFirstRenderable() = 0;
	//virtual EFF3DIRenderable *	GetNextRenderable() = 0;
	virtual effVOID						Render(EFF3DDevice * device);

public:
	effVOID								SetSceneManager(EFF3DSceneManager * sceneManager) { this->sceneManager = sceneManager; }


protected:
	effVOID								Sort();
	EFF3DMaterial *						GetMaterial(RenderQueueElement & renderQueueElement);


protected:
	effVOID								CalculateRenderOrder(RenderQueueElement & renderQueueElement);
protected:
	VECTOR<RenderQueueElement>		    renderables;
	EFF3DSceneManager *					sceneManager;

    EFF3DRenderCommandBuffer *          createCommandBuffers;
    EFF3DRenderCommandBuffer *          destroyCommandBuffers;


    EFF3DCommandBucket<effUINT64> *     commandBuffer;

    EFFLinearAllocator *                allocator;
};

EFF3D_END

#endif