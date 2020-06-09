/******************************************************************************
	created:	2012-03-10   00:04
	file path:	d:\EFF\trunk\EFFEngine\Src\EFF3D\EFF3DSceneManager.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "../../EFF3DPCH.h"

#include "EFF3DAsyncLoader.h"
#include "EFF3DDevice.h"
#include "EFF3DRenderQueue.h"
#include "EFF3DSceneManager.h"
#include "EFF3DTerrain.h"
#include "EFF3DCamera.h"
#include "EFF3DFrustum.h"

#include "../../Terrain/EFF3DTerrainDataPreprocess.h"

EFF3D_BEGIN

EFF3DSceneFrustumFilter::EFF3DSceneFrustumFilter(EFF3DFrustum * frustum)
{
    this->frustum = frustum;
}

EFF3DSceneFrustumFilter::~EFF3DSceneFrustumFilter()
{

}

effBOOL EFF3DSceneFrustumFilter::IsVisible(EFFAabb & aabb)
{
    return frustum->AABBInFrustum(aabb);
}

EFF3DSceneManager::EFF3DSceneManager()
{
	BOOST_ASSERT(EFF3DGetDevice() != NULL);
	asyncLoader = EFFNEW EFF3DAsyncLoader(EFF3DGetDevice(), 1);

	renderQueue = EFFNEW EFF3DRenderQueue();

	terrain = EFFNEW EFF3DTerrain();
}

EFF3DSceneManager::~EFF3DSceneManager()
{
	SF_DELETE(asyncLoader);
	SF_DELETE(renderQueue);
}


effBOOL EFF3DSceneManager::LoadTerrain(const effString & filePath)
{
	TerrainDataPreprocess(filePath, terrain);

	return effTRUE;
}

effVOID EFF3DSceneManager::GetVisibleRenderable(EFF3DCamera * camera, VECTOR<EFF3DSceneFilter *> & filters, 
    VECTOR<EFF3DRenderable *> & result)
{
    return;
}


EFF3D_END