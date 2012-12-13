/******************************************************************************
	created:	2012-03-04   1:03
	file path:	d:\EFF\trunk\EFFEngine\Include\EFF3D\EFF3DCamera.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DCamera.h"
#include "EFF3DAutoParamDataSource.h"

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

EFF3D_END