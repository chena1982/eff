/******************************************************************************
	created:	2008-12-15   0:40
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTexture.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "../EFF3DPCH.h"
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
        sharedHandle[i] = 0;
    }

    currentIndex = 0;
}

EFF3DSharedTexture::~EFF3DSharedTexture()
{
    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        EFF3DTexMgr->Release(textureHandle[i]);
    }
}

effVOID EFF3DSharedTexture::GetSharedTextureInfo(SharedTextureInfo * sharedTextureInfo)
{
    //lstrcpy(sharedTextureInfo->name, name.c_str());

    //EFF3DSharedTexture::GetSharedTextureInfo(sharedTextureInfo);

    const EFF3DImageInfo & imageInfo = EFF3DTexMgr->GetImageInfo(textureHandle[0]);
    sharedTextureInfo->width = imageInfo.width;
    sharedTextureInfo->height = imageInfo.height;
    sharedTextureInfo->format = imageInfo.format;

    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        const EFF3DImageInfo & imageInfo = EFF3DTexMgr->GetImageInfo(textureHandle[i]);
        sharedTextureInfo->sharedTextureHandle[i] = imageInfo.sharedHandle;
    }
}

EFF3DTextureHandle EFF3DSharedTexture::GetClientTexture()
{
    effINT result = currentIndex;

    currentIndex = (currentIndex + 1) % SHAREDTEXTURE_BUFFER_COUNT;

    return textureHandle[result];
}


EFF3DTextureHandle EFF3DSharedTexture::GetHostTexture(effINT index)
{
    return textureHandle[index];
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




EFF3DTextureManager::EFF3DTextureManager() : EFF3DResourceManager()
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

const EFF3DImageInfo &  EFF3DTextureManager::GetImageInfo(EFF3DTextureHandle textureHandle)
{
    EFF3DImage * image = (EFF3DImage *)indices[textureHandle.Index()];
    return image->GetImageInfo();
}

effBOOL EFF3DTextureManager::LockRect(EFF3DTextureHandle textureHandle, effUINT level, EFF3DLockedRect * lockedRect, const EFFRect * rect, effUINT flag)
{
    EFF3DTexture * texture = (EFF3DTexture *)indices[textureHandle.Index()];
    effHRESULT hr = texture->LockRect(level, lockedRect, rect, flag);
    return SUCCEEDED(hr);
}

effBOOL EFF3DTextureManager::UnlockRect(EFF3DTextureHandle textureHandle, effUINT level)
{
    EFF3DTexture * texture = (EFF3DTexture *)indices[textureHandle.Index()];
    effHRESULT hr = texture->UnlockRect(level);
    return SUCCEEDED(hr);
}

effBOOL EFF3DTextureManager::CreateFromFileImpl(const effString & filePath, EFF3DResource * resource, EFF3DResourceType resourceType)
{


	if ( resourceType == EFF3DResourceType_Texture2D )
	{
       /* effINT width, height, level;
        effVOID * srcData;
        effUINT srcDataSize;
        effUINT flag;
        EFF3DFormat format;
        EFF3DTextureHandle textureHandle;

        return EFF3DGetDevice()->CreateTextureFromMemory(srcData, srcDataSize, width, height, level, flag, format, resourceType, (EFF3DTexture *)resource);*/

        return effTRUE;
	}

	return effFALSE;
}

EFF3D_END