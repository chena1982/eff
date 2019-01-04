/******************************************************************************
	created:	2012-03-04   1:03
	file path:	d:\EFF\trunk\EFFEngine\Include\EFF3D\EFF3DCamera.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DCamera.h"
#include "EFF3DAutoParamDataSource.h"

//#include "D3dx9math.h"

EFF3D_BEGIN

RTTI_IMPLEMENT(EFF3DCamera, 0)


EFF3DCamera::EFF3DCamera()
{
	depth = 0;
	autoParamDataSource = EFFNEW EFF3DAutoParamDataSource();
}

EFF3DCamera::~EFF3DCamera()
{
	SF_DELETE(autoParamDataSource);
}

effVOID EFF3DCamera::SetViewParams(const EFFVector3 & eye, const EFFVector3 & lookAt)
{

	this->eye = eye;
	this->lookAt = lookAt;

	// Calc the view matrix
	EFFVector3 up(0.0f, 1.0f, 0.0f);
	EFFMatrixLookAtLH(&view, &eye, &lookAt, &up);

	//EFFMatrix4 result;
	//D3DXMatrixLookAtLH((D3DXMATRIX *)&result, (const D3DXVECTOR3 *)&eye, (const D3DXVECTOR3 *)&lookAt, (const D3DXVECTOR3 *)&up);
}

effVOID EFF3DCamera::SetProjParams(effFLOAT fov, effFLOAT aspect, effFLOAT nearPlane, effFLOAT farPlane)
{
	EFFMatrix4PerspectiveFovLH(&proj, fov, aspect, nearPlane, farPlane);

	//EFFMatrix4 result;
	//D3DXMatrixPerspectiveFovLH((D3DXMATRIX *)&result, fov, aspect, nearPlane, farPlane);
}

EFF3D_END