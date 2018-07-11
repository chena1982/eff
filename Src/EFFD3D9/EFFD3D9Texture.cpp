/******************************************************************************
	created:	2008-12-14   20:35
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Texture.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D9PCH.h"


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

/*effHRESULT EFFD3D9Texture::GetLevelDesc(effUINT Level,EFF3DSURFACE_DESC * pDesc)
{
	assert(texture != NULL);
	return texture->GetLevelDesc(Level, (D3DSURFACE_DESC *)pDesc);
}*/

/*effHRESULT EFFD3D9Texture::GetSurfaceLevel(effUINT level, EFF3DSurface ** surfaceLevel)
{
	assert(texture != NULL);

	EFFD3D9Surface * effD3D9Surface = EFFNEW EFFD3D9Surface();

	effHRESULT hr = texture->GetSurfaceLevel(level, &effD3D9Surface->m_pSurface);
	
	if ( FAILED(hr) ) 
	{
		SF_DELETE(effD3D9Surface);
		return hr;
	}

	//effD3D9Surface->imageInfo.surfaceType = GetFromTexture_Surface;
	effD3D9Surface->imageInfo.textureLevel = level;

	//m_pDevice->m_pImageManager->AddFirstCreateResource(pSurface,std::string(_ucT("Surface")));

	*surfaceLevel = effD3D9Surface;
	return hr;
}*/

effHRESULT EFFD3D9Texture::LockRect(effUINT level, EFF3DLockedRect * lockedRect, const EFFRect * rect, effUINT flags)
{
	assert(texture != NULL);
	return texture->LockRect(level, (D3DLOCKED_RECT *)lockedRect, (const RECT *)rect, flags);
}

effHRESULT EFFD3D9Texture::UnlockRect(effUINT level)
{
	assert(texture != NULL);
	return texture->UnlockRect(level);
}

effVOID EFFD3D9Texture::CalculateSize()
{
}


effBOOL EFFD3D9Texture::Reload()
{
	/*if (imageInfo.pool == EFF3DPOOL_DEFAULT )
	{
		return effTRUE;
	}*/

	return effTRUE;
}

effBOOL EFFD3D9Texture::Unload()
{
	SF_RELEASE(texture);
	return effTRUE;
}


effBOOL EFFD3D9Texture::LoadDataFromFile(const effString & filePath)
{
	//SetOrigin(strFilePath);

    originPath = filePath;

	effTCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);

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

effBOOL EFFD3D9Texture::CreateRuntimeResource(EFF3DDevice * device)
{
	HRESULT hr;

	EFFD3D9Device * pD3D9Device = (EFFD3D9Device *)device;
	
	if ( FAILED(hr = D3DXCreateTextureFromFileInMemory(pD3D9Device->GetD3D9Device(), data, dataSize, &texture)) )
	{
		return effFALSE;
	}


	D3DSURFACE_DESC	desc;
	memset(&desc, 0, sizeof(desc));
	texture->GetLevelDesc(0, &desc);

    imageInfo.width = desc.Width;
    imageInfo.height = desc.Height;
    imageInfo.mipLevels = 0;
    //imageInfo.usage = desc.Usage;
    //imageInfo.format = (EFF3DFORMAT)desc.Format;
    //imageInfo.pool = (EFF3DPOOL)desc.Pool;

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
	for ( effUINT i = 0; i < imageInfo.height; i++ )
	{
		effUINT size = sizeof(effUINT)*imageInfo.width;
		memcpy(dest, src, size);
		dest += lockedRect.Pitch;
		src += size;
	}

	return effTRUE;
}

/*EFFD3D9SharedTexture::EFFD3D9SharedTexture()
{
    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        texture[i] = NULL;
        sharedHandle[i] = NULL;
    }

    device = NULL;
    currentIndex = 0;
}

EFFD3D9SharedTexture::~EFFD3D9SharedTexture()
{
    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        CloseHandle(sharedHandle[i]);
        SF_RELEASE(texture[i]);
    }
}


effHRESULT EFFD3D9SharedTexture::GetSurfaceLevel(effUINT level, EFF3DSurface ** surfaceLevel)
{
    assert(texture[currentIndex] != NULL);

    EFFD3D9Surface * effD3D9Surface = EFFNEW EFFD3D9Surface();

    effHRESULT hr = texture[currentIndex]->GetSurfaceLevel(level, &effD3D9Surface->m_pSurface);

    if (FAILED(hr))
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

effVOID EFFD3D9SharedTexture::GetSharedTextureInfo(SharedTextureInfo * sharedTextureInfo)
{

    EFF3DSharedTexture::GetSharedTextureInfo(sharedTextureInfo);

    sharedTextureInfo->width = m_ImageInfo.width;
    sharedTextureInfo->height = m_ImageInfo.height;
    sharedTextureInfo->format = (effUINT)m_ImageInfo.format;

    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        sharedTextureInfo->sharedTextureHandle[i] = (effDWORD)sharedHandle[i];
    }
}*/


effVOID EFFD3D9Texture::Commit(effUINT stage, effUINT textureFlags)
{
    effUINT flags = 0 == (EFF3D_TEXTURE_INTERNAL_DEFAULT_SAMPLER & textureFlags)
        ? textureFlags : imageInfo.flags;

    effUINT index = (flags & EFF3D_TEXTURE_BORDER_COLOR_MASK) >> EFF3D_TEXTURE_BORDER_COLOR_SHIFT;
    device->SetSamplerState(stage, flags);


    DX_CHECK(device->GetD3D9Device()->SetTexture(stage, texture));
    if (4 > stage)
    {
        DX_CHECK(device->GetD3D9Device()->SetTexture(D3DVERTEXTEXTURESAMPLER0 + stage, texture));
    }
}