/******************************************************************************
	created:	2008-12-15   0:40
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTexture.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DTexture.h"
#include "EFF3DDevice.h"
//#define new EFFNEW

EFF3D_BEGIN

RTTI_IMPLEMENT_PURE(EFF3DImage, 0)


EFF3DSharedTexture::EFF3DSharedTexture()
{
	host = effFALSE;

    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        texture[i] = NULL;
        sharedHandle[i].id = EFF_INVALID_HANDLE;
    }

    currentIndex = 0;
}

EFF3DSharedTexture::~EFF3DSharedTexture()
{
    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        //EFF3DGetDevice()->GetImageManager()-
        //SF_DELETE(texture[i]);
    }
}

effVOID EFF3DSharedTexture::GetSharedTextureInfo(SharedTextureInfo * sharedTextureInfo)
{
    //lstrcpy(sharedTextureInfo->name, name.c_str());

    //EFF3DSharedTexture::GetSharedTextureInfo(sharedTextureInfo);

    sharedTextureInfo->width = texture[0]->GetImageInfo().width;
    sharedTextureInfo->height = texture[0]->GetImageInfo().height;
    sharedTextureInfo->format = (effUINT)texture[0]->GetImageInfo().format;

    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        sharedTextureInfo->sharedTextureHandle[i] = sharedHandle[i].id;
    }
}

EFF3DTexture * EFF3DSharedTexture::GetClientTexture()
{
    effINT result = currentIndex;

    currentIndex = (currentIndex + 1) % SHAREDTEXTURE_BUFFER_COUNT;

    return texture[result];
}


EFF3DTexture * EFF3DSharedTexture::GetHostTexture(effINT index)
{
    return texture[index];
}

effVOID EFF3DSharedTexture::ClientWaitToStartRendering()
{
    clientSemaphore.Wait();
}

effVOID EFF3DSharedTexture::NotifyClientStartRendering()
{
    clientSemaphore.Release(1);
}

effVOID EFF3DSharedTexture::HostWaitToStartRendering()
{
	hostSemaphore.Wait();
}

effVOID EFF3DSharedTexture::NotifyHostStartRendering(effUINT renderTargetIndex)
{
    EFF3DGetDevice()->OnNotifyHostStartRendering(renderTargetIndex);
    hostSemaphore.Release(1);
}

effVOID EFF3DSharedTexture::InitSemaphore()
{
	if (host)
	{
		hostSemaphore.Create(3, 3, _effT("SharedTextureHost"));
		clientSemaphore.Create(0, 1, _effT("SharedTextureClient"));
	}
}




EFF3DTextureManager::EFF3DTextureManager()
{
	//OnCreateFromFile += EFFEventCall(this, &EFF3DImageManager::CreateFromFileImpl);

	//device = NULL;

	//EFF3DResource * pR = NULL;
	//effString s = _effT("ChenA");
	//OnCreateFromFile(_effT("ChenA"), EFF3DRTYPE_TEXTURE, pR);

}

EFF3DTextureManager::~EFF3DTextureManager()
{
}


effBOOL EFF3DTextureManager::CreateFromFileImpl(const effString & filePath, EFF3DResource * resource, EFF3DResourceType resourceType)
{


	if ( resourceType == EFF3DResourceType_Texture2D )
	{
       /* effINT width, height, level;
        effVOID * srcData;
        effUINT srcDataSize;
        effUINT flag;
        EFF3DTextureFormat format;
        EFF3DTextureHandle textureHandle;

        return EFF3DGetDevice()->CreateTextureFromMemory(srcData, srcDataSize, width, height, level, flag, format, resourceType, (EFF3DTexture *)resource);*/

        return effTRUE;
	}

	return effFALSE;
}

EFF3D_END