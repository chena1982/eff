/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\EFFEngine\Src\EFFD3D12\EFFD3D12Texture.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D12Texture_2019_03_23__
#define __EFFD3D12Texture_2019_03_23__



class EFFD3D12Texture : public EFF3DTexture
{
	friend class EFFD3D12Device;
	friend class EFFD3D12Shader;
public:
	EFFD3D12Texture();
	virtual ~EFFD3D12Texture();
public:
	//effHRESULT								GetLevelDesc(effUINT Level, EFF3DSURFACE_DESC * pDesc);
	//effHRESULT								GetSurfaceLevel(effUINT Level, EFF3DSurface ** ppSurfaceLevel);
	effHRESULT								LockRect(effUINT level, EFF3DLockedRect * lockedRect, const EFFRect * rect, effUINT flags);
	effHRESULT								UnlockRect(effUINT level);

	effVOID									CalculateSize();
	effBOOL									Reload();
	effBOOL									Unload();

    effVOID                                 Commit(effUINT stage, effUINT textureFlags, const float _palette[][4]);
protected:
	effBOOL									LoadDataFromFile(const effString & filePath);
	effBOOL									CreateRuntimeResource(EFF3DDevice * device);
	effBOOL									Lock();
	effBOOL									Unlock();
	effBOOL									CopyDataToRuntimeResource();
protected:
	//LPDIRECT3DTEXTURE9						texture;
	EFFD3D12Device *						device;
	//D3DLOCKED_RECT							lockedRect;
};

/*#define SHAREDTEXTURE_BUFFER_COUNT 3

class EFFD3D9SharedTexture : public EFF3DSharedTexture
{
    friend class EFFD3D12Device;
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