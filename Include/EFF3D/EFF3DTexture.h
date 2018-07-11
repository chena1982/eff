/******************************************************************************
	created:	2008-12-12   23:56
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DTexture.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DTexture_2008_12_12__
#define __EFF3DTexture_2008_12_12__

#include "EFF3DResource.h"
#include "EFF3DResourceManager.h"

EFF3D_BEGIN


class EFF3D_API EFF3DImage : public EFF3DResource
{
	RTTI_DECLARE_PURE(EFF3DImage, EFF3DResource)
public:
	EFF3DImage() {}
	virtual ~EFF3DImage() {}
public:
	const EFF3DImageInfo & GetImageInfo() { return imageInfo; }
protected:
    EFF3DImageInfo imageInfo;
};


class EFF3DSurface;

class EFF3D_API EFF3DTexture : public EFF3DImage
{
	friend class EFF3DTextureManager;
protected:
	EFF3DTexture()
	{
        imageInfo.type = EFF3DResourceType_Texture2D;
        userData = NULL;
	}
	virtual ~EFF3DTexture() {};
public:
	//virtual effHRESULT			GetLevelDesc(effUINT Level, EFF3DSURFACE_DESC * pDesc) = 0;
	//virtual effHRESULT			GetSurfaceLevel(effUINT Level, EFF3DSurface ** ppSurfaceLevel) = 0;
	virtual effHRESULT			LockRect(effUINT level, EFF3DLockedRect * lockedRect, const EFFRect * rect, effUINT flag) = 0;
	virtual effHRESULT			UnlockRect(effUINT level) = 0;
public:
    effVOID * userData;
};

#define SHAREDTEXTURE_BUFFER_COUNT 3

class EFF3D_API EFF3DSharedTexture
{
    friend class EFF3DDevice;

protected:
    EFF3DSharedTexture();
    virtual ~EFF3DSharedTexture();

public:
	//virtual effHRESULT			GetSurfaceLevel(effUINT Level, EFF3DSurface ** ppSurfaceLevel) = 0;
    virtual effVOID				GetSharedTextureInfo(SharedTextureInfo * sharedTextureInfo);

    EFF3DTextureHandle          GetClientTexture();
    EFF3DTextureHandle          GetHostTexture(effINT index);


	effVOID						ClientWaitToStartRendering();
    effVOID                     NotifyClientStartRendering();

	effVOID                     HostWaitToStartRendering();
    effVOID                     NotifyHostStartRendering(effUINT renderTargetIndex);
protected:
	effVOID InitSemaphore();
protected:
    EFFSemaphore clientSemaphore;
    EFFSemaphore hostSemaphore;
	effBOOL host;

    EFF3DTextureHandle						textureHandle[SHAREDTEXTURE_BUFFER_COUNT];
    effSIZE                                 sharedHandle[SHAREDTEXTURE_BUFFER_COUNT];                          
    EFF3DDevice *							device;
    effINT                                  currentIndex;
};

class EFF3D_API EFF3DTextureManager : public EFF3DResourceManager
{
	friend class EFF3DDevice;
protected:
    EFF3DTextureManager();
	virtual ~EFF3DTextureManager();
public:

    const EFF3DImageInfo & GetImageInfo(EFF3DTextureHandle textureHandle);

    effBOOL LockRect(EFF3DTextureHandle textureHandle, effUINT level, EFF3DLockedRect * lockedRect, const EFFRect * rect, effUINT flag);
    effBOOL UnlockRect(EFF3DTextureHandle textureHandle, effUINT level);

	virtual effBOOL CreateFromFileImpl(const effString & filePath, EFF3DResource * resource, EFF3DResourceType resourceType);
protected:
	//inline effVOID	SetDevice(EFF3DDevice * device) { this->device = device; }
protected:
	//EFF3DDevice *	device;
};

EFF3D_END

#endif