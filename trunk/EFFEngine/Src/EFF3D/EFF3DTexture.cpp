/******************************************************************************
	created:	2008-12-15   0:40
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTexture.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DTexture.h"
#include "EFF3DSurface.h"

#define new EFFNEW

EFF3D_BEGIN

EFF3DImageManager::EFF3DImageManager()
{
	//OnCreateFromFile += EFFEventCall(this,&EFF3DImageManager::CreateFromFileImpl);



	EFF3DIResource * pR = (EFF3DIResource *)(void *)0x8708;
	OnCreateFromFile(effString(_effT("ChenA")),EFF3DRTYPE_TEXTURE,pR);

}

EFF3DImageManager::~EFF3DImageManager()
{
}


effVOID EFF3DImageManager::CreateFromFileImpl(const effString * strFilePath,EFF3DRESOURCETYPE resourceType,EFF3DIResource * pR)
{
	if ( resourceType == EFF3DRTYPE_TEXTURE )
	{
		int z = 0;
	}
}

EFF3D_END