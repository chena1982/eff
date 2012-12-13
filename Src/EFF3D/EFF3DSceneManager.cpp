/******************************************************************************
	created:	2012-03-10   00:04
	file path:	d:\EFF\trunk\EFFEngine\Src\EFF3D\EFF3DSceneManager.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"

#include "EFF3DAsyncLoader.h"
#include "EFF3DDevice.h"
#include "EFF3DRenderQueue.h"
#include "EFF3DSceneManager.h"


EFF3D_BEGIN

EFF3DSceneManager::EFF3DSceneManager()
{
	BOOST_ASSERT(GetDevice() != NULL);
	asyncLoader = EFFNEW EFF3DAsyncLoader(GetDevice(), 1);

	renderQueue = EFFNEW EFF3DRenderQueue();
}

EFF3DSceneManager::~EFF3DSceneManager()
{
	SF_DELETE(asyncLoader);
	SF_DELETE(renderQueue);
}

EFF3D_END