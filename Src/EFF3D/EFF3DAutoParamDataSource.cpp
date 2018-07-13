/******************************************************************************
	created:	2008-12-15   2:24
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DAutoParamDataSource.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DAutoParamDataSource.h"
#include "EFF3DCamera.h"
////#define new EFFNEW

EFF3D_BEGIN

EFF3DAutoParamDataSource::EFF3DAutoParamDataSource()
{
	bonesMatrix = NULL;
	bonesMatrixNum = 0;



	dirLightExtrusionDistance = 0.0f;


	boneMatrixDirty = effTRUE;
	worldMatrixDirty = effTRUE;
	viewProjMatrixDirty = effTRUE;
	worldViewProjMatrixDirty = effTRUE;
	reflectionWorldViewProjMatrixDirty = effTRUE;
	textureProjMatrixDirty = effTRUE;


	currentCamera = NULL;
	currentRenderable = NULL;
	lastUseBoneRenderable = NULL;
}

EFF3DAutoParamDataSource::~EFF3DAutoParamDataSource()
{
}

EFFMatrix4 * EFF3DAutoParamDataSource::GetBonesMatrix()
{
	return bonesMatrix;
}

effINT EFF3DAutoParamDataSource::GetBonesMatrixCount()
{
	return bonesMatrixNum;
}

EFFMatrix4 & EFF3DAutoParamDataSource::GetWorldMatrix()
{
	return worldMatrix;
}

EFFMatrix4 & EFF3DAutoParamDataSource::GetViewProjectionMatrix()
{
	return viewProjMatrix;
}

EFFMatrix4 & EFF3DAutoParamDataSource::GetWorldViewProjMatrix()
{
	return worldViewProjMatrix;
}

EFFMatrix4 & EFF3DAutoParamDataSource::GetReflectionWorldViewProjMatrix()
{
	return reflectionWorldViewProjMatrix;
}

EFFMatrix4 & EFF3DAutoParamDataSource::GetTextureProjMatrix()
{
	return textureProjMatrix;
}

effFLOAT EFF3DAutoParamDataSource::GetShadowExtrusionDistance()
{
	return dirLightExtrusionDistance;
}

EFFVector4 & EFF3DAutoParamDataSource::GetLightDirection()
{
	return lightDirection;
}

EFFVector4 & EFF3DAutoParamDataSource::GetCameraPosition()
{
	return cameraPosition;
}

EFFVector4 &  EFF3DAutoParamDataSource::GetCameraPositionObjectSpace()
{
	return cameraPositionObjectSpace;
}

effVOID EFF3DAutoParamDataSource::UpdatePerFrameData()
{
    uniformBufferCamera.uniformBuffer.cameraPos = currentCamera->GetPostion();


}

EFF3D_END