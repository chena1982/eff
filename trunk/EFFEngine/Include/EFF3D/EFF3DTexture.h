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


class  EFF3D_API EFF3DImage : public EFF3DResource
{
public:
	EFF3DIMAGE_INFO & GetImageInfo() { return m_ImageInfo; }
protected:
	EFF3DIMAGE_INFO m_ImageInfo;
};

class EFF3D_API EFF3DBaseTexture : public EFF3DImage
{
public:

	EFF3DBaseTexture() {}
	virtual ~EFF3DBaseTexture() {}
};

class EFF3DSurface;

class EFF3D_API EFF3DTexture : public EFF3DBaseTexture
{
	friend class EFF3DTextureManager;
protected:
	EFF3DTexture()
	{
		m_ImageInfo.ResourceType = EFF3DRTYPE_TEXTURE;
	}
	virtual ~EFF3DTexture() {};
public:
	virtual effHRESULT			GetLevelDesc(effUINT Level,EFF3DSURFACE_DESC * pDesc) = 0;
	virtual effHRESULT			GetSurfaceLevel(effUINT Level,EFF3DSurface ** ppSurfaceLevel) = 0;
	virtual effHRESULT			LockRect(effUINT Level,EFF3DLOCKED_RECT * pLockedRect,const EFFRect * pRect,effUINT Flags) = 0;
	virtual effHRESULT			UnlockRect(effUINT Level) = 0;
};


class EFF3D_API EFF3DImageManager : public EFF3DResourceManager
{
	friend class EFF3DDevice;
protected:
	EFF3DImageManager();
	virtual ~EFF3DImageManager();
public:

	effVOID CreateFromFileImpl(const effString * strFilePath,EFF3DRESOURCETYPE resourceType,EFF3DIResource ** pR);
};

EFF3D_END

#endif