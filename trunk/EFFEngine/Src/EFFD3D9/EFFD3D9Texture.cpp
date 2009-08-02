/******************************************************************************
	created:	2008-12-14   20:35
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Texture.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFD3D9Texture.h"
#include "EFFD3D9Surface.h"

#define new EFFNEW


EFFD3D9Texture::EFFD3D9Texture()
{
	m_pTexture = NULL;
	m_pDevice = NULL;
}

EFFD3D9Texture::~EFFD3D9Texture()
{
	SF_RELEASE(m_pTexture);
}

effHRESULT EFFD3D9Texture::GetLevelDesc(effUINT Level,EFF3DSURFACE_DESC * pDesc)
{
	assert(m_pTexture != NULL);
	return m_pTexture->GetLevelDesc(Level,(D3DSURFACE_DESC *)pDesc);
}

effHRESULT EFFD3D9Texture::GetSurfaceLevel(effUINT Level,EFF3DSurface ** ppSurfaceLevel)
{
	assert(m_pTexture != NULL);

	EFFD3D9Surface * pSurface = new EFFD3D9Surface;

	effHRESULT hr = m_pTexture->GetSurfaceLevel(Level,&pSurface->m_pSurface);
	
	if ( FAILED(hr) ) 
	{
		SF_DELETE(pSurface);
		return hr;
	}

	pSurface->m_ImageInfo.SurfaceType = GetFromTexture_Surface;
	pSurface->m_ImageInfo.TextureLevel = Level;

	//m_pDevice->m_pImageManager->AddFirstCreateResource(pSurface,std::string(_ucT("Surface")));

	*ppSurfaceLevel = pSurface;
	return hr;
}

effHRESULT EFFD3D9Texture::LockRect(effUINT Level,EFF3DLOCKED_RECT * pLockedRect,const EFFRect * pRect,effDWORD Flags)
{
	assert(m_pTexture != NULL);
	return m_pTexture->LockRect(Level,(D3DLOCKED_RECT *)pLockedRect,(const RECT *)pRect,Flags);
}

effHRESULT EFFD3D9Texture::UnlockRect(effUINT Level)
{
	assert(m_pTexture != NULL);
	return m_pTexture->UnlockRect(Level);
}

effDWORD EFFD3D9Texture::CalculateSize()
{
	return 0;
}

effBOOL EFFD3D9Texture::LoadDataResourceFromFile(const effString & strFilePath)
{
	SetOrigin(strFilePath);
	return effTRUE;
}

effBOOL EFFD3D9Texture::CreateRuntimeResource()
{
	return effTRUE;
}

effBOOL EFFD3D9Texture::Reload()
{
	if ( m_ImageInfo.Pool == EFF3DPOOL_MANAGED )
	{
		return effTRUE;
	}
	return effTRUE;
}

effVOID EFFD3D9Texture::Unload()
{
	SF_RELEASE(m_pTexture);
}