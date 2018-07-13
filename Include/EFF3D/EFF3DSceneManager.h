/******************************************************************************
	created:	2012-03-10   00:04
	file path:	d:\EFF\trunk\EFFEngine\Include\EFF3D\EFF3DSceneManager.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DSceneManager_2012_03_10__
#define __EFF3DSceneManager_2012_03_10__


EFF3D_BEGIN

class EFF3DCamera;
class EFF3DFrustum;
class EFF3DRenderQueue;
class EFF3DAsyncLoader;
class EFF3DMaterial;
class EFF3DTerrain;
class EFF3DRenderable;

class EFF3DSceneFilter
{
public:
    virtual effBOOL IsVisible(EFFAabb & aabb) { return effTRUE; }
};

class EFF3DSceneFrustumFilter : public EFF3DSceneFilter
{
public:
    EFF3DSceneFrustumFilter(EFF3DFrustum * frustum);
    virtual ~EFF3DSceneFrustumFilter();
public:
    virtual effBOOL IsVisible(EFFAabb & aabb);
protected:
    EFF3DFrustum * frustum;
};

class EFF3DSceneManager
{
public:
	EFF3DSceneManager();
	~EFF3DSceneManager();

public:
	effVOID								AddCamera(EFF3DCamera * camera);
	effVOID								RemoveCamera(EFF3DCamera * camera);

	effVOID								SetCameraLayerMaterial(effINT cameraLayer, EFF3DMaterial * material);

	effBOOL								LoadTerrain(const effString & filePath);

public:
	inline VECTOR<EFF3DCamera *>		GetCameras() { return cameras; }
	inline EFF3DAsyncLoader *			GetAsyncLoader() { return asyncLoader; }
	inline EFF3DMaterial *				GetRenderLayerMaterial(effINT renderLayer) { return renderLayerMaterial[renderLayer]; }


    effVOID                             GetVisibleRenderable(EFF3DCamera * camera, VECTOR<EFF3DSceneFilter *> & filters, 
                                            VECTOR<EFF3DRenderable *> & result);



protected:

	VECTOR<EFF3DCamera *>				cameras;
	EFF3DRenderQueue *					renderQueue;
	EFF3DAsyncLoader *					asyncLoader;

	EFF3DMaterial *						renderLayerMaterial[CAMERA_MAX_RENDER_LAYER];

	EFF3DTerrain *						terrain;
};

EFF3D_END

#endif