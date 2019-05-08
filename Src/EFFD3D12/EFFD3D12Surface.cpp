/******************************************************************************
	created:	2008-12-14   20:56
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Surface.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D9PCH.h"
#include "EFFD3D9Surface.h"

//#define new EFFNEW

EFFD3D9Surface::EFFD3D9Surface()
{
	m_pSurface = NULL;
}

EFFD3D9Surface::~EFFD3D9Surface()
{
	SF_RELEASE(m_pSurface);
}

effHRESULT EFFD3D9Surface::GetDesc(EFF3DSURFACE_DESC * pDesc)
{
	assert(m_pSurface != NULL);
	return m_pSurface->GetDesc((D3DSURFACE_DESC *)pDesc);
}

effHRESULT EFFD3D9Surface::LockRect(EFF3DLOCKED_RECT * pLockedRect,const EFFRect * pRect,effUINT Flags)
{
	assert(m_pSurface != NULL);
	return m_pSurface->LockRect((D3DLOCKED_RECT *)pLockedRect,(const RECT *)pRect,Flags);
}

effHRESULT EFFD3D9Surface::UnlockRect()
{
	assert(m_pSurface != NULL);
	return m_pSurface->UnlockRect();
}

effVOID EFFD3D9Surface::CalculateSize()
{
}

effBOOL EFFD3D9Surface::LoadDataFromFile(const effString & strFilePath)
{
    originPath = strFilePath;
	//SetOrigin(strFilePath);
	return effTRUE;
}

effBOOL EFFD3D9Surface::CreateRuntimeResource(EFF3DDevice * pDevice)
{
	return effTRUE;
}

effBOOL EFFD3D9Surface::Reload()
{
	/*if ( imageInfo.pool == EFF3DPOOL_DEFAULT )
	{
		return effTRUE;
	}*/

	return effTRUE;
}

effBOOL EFFD3D9Surface::Unload()
{
	SF_RELEASE(m_pSurface);
	return effTRUE;
}

