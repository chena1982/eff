/******************************************************************************
	created:	2008-12-14   20:35
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Texture.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"

#include "EFFD3D9Surface.h"
#include "EFFD3D9Device.h"
#include "EFFD3D9Texture.h"

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

effHRESULT EFFD3D9Texture::LockRect(effUINT Level,EFF3DLOCKED_RECT * pLockedRect,const EFFRect * pRect,effUINT Flags)
{
	assert(m_pTexture != NULL);
	return m_pTexture->LockRect(Level,(D3DLOCKED_RECT *)pLockedRect,(const RECT *)pRect,Flags);
}

effHRESULT EFFD3D9Texture::UnlockRect(effUINT Level)
{
	assert(m_pTexture != NULL);
	return m_pTexture->UnlockRect(Level);
}

effUINT EFFD3D9Texture::CalculateSize()
{
	return 0;
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


effBOOL EFFD3D9Texture::LoadDataFromFile(const effString & strFilePath)
{
	SetOrigin(strFilePath);

	effTCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szDir);

	EFFSTLFile file;
	if ( !file.Open("../EditorRes/popup.png","rb") )
	{
		return effFALSE;
	}

	m_uiDataSize = file.Length();

	m_pData = new effBYTE[m_uiDataSize];
	
	file.Read(m_pData,m_uiDataSize);
	file.Close();



	return effTRUE;
}

effBOOL EFFD3D9Texture::CreateRuntimeResource(EFF3DDevice * pDevice)
{
	HRESULT hr;

	EFFD3D9Device * pD3D9Device = (EFFD3D9Device *)pDevice;
	
	if ( FAILED(hr = D3DXCreateTextureFromFileInMemory(pD3D9Device->GetD3DDevice(),m_pData,m_uiDataSize,&m_pTexture)) )
	{
		return effFALSE;
	}


	D3DSURFACE_DESC	desc;
	memset(&desc,0,sizeof(desc));
	m_pTexture->GetLevelDesc(0,&desc);

	m_ImageInfo.Width = desc.Width;
	m_ImageInfo.Height = desc.Height;
	m_ImageInfo.MipLevels = 0;
	m_ImageInfo.Usage = desc.Usage;
	m_ImageInfo.Format = (EFF3DFORMAT)desc.Format;
	m_ImageInfo.Pool = (EFF3DPOOL)desc.Pool;

	return effTRUE;
}


effHRESULT EFFD3D9Texture::Lock()
{
	return m_pTexture->LockRect(0,&m_lockedRect,NULL,0);
}


effHRESULT EFFD3D9Texture::Unlock()
{
	return m_pTexture->UnlockRect(0);
}

effBOOL EFFD3D9Texture::CopyDataToRuntimeResource()
{
	effBYTE * pDest = (effBYTE *)m_lockedRect.pBits;
	effBYTE * pSrc = m_pData;
	for ( effUINT i = 0; i < m_ImageInfo.Height; i++ )
	{
		effUINT uiSize = sizeof(effUINT)*m_ImageInfo.Width;
		memcpy(pDest,pSrc,uiSize);
		pDest += m_lockedRect.Pitch;
		pSrc+= uiSize;
	}
	return effTRUE;
}