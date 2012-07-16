/******************************************************************************
	created:	2008-12-15   2:23
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DAutoParamDataSource.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DAutoParamDataSource_2008_12_15__
#define __EFF3DAutoParamDataSource_2008_12_15__


EFF3D_BEGIN

class EFF3DRenderable;
class EFF3DCamera;

class EFF3DAutoParamDataSource
{
public:
	EFF3DAutoParamDataSource();
	~EFF3DAutoParamDataSource();



public:
	effVOID					SetRenderable(EFF3DRenderable * renderable);
	effVOID					SetCurrentCamera(EFF3DCamera * camera);
	effVOID					SetShadowDirLightExtrusionDistance(effFLOAT dist);
	effVOID					SetAmbientLightColor(const EFF3DCOLOR & ambient);

	EFFMatrix4 *			GetBonesMatrix();
	effINT					GetBonesMatrixCount();

	EFFMatrix4 &			GetWorldMatrix();
	EFFMatrix4 &			GetViewProjectionMatrix();
	EFFMatrix4 &			GetWorldViewProjMatrix();
	EFFMatrix4 &			GetReflectionWorldViewProjMatrix();
	EFFMatrix4 &			GetTextureProjMatrix();

	effFLOAT				GetShadowExtrusionDistance();
	EFFVector4 &			GetLightDirection();
	EFFVector4 &			GetCameraPosition();
	EFFVector4 &			GetCameraPositionObjectSpace();


	EFF3DCOLOR &			GetAmbientLightColor();



	effFLOAT				GetFPS();
	effFLOAT				GetViewportWidth();
	effFLOAT				GetViewportHeight();
	effFLOAT				GetInverseViewportWidth();
	effFLOAT				GetInverseViewportHeight();


protected:


	EFFMatrix4 *			bonesMatrix;
	effINT					bonesMatrixNum;

	EFFMatrix4				worldMatrix;
	EFFMatrix4				viewProjMatrix;
	EFFMatrix4				worldViewProjMatrix;
	EFFMatrix4				reflectionWorldViewProjMatrix;
	EFFMatrix4				textureProjMatrix;

	effFLOAT				dirLightExtrusionDistance;
	EFFVector4				lightDirection;
	EFFVector4				cameraPosition;
	EFFVector4				cameraPositionObjectSpace;





	effBOOL					boneMatrixDirty;
	effBOOL					worldMatrixDirty;
	effBOOL					viewProjMatrixDirty;
	effBOOL					worldViewProjMatrixDirty;
	effBOOL					reflectionWorldViewProjMatrixDirty;
	effBOOL					textureProjMatrixDirty;


	EFF3DCOLOR				ambientColor;

	EFF3DCamera *			currentCamera;
	EFF3DRenderable *		currentRenderable;
	//UC3DSkeleton *		m_pLastSkeleton;
	EFF3DRenderable *		lastUseBoneRenderable;
};

EFF3D_END

#endif