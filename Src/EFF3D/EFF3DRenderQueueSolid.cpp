/******************************************************************************
	created:	2008-12-15   1:58
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DRenderQueueGroupSolid.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DRenderQueueSolid.h"
#include "EFF3DRenderable.h"
#include "EFF3DMaterial.h"
#include "EFF3DAutoParamDataSource.h"
#include "EFF3DDevice.h"

//#define new EFFNEW

EFF3D_BEGIN

EFF3DRenderQueueSolid::EFF3DRenderQueueSolid()
{
}

EFF3DRenderQueueSolid::~EFF3DRenderQueueSolid()
{
}

effVOID EFF3DRenderQueueSolid::AddRenderable(EFF3DRenderable * renderable)
{
	RenderQueueElement rqe;
	rqe.renderable = renderable;

	renderables.push_back(rqe);
}

effVOID EFF3DRenderQueueSolid::RemoveRenderable(EFF3DRenderable * renderable)
{
	std::vector<RenderQueueElement>::iterator it = renderables.begin();

	for ( ; it != renderables.end(); it++ )
	{
		if ( (*it).renderable == renderable )
		{
			renderables.erase(it);
			return;
		}
	}
}

effVOID EFF3DRenderQueueSolid::Render(EFF3DDevice * device)
{
	/*EFF3DAutoParamDataSource * dataSource = device->GetAutoParamDataSource();
	EFF3DObject * lastObject = NULL;

	std::vector<EFF3DRenderable *>::iterator it,itend;
	itend = renderables.end();
	for ( it = renderables.begin(); it != itend; it++ )
	{
		EFF3DRenderable * renderable = *it;

		EFF3DMaterial * material = renderable->GetMaterial();


		if ( material != NULL )
		{
			//pMaterial->OnMaterialBeforeRender(pMaterial,pDevice);
		}

		renderable->Render(device);


		if ( material != NULL )
		{
			//pMaterial->OnMaterialEndRender(pMaterial,pDevice);
		}

	}*/

}

EFF3D_END