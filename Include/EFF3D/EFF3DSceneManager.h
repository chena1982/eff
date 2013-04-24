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
class EFF3DRenderQueue;
class EFF3DAsyncLoader;
class EFF3DMaterial;
class EFF3DTerrain;


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
	inline std::vector<EFF3DCamera *>	GetCameras() { return cameras; }
	inline EFF3DAsyncLoader *			GetAsyncLoader() { return asyncLoader; }
	inline EFF3DMaterial *				GetRenderLayerMaterial(effINT renderLayer) { return renderLayerMaterial[renderLayer]; }


protected:

	std::vector<EFF3DCamera *>			cameras;
	EFF3DRenderQueue *					renderQueue;
	EFF3DAsyncLoader *					asyncLoader;

	EFF3DMaterial *						renderLayerMaterial[CAMERA_MAX_RENDER_LAYER];

	EFF3DTerrain *						terrain;
};

EFF3D_END

#endif