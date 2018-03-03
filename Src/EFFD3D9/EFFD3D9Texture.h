/******************************************************************************
	created:	2008-12-14   20:32
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Texture.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9Texture_2008_12_14__
#define __EFFD3D9Texture_2008_12_14__



class EFFD3D9Texture : public EFF3DTexture
{
	friend class EFFD3D9Device;
	friend class EFFD3D9Shader;
public:
	EFFD3D9Texture();
	virtual ~EFFD3D9Texture();
public:
	effHRESULT								GetLevelDesc(effUINT Level, EFF3DSURFACE_DESC * pDesc);
	//effHRESULT								GetSurfaceLevel(effUINT Level, EFF3DSurface ** ppSurfaceLevel);
	effHRESULT								LockRect(effUINT Level, EFF3DLOCKED_RECT * pLockedRect, const EFFRect * pRect, effUINT Flags);
	effHRESULT								UnlockRect(effUINT Level);

	effVOID									CalculateSize();
	effBOOL									Reload();
	effBOOL									Unload();
protected:
	effBOOL									LoadDataFromFile(const effString & strFilePath);
	effBOOL									CreateRuntimeResource(EFF3DDevice * pDevice);
	effBOOL									Lock();
	effBOOL									Unlock();
	effBOOL									CopyDataToRuntimeResource();
protected:
	LPDIRECT3DTEXTURE9						texture;
	EFF3DDevice *							device;
	D3DLOCKED_RECT							lockedRect;
};

/*#define SHAREDTEXTURE_BUFFER_COUNT 3

class EFFD3D9SharedTexture : public EFF3DSharedTexture
{
    friend class EFFD3D9Device;
public:
    EFFD3D9SharedTexture();
    virtual ~EFFD3D9SharedTexture();
public:
    effHRESULT			                    GetSurfaceLevel(effUINT Level, EFF3DSurface ** ppSurfaceLevel);
    effVOID                                 GetSharedTextureInfo(SharedTextureInfo * sharedTextureInfo);
protected:
    effBOOL									LoadDataFromFile(const effString & strFilePath) { return effFALSE;  }


protected:
    LPDIRECT3DTEXTURE9						texture[SHAREDTEXTURE_BUFFER_COUNT];
    HANDLE                                  sharedHandle[SHAREDTEXTURE_BUFFER_COUNT];
    EFF3DDevice *							device;
    effINT                                  currentIndex;
};*/

#endif