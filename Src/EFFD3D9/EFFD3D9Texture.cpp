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

////#define new EFFNEW


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

effHRESULT EFFD3D9Texture::GetSurfaceLevel(effUINT level, EFF3DSurface ** surfaceLevel)
{
	assert(m_pTexture != NULL);

	EFFD3D9Surface * effD3D9Surface = EFFNEW EFFD3D9Surface();

	effHRESULT hr = m_pTexture->GetSurfaceLevel(level, &effD3D9Surface->m_pSurface);
	
	if ( FAILED(hr) ) 
	{
		SF_DELETE(effD3D9Surface);
		return hr;
	}

	effD3D9Surface->m_ImageInfo.surfaceType = GetFromTexture_Surface;
	effD3D9Surface->m_ImageInfo.textureLevel = level;

	//m_pDevice->m_pImageManager->AddFirstCreateResource(pSurface,std::string(_ucT("Surface")));

	*surfaceLevel = effD3D9Surface;
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

effVOID EFFD3D9Texture::CalculateSize()
{
}


effBOOL EFFD3D9Texture::Reload()
{
	if ( m_ImageInfo.pool == EFF3DPOOL_MANAGED )
	{
		return effTRUE;
	}
	return effTRUE;
}

effBOOL EFFD3D9Texture::Unload()
{
	SF_RELEASE(m_pTexture);
	return effTRUE;
}


effBOOL EFFD3D9Texture::LoadDataFromFile(const effString & strFilePath)
{
	SetOrigin(strFilePath);

	effTCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szDir);

	EFFSTLFile file;
	if ( !file.Open(_effT("../EditorRes/popup.png"), _effT("rb")) )
	{
		return effFALSE;
	}

	dataSize = file.Length();

	data = EFFNEW effBYTE[dataSize];
	
	file.Read(data, dataSize);
	file.Close();



	return effTRUE;
}

effBOOL EFFD3D9Texture::CreateRuntimeResource(EFF3DDevice * pDevice)
{
	HRESULT hr;

	EFFD3D9Device * pD3D9Device = (EFFD3D9Device *)pDevice;
	
	if ( FAILED(hr = D3DXCreateTextureFromFileInMemory(pD3D9Device->GetD3D9Device(), data, dataSize, &m_pTexture)) )
	{
		return effFALSE;
	}


	D3DSURFACE_DESC	desc;
	memset(&desc,0,sizeof(desc));
	m_pTexture->GetLevelDesc(0,&desc);

	m_ImageInfo.width = desc.Width;
	m_ImageInfo.height = desc.Height;
	m_ImageInfo.mipLevels = 0;
	m_ImageInfo.usage = desc.Usage;
	m_ImageInfo.format = (EFF3DFORMAT)desc.Format;
	m_ImageInfo.pool = (EFF3DPOOL)desc.Pool;

	return effTRUE;
}


effBOOL EFFD3D9Texture::Lock()
{
	return FAILED(m_pTexture->LockRect(0, &m_lockedRect, NULL, 0));
}


effBOOL EFFD3D9Texture::Unlock()
{
	return FAILED(m_pTexture->UnlockRect(0));
}

effBOOL EFFD3D9Texture::CopyDataToRuntimeResource()
{
	effBYTE * dest = (effBYTE *)m_lockedRect.pBits;
	effBYTE * src = data;
	for ( effUINT i = 0; i < m_ImageInfo.height; i++ )
	{
		effUINT size = sizeof(effUINT)*m_ImageInfo.width;
		memcpy(dest, src, size);
		dest += m_lockedRect.Pitch;
		src += size;
	}
	return effTRUE;
}