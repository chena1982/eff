/******************************************************************************
	created:	2008-12-13   0:22
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DSurface.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DSurface_2008_12_13__
#define __EFF3DSurface_2008_12_13__

#include "EFF3DTexture.h"

EFF3D_BEGIN

class EFF3D_API EFF3DSurface : public EFF3DImage
{
public:
	EFF3DSurface()
	{
		m_ImageInfo.resourceType = EFF3DRTYPE_SURFACE;
		m_ImageInfo.surfaceType = Normal_Surface;
	}
	virtual ~EFF3DSurface() {}
public:
	virtual effHRESULT				GetDesc(EFF3DSURFACE_DESC * pDesc) = 0;
	virtual effHRESULT				LockRect(EFF3DLOCKED_RECT * pLockedRect,const EFFRect * pRect,effUINT Flags) = 0;
	virtual effHRESULT				UnlockRect() = 0;
};


EFF3D_END

#endif