/******************************************************************************
	created:	2008-12-15   2:55
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DRenderQueue.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"

#include "EFF3DMaterial.h"
#include "EFF3DShader.h"
#include "EFF3DRenderable.h"
#include "EFF3DCamera.h"
#include "EFF3DSceneManager.h"
#include  "EFF3DObject.h"
#include "EFF3DRenderQueue.h"
#include "EFF3DRenderCommandBuffer.h"
//#define new EFFNEW

EFF3D_BEGIN


EFF3DRenderQueue::EFF3DRenderQueue()
{
    commandBuffer = EFFNEW EFF3DCommandBucket<effUINT64>(2048);

    allocator = EFFNEW EFFLinearAllocator(EFF3D_CONFIG_MAX_COMMAND_BUFFER_SIZE);
}

EFF3DRenderQueue::~EFF3DRenderQueue()
{
    SF_DELETE(commandBuffer);
    SF_DELETE(allocator);
}

effVOID EFF3DRenderQueue::AddRenderable(EFF3DRenderable * renderable)
{

	
	/*for ( effUINT i = 0; i < sceneManager->GetCameras().size(); i++ )
	{
		EFF3DCamera * camera = sceneManager->GetCameras()[i];
		for ( effUINT j = 0; j < OBJECT_MAX_RENDER_LAYER; j++ )
		{
            //EFF3DObject * object = NULL;
			EFF3DObject * object = (EFF3DObject *)renderable->GetObject();
			if ( camera->GetLayerMask() && object->GetRenderLayer(j) )
			{
				RenderQueueElement rqe;
				rqe.renderable = renderable;
				rqe.camera = camera;
				rqe.renderLayer = object->GetRenderLayer(j);
				renderables.push_back(rqe);
			}
		}
	}*/

    EFF3DDrawIndexedCommand * command = commandBuffer->AddCommand<EFF3DDrawIndexedCommand>(0, sizeof(EFF3DDrawIndexedCommand), *allocator);

}

//删除的时候需要遍历，比较慢，所以在排序遍历的时候删除，这样就变成免费的了
effVOID EFF3DRenderQueue::RemoveRenderable(EFF3DRenderable * renderable)
{
	//effUINT64 order = 0;
	//renderables.erase(order);
}

bool sortByOrder(const EFF3DRenderQueue::RenderQueueElement & s1, const EFF3DRenderQueue::RenderQueueElement & s2)
{
	return s1.renderOrder < s2.renderOrder;
} 

effVOID EFF3DRenderQueue::Sort()
{
	VECTOR<RenderQueueElement>::iterator it = renderables.begin();
	for ( ; it != renderables.end(); it++ )
	{
		if ( !it->renderable->GetEnabled() )
		{
			//这里不直接删除，因为需要后面的元素朝前移，会造成内存拷贝，
			//用现在这个元素和末尾的元素交换，然后resize就行了
			std::swap(*it, renderables[renderables.size() - 1]);
			renderables.resize(renderables.size() - 1);
		}
		else
		{
			CalculateRenderOrder(*it);
		}
	}
	std::sort(renderables.begin(), renderables.end(), sortByOrder);
}

//因为渲染顺序是由EFF3DRenderQueue来决定的，所以计算渲染顺序放在这里，而不是放在EFF3DRenderable里
effVOID EFF3DRenderQueue::CalculateRenderOrder(RenderQueueElement & renderQueueElement)
{
	EFF3DRenderable * renderable = renderQueueElement.renderable;
	OpaqueRenderOrder order;
	if ( renderable->GetNeedRecalculateRenderOrder() )
	{
		order.orderBits.fullscreenLayer = renderable->GetFullscreenLayer();
		order.orderBits.camera = renderQueueElement.camera->GetDepth();
		order.orderBits.renderLayer = renderQueueElement.renderLayer;
		order.orderBits.transparencyType = renderable->GetTransparencyType();

		renderable->SetNeedRecalculateRenderOrder(effFALSE);
	}
	else
	{
		order.orderUINT64 = renderQueueElement.renderOrder;
		order.orderBits.renderLayer = renderQueueElement.renderLayer;
	}

	renderQueueElement.renderOrder = order.orderUINT64;
}

EFF3DMaterial *	EFF3DRenderQueue::GetMaterial(RenderQueueElement & renderQueueElement)
{
	EFF3DMaterial * material = sceneManager->GetRenderLayerMaterial(renderQueueElement.renderLayer);

	//如果render layer的material不为空，说明render layer的materia重载了renderable的material
	if ( material == NULL )
	{
		material = renderQueueElement.renderable->GetMaterial();
	}

	return material;
}

effVOID	EFF3DRenderQueue::Render(EFF3DDevice * device)
{
	EFF3DMaterial * lastMaterial = NULL;
	EFF3DShader * lastShader = NULL;
	EFF3DObject * lastObject = NULL;

	effUINT renderablesCount = (effUINT)renderables.size();
	for ( effUINT i = 0; i < renderablesCount; i++ )
	{
		RenderQueueElement & renderQueueElement = renderables[i];
		EFF3DRenderable * renderable = renderQueueElement.renderable;

        EFF3DObject * object = NULL;
		//EFF3DObject * object = (EFF3DObject *)renderable->GetObject();
		EFF3DAutoParamDataSource * autoParamDataSource = renderables[i].camera->GetAutoParamDataSource();


		EFF3DMaterial * material = GetMaterial(renderQueueElement);

		if ( material != lastMaterial )
		{
			lastMaterial = material;
			material->BeforeMaterialRender();
		}

		EFF3DShader * shader = material->GetShader();

		if ( shader != lastShader )
		{
			lastShader = shader;
			shader->UpdateAutoParametersPerShader(autoParamDataSource);
		}

		if ( object != lastObject )
		{
			lastObject = object;
			shader->UpdateAutoParametersPerObject(autoParamDataSource);
		}

		shader->UpdateAutoParametersPerRenderable(autoParamDataSource);
		renderable->Render(device);
		
		if ( i == renderablesCount || material != GetMaterial(renderQueueElement) )
		{
			material->AfterMaterialRender();
		}

	}
}




EFF3D_END