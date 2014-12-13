/******************************************************************************
	created:	2008-12-14   20:35
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Texture.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D9PCH.h"

#include "EFFD3D9Surface.h"
#include "EFFD3D9Device.h"
#include "EFFD3D9Texture.h"

////#define new EFFNEW


EFFD3D9Texture::EFFD3D9Texture()
{
	texture = NULL;
	device = NULL;
}

EFFD3D9Texture::~EFFD3D9Texture()
{
	SF_RELEASE(texture);
}

effHRESULT EFFD3D9Texture::GetLevelDesc(effUINT Level,EFF3DSURFACE_DESC * pDesc)
{
	assert(texture != NULL);
	return texture->GetLevelDesc(Level, (D3DSURFACE_DESC *)pDesc);
}

effHRESULT EFFD3D9Texture::GetSurfaceLevel(effUINT level, EFF3DSurface ** surfaceLevel)
{
	assert(texture != NULL);

	EFFD3D9Surface * effD3D9Surface = EFFNEW EFFD3D9Surface();

	effHRESULT hr = texture->GetSurfaceLevel(level, &effD3D9Surface->m_pSurface);
	
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
	assert(texture != NULL);
	return texture->LockRect(Level,(D3DLOCKED_RECT *)pLockedRect,(const RECT *)pRect,Flags);
}

effHRESULT EFFD3D9Texture::UnlockRect(effUINT Level)
{
	assert(texture != NULL);
	return texture->UnlockRect(Level);
}

effVOID EFFD3D9Texture::CalculateSize()
{
}


effBOOL EFFD3D9Texture::Reload()
{
	if ( m_ImageInfo.pool == EFF3DPOOL_DEFAULT )
	{
		return effTRUE;
	}

	return effTRUE;
}

effBOOL EFFD3D9Texture::Unload()
{
	SF_RELEASE(texture);
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
	
	if ( FAILED(hr = D3DXCreateTextureFromFileInMemory(pD3D9Device->GetD3D9Device(), data, dataSize, &texture)) )
	{
		return effFALSE;
	}


	D3DSURFACE_DESC	desc;
	memset(&desc, 0, sizeof(desc));
	texture->GetLevelDesc(0, &desc);

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
	return FAILED(texture->LockRect(0, &lockedRect, NULL, 0));
}


effBOOL EFFD3D9Texture::Unlock()
{
	return FAILED(texture->UnlockRect(0));
}

effBOOL EFFD3D9Texture::CopyDataToRuntimeResource()
{
	effBYTE * dest = (effBYTE *)lockedRect.pBits;
	effBYTE * src = data;
	for ( effUINT i = 0; i < m_ImageInfo.height; i++ )
	{
		effUINT size = sizeof(effUINT)*m_ImageInfo.width;
		memcpy(dest, src, size);
		dest += lockedRect.Pitch;
		src += size;
	}

	return effTRUE;
}