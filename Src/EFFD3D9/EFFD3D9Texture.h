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
	effHRESULT								GetLevelDesc(effUINT Level,EFF3DSURFACE_DESC * pDesc);
	effHRESULT								GetSurfaceLevel(effUINT Level,EFF3DSurface ** ppSurfaceLevel);
	effHRESULT								LockRect(effUINT Level,EFF3DLOCKED_RECT * pLockedRect,const EFFRect * pRect,effUINT Flags);
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
	LPDIRECT3DTEXTURE9						m_pTexture;
	EFF3DDevice *							m_pDevice;
	D3DLOCKED_RECT							m_lockedRect;
};

#endif