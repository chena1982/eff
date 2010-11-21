/******************************************************************************
	created:	2008-12-14   20:53
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Surface.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9Surface_2008_12_14__
#define __EFFD3D9Surface_2008_12_14__



class EFFD3D9Surface : public EFF3DSurface
{
	friend class EFFD3D9Texture;
	friend class EFFD3D9Device;
public:
	EFFD3D9Surface();
	virtual ~EFFD3D9Surface();
public:
	virtual effHRESULT					GetDesc(EFF3DSURFACE_DESC * pDesc);
	virtual effHRESULT					LockRect(EFF3DLOCKED_RECT * pLockedRect,const EFFRect * pRect,effUINT Flags);
	virtual effHRESULT					UnlockRect();

	effUINT									CalculateSize();
	effBOOL									Reload();
	effVOID									Unload();

protected:
	effBOOL									LoadDataFromFile(const effString & strFilePath);
	effBOOL									CreateRuntimeResource(EFF3DDevice * pDevice);
protected:
	LPDIRECT3DSURFACE9				m_pSurface;
};

#endif