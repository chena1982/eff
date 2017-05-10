/******************************************************************************
	created:	2008-12-15   0:40
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTexture.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DTexture.h"
#include "EFF3DSurface.h"
#include "EFF3DDevice.h"
//#define new EFFNEW

EFF3D_BEGIN

RTTI_IMPLEMENT_PURE(EFF3DImage, 0)


EFF3DSharedTexture::EFF3DSharedTexture()
{

}

EFF3DSharedTexture::~EFF3DSharedTexture()
{

}

EFF3DImageManager::EFF3DImageManager()
{
	//OnCreateFromFile += EFFEventCall(this, &EFF3DImageManager::CreateFromFileImpl);

	device = NULL;

	//EFF3DResource * pR = NULL;
	//effString s = _effT("ChenA");
	//OnCreateFromFile(_effT("ChenA"), EFF3DRTYPE_TEXTURE, pR);

}

EFF3DImageManager::~EFF3DImageManager()
{
}


EFF3DResource * EFF3DImageManager::CreateFromFile(const effString & filePath, EFF3DRESOURCETYPE resourceType)
{
	EFF3DResource * resource = GetResource(filePath);

	if ( resource != NULL )
	{
		return resource;
	}

	if ( resourceType == EFF3DRTYPE_TEXTURE )
	{
		EFF3DTexture * texture = NULL;
		if ( device->CreateTextureFromFile(filePath, &texture) )
		{
			AddResource(texture);
			return texture;
		}
	}
	return NULL;
}

EFF3D_END