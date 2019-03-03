/******************************************************************************
	created:	2008-12-2   0:21
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Device.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D9PCH.h"
#include "EFFD3D9Device.h"
#include "EFFD3D9Texture.h"
#include "EFFD3D9IndexBuffer.h"
#include "EFFD3D9VertexBuffer.h"
#include "EFFD3D9VertexDeclaration.h"
#include "EFFD3D9Shader.h"
#include "EFFD3D9Query.h"

#include "murmur3/murmur3.h"
//#define new EFFNEW


effVOID InitWindow(effINT width, effINT height, D3DPRESENT_PARAMETERS *d3dpp, effBOOL enableAA, effUINT depthStencilFormat)  
{
	d3dpp->Windowed = TRUE;

	if ( GetOSVersion() == Win7 )
	{
		d3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;
		//d3dpp->SwapEffect = D3DSWAPEFFECT_OVERLAY;
		//d3dpp->SwapEffect = D3DSWAPEFFECT_FLIPEX;
		//d3dpp->BackBufferCount = 2;
	}
	else
	{
		d3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;
	}

	d3dpp->BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp->BackBufferWidth = width;
	d3dpp->BackBufferHeight = height;
	d3dpp->AutoDepthStencilFormat = s_textureFormat[depthStencilFormat].d3d9Format;



	if ( enableAA )
	{
		//d3dpp->MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
	}

	d3dpp->EnableAutoDepthStencil = TRUE;
	d3dpp->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

effVOID InitFullScreen(effINT width, effINT height, D3DPRESENT_PARAMETERS * d3dpp, HWND hWnd)
{

	d3dpp->Windowed = effFALSE;
	d3dpp->BackBufferCount = 2;
	d3dpp->BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp->BackBufferWidth = width;
	d3dpp->BackBufferHeight = height;
	d3dpp->hDeviceWindow = hWnd;

	d3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp->AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp->EnableAutoDepthStencil = TRUE;
}


effBOOL effCreate3DDevice(EFF3DDevice ** eff3DDevice, effBOOL window, HWND hWnd, effINT width, effINT height)
{

	LPDIRECT3D9EX D3D;
	if ( FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &D3D)) )
	{
		return effFALSE;
	}

	EFFD3D9Device * D3DDevice = EFFNEW EFFD3D9Device;
	D3DDevice->SetD3D9(D3D);
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));



	UINT AdapterToUse = D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;
#ifdef SHIPPING_VERSION
	// When building a shipping version, disable NVPerfHUD (opt-out)
#else
	// Look for 'NVIDIA NVPerfHUD' adapter
	// If it is present, override default settings
	for ( UINT Adapter = 0; Adapter < D3D->GetAdapterCount(); Adapter++ )
	{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT Res;
		Res = D3D->GetAdapterIdentifier(Adapter, 0, &Identifier);

		if ( strcmp(Identifier.Description, "NVIDIA PerfHUD") == 0 )
		{
			AdapterToUse = Adapter;
			DeviceType = D3DDEVTYPE_REF;
			break;
		}
	}
#endif


	D3DADAPTER_IDENTIFIER9 Identifier;

	D3D->GetAdapterIdentifier(AdapterToUse, 0, &Identifier);
	//pD3DDevice->m_strDeviceName = Identifier.Description;

	D3DCAPS9 d3dCaps;
	D3D->GetDeviceCaps(AdapterToUse, DeviceType, &d3dCaps);
	effUINT behaviorFlags;
	if ( (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 )
	{
		behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else
	{
		behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}


	if ( !window )
	{
		InitFullScreen(width, height, &d3dpp, hWnd);


		if (FAILED(D3D->CreateDeviceEx(AdapterToUse, DeviceType, hWnd, behaviorFlags, &d3dpp, NULL, &D3DDevice->GetD3D9Device())))
		{
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;

			//如果创建D3DFMT_X8R8G8B8缓冲模式失败，试试D3DFMT_X1R5G5B5模式
			if (FAILED(D3D->CreateDeviceEx(AdapterToUse, DeviceType, hWnd, behaviorFlags, &d3dpp, NULL, &D3DDevice->GetD3D9Device())))
			{
				SF_RELEASE(D3DDevice);
				return effFALSE;
			}
		}
	}
	else
	{
		InitWindow(width, height, &d3dpp, effFALSE, (effUINT)EFF3D_TEXTURE_FORMAT_D24S8);

		if( FAILED(D3D->CreateDeviceEx(AdapterToUse, DeviceType, hWnd, behaviorFlags, &d3dpp, NULL, &D3DDevice->GetD3D9Device())) )
		{
			SF_RELEASE(D3DDevice);
			return effFALSE;
		}
	}

	IDirect3DSurface9 * backBuffer = NULL;
	if ( FAILED(D3DDevice->GetD3D9Device()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer)) )
	{
		SF_RELEASE(D3DDevice);
		return effFALSE;
	};

	D3DSURFACE_DESC backBufferDesc;
	backBuffer->GetDesc(&backBufferDesc);
	D3DDevice->SetBackBufferSize(backBufferDesc.Width, backBufferDesc.Height);
	SF_RELEASE(backBuffer);

	//CGcontext cgContext = cgCreateContext();
	//D3DDevice->SetCGContex(cgContext);
	//cgD3D9SetDevice(D3DDevice->GetD3D9Device());

	*eff3DDevice =	D3DDevice;

	return effTRUE;
}

EFFD3D9Device::EFFD3D9Device()
{
	D3D9 = NULL;       
	D3D9Device = NULL;
	//cgContext = NULL;
}

#ifdef __cplusplus
#define XXX
#endif

EFFD3D9Device::~EFFD3D9Device()
{
	//cgD3D9SetDevice(NULL);
	//cgDestroyContext(cgContext);

	SF_RELEASE(D3D9Device);
	SF_RELEASE(D3D9);
}


effBOOL EFFD3D9Device::BeginScene()
{
	const effUINT64 primitiveType = renderDebugFlags & EFF3D_DEBUG_WIREFRAME ? EFF3D_STATE_PT_LINES : 0;
	effBYTE primitiveIndex = effBYTE(primitiveType >> EFF3D_STATE_PT_SHIFT);
	primitiveInfo = s_primInfo[primitiveIndex];

	currentDrawCommand.Clear();
	currentDrawCommand.stateFlags = EFF3D_STATE_NONE;
	currentDrawCommand.stencil = PackStencil(EFF3D_STENCIL_NONE, EFF3D_STENCIL_NONE);

	// invalidate sampler state
	for (effUINT stage = 0; stage < EFF3D_CONFIG_MAX_TEXTURE_SAMPLERS; ++stage)
	{
		samplerFlags[stage] = UINT32_MAX;
	}

	return SUCCEEDED(D3D9Device->BeginScene());
}

effBOOL EFFD3D9Device::EndScene()
{
	return SUCCEEDED(D3D9Device->EndScene());
}

effBOOL EFFD3D9Device::Clear(effUINT count, const EFFRect * rects, effUINT flags, EFF3DCOLOR color, effFLOAT z, effUINT stencil)
{
	return SUCCEEDED(D3D9Device->Clear(count,(const D3DRECT *)rects, flags, color, z, stencil));
}

effBOOL EFFD3D9Device::Present(const EFFRect * sourceRect, const EFFRect * destRect)
{
	return SUCCEEDED(D3D9Device->PresentEx((const RECT *)sourceRect, (const RECT *)destRect, NULL, NULL, 0));
}

effVOID UnloadTexture(EFF3DResource * resource, effVOID * userData)
{

    EFF3DImage * image = (EFF3DImage *)resource;
    const EFF3DImageInfo & imageInfo = image->GetImageInfo();

    /*if (imageInfo.pool == EFF3DPOOL_DEFAULT)
    {
        image->Unload();
        return;
    }*/

    if (imageInfo.type == EFF3DResourceType_Texture2D)
    {
        /*if (imageInfo.surfaceType == DepthStencil_Surface || imageInfo.surfaceType == GetFromTexture_Surface)
        {
            EFF3DSurface * surface = (EFF3DSurface *)resource;
            surface->Unload();
            return;
        }*/

        /*if ( imageInfo.surfaceType == RenderTo_Surface )
        {
        EFF3DRenderToSurface * renderToSurface = reinterpret_cast<EFF3DRenderToSurface *>(it->second);
        renderToSurface->UnloadResource();
        continue;
        }*/

    }
    //it->second->UnloadResource();
   
}

effVOID ReloadTexture(EFF3DResource * resource, effVOID * userData)
{
    EFF3DImage * image = (EFF3DImage *)resource;
    if (image->isUnloaded)
    {
        if (!image->Reload())
        {
            return;
        }
    }
}

effBOOL EFFD3D9Device::Reset(effBOOL window, effINT width, effINT height)
{
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	IDirect3DSurface9 * pDSSurface = NULL;
	D3D9Device->GetDepthStencilSurface(&pDSSurface);
	D3DSURFACE_DESC DSSurfaceDesc;
	memset(&DSSurfaceDesc, 0, sizeof(D3DSURFACE_DESC));
	pDSSurface->GetDesc(&DSSurfaceDesc);

	InitWindow(width, height, &d3dpp, effFALSE, (effUINT)DSSurfaceDesc.Format);
	SF_RELEASE(pDSSurface);


    GetTextureManager()->ForEach(UnloadTexture, NULL);

	/*UCFontMap::iterator itFont = mapFont.begin();
	for ( ; itFont != mapFont.end(); itFont++ )
	{
		UCD3DPlaneFont * pPlaneFont = (UCD3DPlaneFont *)itFont->second;
		if ( pPlaneFont->pFont != NULL )
		{
			pPlaneFont->pFont->OnLostDevice();
		}
	}*/

	//GetEffectManager()->OnLostDevice();

	//m_BeforeReset(this,NULL);

	HRESULT hr;
	if ( FAILED(hr = D3D9Device->Reset(&d3dpp)) )
	{
		return effFALSE;
	}


    GetTextureManager()->ForEach(ReloadTexture, NULL);

	//m_EndReset(this,NULL);

	//GetEffectManager()->OnResetDevice();

	/*itFont = mapFont.begin();
	for ( ; itFont != mapFont.end(); itFont++ )
	{
		UCD3DPlaneFont * pPlaneFont = (UCD3DPlaneFont *)itFont->second;
		if ( pPlaneFont->pFont != NULL )
		{
			pPlaneFont->pFont->OnResetDevice();
		}
	}

	if ( m_pLine != NULL )
	{
		m_pLine->OnResetDevice();
	}*/



	IDirect3DSurface9 * backBuffer = NULL;
	if ( FAILED(D3D9Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer)) )
	{
		return effFALSE;
	};

	D3DSURFACE_DESC backBufferDesc;
	backBuffer->GetDesc(&backBufferDesc);
	width = backBufferDesc.Width;
	height = backBufferDesc.Height;
	SF_RELEASE(backBuffer);

	return effTRUE;
}

EFF3DResource * EFFD3D9Device::CreateEmptyResourceImpl(EFF3DResourceType resourceType)
{
	EFF3DResource * resource = NULL;
	
	switch(resourceType)
	{
        case EFF3DResourceType_Texture2D:
        case EFF3DResourceType_Texture3D:
        case EFF3DResourceType_TextureCube:
        case EFF3DResourceType_RenderTarget:
	        {
		        resource = EFFNEW EFFD3D9Texture();
	        }
	        break;
        case EFF3DResourceType_IndexBuffer:
            {
                resource = EFFNEW EFFD3D9IndexBuffer();
            }
            break;
        case EFF3DResourceType_VertexBuffer:
            {
                resource = EFFNEW EFFD3D9VertexBuffer();
            }
            break;
		case EFF3DResourceType_TimeQuery:
			{
				resource = EFFNEW EFFD3D9TimeQuery();
			}
			break;
	    default:
		    break;
	}

	return resource;
}

effBOOL EFFD3D9Device::CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT flag, EFF3DTextureFormat format, 
	EFF3DResourceType resourceType, EFF3DTextureHandle * textureHandle, effSIZE sharedHandle)
{
	//assert(texture != NULL);

    EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)CreateEmptyResource(resourceType, *textureHandle);

    effHRESULT hr;
    effUINT usage = 0;

	if( FAILED(hr = D3D9Device->CreateTexture(width, height, levels, usage, s_textureFormat[format].d3d9Format, 
		D3DPOOL_DEFAULT, &effD3D9Texture->texture, (HANDLE *)&sharedHandle)) )
	{
		SF_DELETE(effD3D9Texture);
		return effFALSE;
	}


	effD3D9Texture->imageInfo.width = width;
	effD3D9Texture->imageInfo.height = height;
	effD3D9Texture->imageInfo.mipLevels = levels;
	effD3D9Texture->imageInfo.format = format;
	effD3D9Texture->imageInfo.type = resourceType;
    effD3D9Texture->imageInfo.sharedHandle = (effHANDLE)sharedHandle;

    *textureHandle = effD3D9Texture->id;

	return effTRUE;	
}



/*effBOOL EFFD3D9Device::_CreateSharedTexture(effUINT width, effUINT height, effUINT levels, effUINT usage, EFF3DFORMAT format, EFF3DSharedTexture** texture)
{
    assert(texture != NULL);

    EFFD3D9SharedTexture * effD3D9Texture = EFFNEW EFFD3D9SharedTexture();

    effHRESULT hr;

    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        if (FAILED(hr = D3D9Device->CreateTexture(width, height, levels, usage, (D3DFORMAT)format, D3DPOOL_DEFAULT, &effD3D9Texture->texture[i], &effD3D9Texture->sharedHandle[i])))
        {
            SF_DELETE(effD3D9Texture);
            return effFALSE;
        }
    }

    effD3D9Texture->m_ImageInfo.width = width;
    effD3D9Texture->m_ImageInfo.height = height;
    effD3D9Texture->m_ImageInfo.mipLevels = levels;
    effD3D9Texture->m_ImageInfo.usage = usage;
    effD3D9Texture->m_ImageInfo.format = (EFF3DFORMAT)format;
    effD3D9Texture->m_ImageInfo.pool = EFF3DPOOL_DEFAULT;
    effD3D9Texture->m_ImageInfo.resourceType = EFF3DRTYPE_TEXTURE;

    effD3D9Texture->AddRef();


    *texture = effD3D9Texture;

    return effTRUE;
}

effBOOL EFFD3D9Device::_CreateSharedTexture(SharedTextureInfo * sharedTextureInfo, EFF3DSharedTexture ** texture)
{

    EFFD3D9SharedTexture * effD3D9Texture = EFFNEW EFFD3D9SharedTexture();

    effD3D9Texture->sharedHandle[0] = (HANDLE)sharedTextureInfo->sharedTextureHandle[0];
    effD3D9Texture->sharedHandle[1] = (HANDLE)sharedTextureInfo->sharedTextureHandle[1];
	effD3D9Texture->sharedHandle[2] = (HANDLE)sharedTextureInfo->sharedTextureHandle[2];

    effHRESULT hr;

    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        if (FAILED(hr = D3D9Device->CreateTexture(sharedTextureInfo->width, sharedTextureInfo->height, 1, 0, (D3DFORMAT)sharedTextureInfo->format, D3DPOOL_DEFAULT, &effD3D9Texture->texture[i], &effD3D9Texture->sharedHandle[i])))
        {
            SF_DELETE(effD3D9Texture);
            return effFALSE;
        }
    }


    effD3D9Texture->sharedHandle[0] = NULL;
    effD3D9Texture->sharedHandle[1] = NULL;
	effD3D9Texture->sharedHandle[2] = NULL;

    effD3D9Texture->AddRef();


    *texture = effD3D9Texture;

    return effTRUE;
}*/



effBOOL EFFD3D9Device::CreateTextureFromMemory(effVOID * srcData, effUINT srcDataSize, effINT width, effINT height, effINT level, effUINT flag,
                                            EFF3DTextureFormat format, EFF3DResourceType resourceType, EFF3DTextureHandle * textureHandle)
{
	//assert(texture != NULL);

	effINT pixelSize = EFF3DGetPixelSizeFromFormat(format);
	if (width * height * pixelSize > (effINT)srcDataSize)
	{
		return effFALSE;
	}

    EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)CreateEmptyResource(resourceType, *textureHandle);
    effHRESULT hr;


    effHANDLE shareHandle = NULL;
	if (FAILED(hr = D3D9Device->CreateTexture(width, height, level, 0, s_textureFormat[format].d3d9Format, D3DPOOL_DEFAULT, &effD3D9Texture->texture, &shareHandle)))
	{
		SF_DELETE(effD3D9Texture);
		return effFALSE;
	}

    effD3D9Texture->userData = shareHandle;

    EFF3DLockedRect rc;
	if (FAILED(effD3D9Texture->LockRect(0, &rc, NULL, 0)))
	{
		SF_DELETE(effD3D9Texture);
		return effFALSE;
	}

	if (rc.pitch < pixelSize * width)
	{
		SF_DELETE(effD3D9Texture);
		effD3D9Texture->UnlockRect(0);
		return effFALSE;
	}

	for (effINT i = 0; i < height; i++)
	{
		memcpy(((effBYTE *)rc.data) + rc.pitch * i, ((effBYTE *)srcData) + i * width * pixelSize, width * pixelSize);
	}

	effD3D9Texture->UnlockRect(0);

	effD3D9Texture->imageInfo.width = width;
	effD3D9Texture->imageInfo.height = height;
	//effD3D9Texture->imageInfo.usage = 0;
	effD3D9Texture->imageInfo.format = format;
	//effD3D9Texture->imageInfo.pool = EFF3DPOOL_DEFAULT;
	effD3D9Texture->imageInfo.type = resourceType;


    *textureHandle = effD3D9Texture->id;

	return effTRUE;	
}

/*effBOOL EFFD3D9Device::CreateRenderTarget(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample, effUINT multisampleQuality, effBOOL lockable, EFF3DSurface ** surface)
{
	assert(surface != NULL);

	EFFD3D9Surface * effD3D9Surface = EFFNEW EFFD3D9Surface();

	effHRESULT hr;
	if ( FAILED(hr = D3D9Device->CreateRenderTarget(width, height, (D3DFORMAT)format, (D3DMULTISAMPLE_TYPE)multiSample, multisampleQuality, lockable, &effD3D9Surface->m_pSurface, NULL)) )
	{
		SF_DELETE(effD3D9Surface);
		return effFALSE;
	}

	effD3D9Surface->m_ImageInfo.width = width;
	effD3D9Surface->m_ImageInfo.height = height;
	effD3D9Surface->m_ImageInfo.format = (EFF3DFORMAT)format;
	effD3D9Surface->m_ImageInfo.surfaceType = RenderTo_Surface;

	effD3D9Surface->AddRef();
	*surface = effD3D9Surface;

	return effTRUE;
}

effBOOL EFFD3D9Device::CreateDepthStencilSurface(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample, effUINT multisampleQuality, effBOOL discard, EFF3DSurface ** surface)
{
	assert(surface != NULL);

	EFFD3D9Surface * effD3D9Surface = EFFNEW EFFD3D9Surface();

	effHRESULT hr;
	if ( FAILED(hr = D3D9Device->CreateDepthStencilSurface(width, height, (D3DFORMAT)format, (D3DMULTISAMPLE_TYPE)multiSample, multisampleQuality, discard, &effD3D9Surface->m_pSurface, NULL)) )
	{
		SF_DELETE(effD3D9Surface);
		return effFALSE;
	}

	effD3D9Surface->m_ImageInfo.width = width;
	effD3D9Surface->m_ImageInfo.height = height;
	effD3D9Surface->m_ImageInfo.format = (EFF3DFORMAT)format;
	effD3D9Surface->m_ImageInfo.surfaceType = DepthStencil_Surface;

	effD3D9Surface->AddRef();
	*surface = effD3D9Surface;

	return effTRUE;
}*/

effBOOL EFFD3D9Device::CreateIndexBuffer(effVOID * data, effUINT size, effUINT flag, EFF3DIndexBufferHandle * ibHandle)
{
	assert(ibHandle != NULL);
	
    EFFD3D9IndexBuffer * effD3D9IndexBuffer = (EFFD3D9IndexBuffer *)CreateEmptyResource(EFF3DResourceType_IndexBuffer, *ibHandle);
	effHRESULT hr;

    D3DFORMAT format = D3DFMT_INDEX16;
    if (flag & EFF3D_BUFFER_INDEX32)
    {
        format = D3DFMT_INDEX32;
    }

	if ( FAILED(hr = D3D9Device->CreateIndexBuffer(size, 0, format, D3DPOOL_DEFAULT, &effD3D9IndexBuffer->d3d9IndexBuffer, NULL)) )
	{
        SF_RELEASE(effD3D9IndexBuffer);
		return effFALSE;
	}

    if (data != NULL)
    {
        effD3D9IndexBuffer->Update(0, size, data);
    }

	*ibHandle = effD3D9IndexBuffer->id;
	return effTRUE;
}

effBOOL EFFD3D9Device::UpdateIndexBuffer(effUINT offset, effVOID * data, effUINT size)
{
    return effTRUE;
}

effBOOL EFFD3D9Device::CreateVertexBuffer(effVOID * data, effUINT size, effUINT flag, EFF3DVertexDeclarationHandle vertexDeclHandle,
	EFF3DVertexBufferHandle * vertexBufferHandle)
{
	//https://docs.microsoft.com/en-us/windows/desktop/api/d3d9/nf-d3d9-idirect3ddevice9-createvertexbuffer

	EFFD3D9VertexBuffer * effD3D9VertexBuffer = (EFFD3D9VertexBuffer *)CreateEmptyResource(EFF3DResourceType_VertexBuffer, *vertexBufferHandle);
	effD3D9VertexBuffer->vertexDeclHandle = vertexDeclHandle;
	effHRESULT hr;

    effUINT usage = D3DUSAGE_WRITEONLY;
	// https://docs.microsoft.com/zh-cn/windows/desktop/direct3d9/d3dpool
	// Differences between Direct3D 9 and Direct3D 9Ex:
	// D3DPOOL_MANAGED is valid with IDirect3DDevice9; however, it is not valid with IDirect3DDevice9Ex.
    D3DPOOL pool = D3DPOOL_DEFAULT;

    if (data == NULL)
    {
        usage |= D3DUSAGE_DYNAMIC;
        pool = D3DPOOL_DEFAULT;
    }

	if ( FAILED(hr = D3D9Device->CreateVertexBuffer(size, usage, 0, pool, &effD3D9VertexBuffer->d3d9VertexBuffer, NULL)) )
	{
		SF_RELEASE(effD3D9VertexBuffer);
		return effFALSE;
	}

    if (data != NULL)
    {
        effD3D9VertexBuffer->Update(0, size, data);
    }

	*vertexBufferHandle = effD3D9VertexBuffer->id;
	return effTRUE;
}

effBOOL EFFD3D9Device::UpdateVertexBuffer(effUINT offset, effVOID * data, effUINT size)
{
    return effTRUE;
}

static D3DVERTEXELEMENT9 * fillVertexDecl(effBYTE stream, D3DVERTEXELEMENT9 * out, const EFF3DVertexDeclaration & vertexDecl)
{
    D3DVERTEXELEMENT9 * elem = out;

    for (effUINT attr = 0; attr < EFF3DVertexAttrib::Count; ++attr)
    {
        if (UINT16_MAX != vertexDecl.attributes[attr].valueUINT16)
        {
            effBYTE num;
            EFF3DVertexAttribType::Enum type;
            effBOOL normalized;
            effBOOL asInt;
            vertexDecl.Decode(EFF3DVertexAttrib::Enum(attr), num, type, normalized, asInt);

            memcpy(elem, &s_attrib[attr], sizeof(D3DVERTEXELEMENT9));

            elem->Stream = stream;
            elem->Type = s_attribType[type][num - 1][normalized];
            elem->Offset = vertexDecl.offset[attr];
            ++elem;
        }
    }

    return elem;
}


effVOID EFFD3D9Device::SetVertices()
{
    effBOOL vertexStreamChanged = HasVertexStreamChanged();

    if (programChanged || vertexStreamChanged)
    {
        currentDrawCommand.streamMask = newDrawCommand->streamMask;
        currentDrawCommand.instanceDataBufferHandle = newDrawCommand->instanceDataBufferHandle;
        currentDrawCommand.instanceDataOffset = newDrawCommand->instanceDataOffset;
        currentDrawCommand.instanceDataStride = newDrawCommand->instanceDataStride;

        const EFF3DVertexDeclaration * decls[EFF3D_CONFIG_MAX_VERTEX_STREAMS];

        const effBOOL instanced = effTRUE && newDrawCommand->instanceDataBufferHandle.IsValid();

        const effUINT freq = instanced ? D3DSTREAMSOURCE_INDEXEDDATA | newDrawCommand->numInstances : 1;

        effUINT numVertices = newDrawCommand->numVertices;
        effBYTE numStreams = 0;
        for (effUINT idx = 0, streamMask = newDrawCommand->streamMask, ntz = effUINT_cnttz(streamMask)
            ; 0 != streamMask
            ; streamMask >>= 1, idx += 1, ntz = effUINT_cnttz(streamMask), ++numStreams
            )
        {
            streamMask >>= ntz;
            idx += ntz;

            currentDrawCommand.stream[idx].vertexDeclHandle = newDrawCommand->stream[idx].vertexDeclHandle;
            currentDrawCommand.stream[idx].vertexBufferHandle = newDrawCommand->stream[idx].vertexBufferHandle;
            currentDrawCommand.stream[idx].startVertex = newDrawCommand->stream[idx].startVertex;

            const EFF3DVertexBufferHandle vertexBufferHandle = newDrawCommand->stream[idx].vertexBufferHandle;
            const EFFD3D9VertexBuffer * vb = (EFFD3D9VertexBuffer *)GetVertexBufferManager()->GetResource(vertexBufferHandle);
            const EFF3DVertexDeclarationHandle vertexDeclHandle = !vb->vertexDeclHandle.IsValid() ? newDrawCommand->stream[idx].vertexDeclHandle : vb->vertexDeclHandle;
            const EFF3DVertexDeclaration * vertexDecl = vertexDeclManager->GetVertexDeclaration(vertexDeclHandle);
            const effUINT stride = vertexDecl->stride;

            decls[numStreams] = vertexDecl;

            numVertices = std::min(UINT32_MAX == newDrawCommand->numVertices ? vb->size / stride : newDrawCommand->numVertices, numVertices);

            DX_CHECK(D3D9Device->SetStreamSourceFreq(0, freq));
            DX_CHECK(D3D9Device->SetStreamSource(numStreams, vb->d3d9VertexBuffer, 0, stride));
        }

        currentDrawCommand.numVertices = numVertices;

        if (0 < numStreams)
        {
            if (instanced)
            {
                const EFFD3D9VertexBuffer & inst = *(EFFD3D9VertexBuffer *)GetVertexBufferManager()->GetResource(newDrawCommand->instanceDataBufferHandle);
                DX_CHECK(D3D9Device->SetStreamSourceFreq(numStreams, UINT(D3DSTREAMSOURCE_INSTANCEDATA | 1)));
                DX_CHECK(D3D9Device->SetStreamSource(numStreams, inst.d3d9VertexBuffer, newDrawCommand->instanceDataOffset, newDrawCommand->instanceDataStride));
                SetInputLayout(numStreams, decls, newDrawCommand->instanceDataStride / 16);
            }
            else
            {
                DX_CHECK(D3D9Device->SetStreamSource(numStreams, NULL, 0, 0));
                SetInputLayout(numStreams, decls, 0);
            }
        }
        else
        {
            DX_CHECK(D3D9Device->SetStreamSource(0, NULL, 0, 0));
            DX_CHECK(D3D9Device->SetStreamSource(1, NULL, 0, 0));
        }
    }

}

effVOID EFFD3D9Device::SetInputLayout(effBYTE numStreams, const EFF3DVertexDeclaration ** vertexDecls, effUINT16 numInstanceData)
{
    effBYTE key[4 * EFF3D_CONFIG_MAX_VERTEX_STREAMS];
    effUINT size = 0;
    memset(key, 0, sizeof(key));

    //memcpy(key, &numInstanceData, sizeof(effUINT16));
    //size += 2;

    for (effBYTE stream = 0; stream < numStreams; ++stream)
    {
        memcpy(key + stream * 4, &vertexDecls[stream]->hash, sizeof(effUINT));
        size += 4;
    }

    effUINT layoutHash;
    MurmurHash3_x86_32(key, size, 9582, &layoutHash);

    LPDIRECT3DVERTEXDECLARATION9 layout = NULL;
    auto it = inputLayoutCache.find(layoutHash);
    if (it == inputLayoutCache.end())
    {
        D3DVERTEXELEMENT9 vertexElements[EFF3DVertexAttrib::Count + 1 + EFF3D_CONFIG_MAX_INSTANCE_DATA_COUNT];
        D3DVERTEXELEMENT9* elem = vertexElements;

        for (effBYTE stream = 0; stream < numStreams; ++stream)
        {
            elem = fillVertexDecl(stream, elem, *vertexDecls[stream]);
        }

        const D3DVERTEXELEMENT9 inst = { numStreams, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 };

        for (effBYTE ii = 0; ii < numInstanceData; ++ii)
        {
            memcpy(elem, &inst, sizeof(D3DVERTEXELEMENT9));
            elem->UsageIndex = effBYTE(7 - ii); // TEXCOORD7 = i_data0, TEXCOORD6 = i_data1, etc.
            elem->Offset = ii * 16;
            ++elem;
        }

        memcpy(elem, &s_attrib[EFF3DVertexAttrib::Count], sizeof(D3DVERTEXELEMENT9));

        DX_CHECK(D3D9Device->CreateVertexDeclaration(vertexElements, &layout));

        inputLayoutCache[layoutHash] = layout;
    }
	else
	{
		layout = it->second;
	}

    DX_CHECK(D3D9Device->SetVertexDeclaration(layout));
}

effVOID EFFD3D9Device::Draw(EFF3DDrawCommand & drawCommand)
{

	newDrawCommand = &drawCommand;




	SetRenderState(drawCommand);
	SetDepthStencilState(drawCommand);

	SetVertices();
	SetIndexBuffer();
	SetTextures(drawCommand);



    if (0 != currentDrawCommand.streamMask)
    {
        effUINT numVertices = drawCommand.numVertices;
        effUINT numIndices = 0;
        effUINT numPrimsSubmitted = 0;
        effUINT numInstances = 0;
        effUINT numPrimsRendered = 0;

        if (drawCommand.indexBufferHandle.IsValid())
        {
            if (UINT32_MAX == drawCommand.numIndices)
            {
				EFFD3D9IndexBuffer * effD3D9IB = (EFFD3D9IndexBuffer *)indexBufferManager->GetResource(drawCommand.indexBufferHandle);
                const uint32_t indexSize = 0 == (effD3D9IB->flags & EFF3D_BUFFER_INDEX32) ? 2 : 4;
                numIndices = effD3D9IB->size / indexSize;
                numPrimsSubmitted = numIndices / primitiveInfo.div - primitiveInfo.sub;
                numInstances = drawCommand.numInstances;
                numPrimsRendered = numPrimsSubmitted * drawCommand.numInstances;

                DX_CHECK(D3D9Device->DrawIndexedPrimitive(primitiveInfo.type
                    , drawCommand.stream[0].startVertex
                    , 0
                    , numVertices
                    , 0
                    , numPrimsSubmitted
                ));
            }
            else if (primitiveInfo.min <= drawCommand.numIndices)
            {
                numIndices = drawCommand.numIndices;
                numPrimsSubmitted = numIndices / primitiveInfo.div - primitiveInfo.sub;
                numInstances = drawCommand.numInstances;
                numPrimsRendered = numPrimsSubmitted * drawCommand.numInstances;

                DX_CHECK(D3D9Device->DrawIndexedPrimitive(primitiveInfo.type
                    , drawCommand.stream[0].startVertex
                    , 0
                    , numVertices
                    , drawCommand.startIndex
                    , numPrimsSubmitted
                ));
            }
        }
        else
        {
            numPrimsSubmitted = numVertices / primitiveInfo.div - primitiveInfo.sub;
            numInstances = drawCommand.numInstances;
            numPrimsRendered = numPrimsSubmitted * drawCommand.numInstances;

            DX_CHECK(D3D9Device->DrawPrimitive(primitiveInfo.type
                , drawCommand.stream[0].startVertex
                , numPrimsSubmitted
            ));
        }


        //statsNumPrimsSubmitted[primIndex] += numPrimsSubmitted;
        //statsNumPrimsRendered[primIndex] += numPrimsRendered;
        //statsNumInstances[primIndex] += numInstances;
        //statsNumIndices += numIndices;
    }
}

effBOOL EFFD3D9Device::CreateTimeQuery(effUINT flag, EFF3DTimeQueryHandle * queryHandle)
{
	EFFD3D9TimeQuery * effD3D9TimeQuery = (EFFD3D9TimeQuery *)CreateEmptyResource(EFF3DResourceType_TimeQuery, *queryHandle);
	effD3D9TimeQuery->Init(D3D9Device);

	*queryHandle = effD3D9TimeQuery->id;
	return effTRUE;
}

effBOOL EFFD3D9Device::DrawIndexedPrimitive(EFF3DPrimitiveType type, effINT baseVertexIndex, effUINT minIndex, effUINT numVertices,
    effUINT startIndex, effUINT primitiveCount)
{
    return effTRUE;
	//return SUCCEEDED(D3D9Device->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount));
}

effBOOL EFFD3D9Device::DrawIndexedPrimitiveUP(EFF3DPrimitiveType primitiveType, effUINT minVertexIndex, effUINT numVertices, effUINT primitiveCount,
    const effVOID * indexData, effBOOL index16,
    const effVOID * vertexStreamZeroData, effUINT vertexStreamZeroStride)
{
    return effTRUE;
	//return SUCCEEDED(D3D9Device->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)primitiveType, minVertexIndex, numVertices, primitiveCount, indexData, (D3DFORMAT)indexDataFormat, vertexStreamZeroData, vertexStreamZeroStride));
}

effBOOL EFFD3D9Device::DrawPrimitive(EFF3DPrimitiveType primitiveType, effUINT startVertex, effUINT primitiveCount)
{
	//HRESULT hr = D3D9Device->DrawPrimitive((D3DPRIMITIVETYPE)primitiveType, startVertex, primitiveCount);
	//return SUCCEEDED(hr);
    return effTRUE;
}

effBOOL EFFD3D9Device::DrawPrimitiveUP(EFF3DPrimitiveType primitiveType, effUINT primitiveCount, const effVOID * vertexStreamZeroData,
    effUINT vertexStreamZeroStride)
{
    return effTRUE;
	//return SUCCEEDED(D3D9Device->DrawPrimitiveUP((D3DPRIMITIVETYPE)primitiveType, primitiveCount, vertexStreamZeroData, vertexStreamZeroStride));
}

/*effBOOL EFFD3D9Device::SetTransform(EFF3DPrimitiveType state, const EFFMatrix4 * matrix)
{
	return SUCCEEDED(D3D9Device->SetTransform((D3DTRANSFORMSTATETYPE)state, (const D3DMATRIX *)matrix));
}*/

effVOID	EFFD3D9Device::SetIndexBuffer()
{
	if (currentDrawCommand.indexBufferHandle != newDrawCommand->indexBufferHandle)
	{
		currentDrawCommand.indexBufferHandle = newDrawCommand->indexBufferHandle;
		SetIndexBuffer(newDrawCommand->indexBufferHandle);
	}
}


effBOOL EFFD3D9Device::SetIndexBuffer(EFF3DIndexBufferHandle ibHandle)
{
	if (ibHandle.IsValid())
	{
		EFFD3D9IndexBuffer * effD3D9IB = (EFFD3D9IndexBuffer *)indexBufferManager->GetResource(ibHandle);
		return SUCCEEDED(D3D9Device->SetIndices(effD3D9IB->d3d9IndexBuffer));
	}
	else
	{
		//DX_CHECK(device->SetIndices(NULL));
		return effFALSE;
	}
}

//effBOOL EFFD3D9Device::SetRenderState(EFF3DRENDERSTATETYPE state, effUINT value)
//{
//	return SUCCEEDED(D3D9Device->SetRenderState((D3DRENDERSTATETYPE)state, value));
//}

effVOID EFFD3D9Device::SetRenderState(EFF3DDrawCommand & drawCommand)
{
    const effUINT64 newFlags = drawCommand.stateFlags;
    effUINT64 changedFlags = currentDrawCommand.stateFlags ^ drawCommand.stateFlags;
    currentDrawCommand.stateFlags = newFlags;

    if ((EFF3D_STATE_CULL_MASK
        | EFF3D_STATE_WRITE_Z
        | EFF3D_STATE_DEPTH_TEST_MASK
        | EFF3D_STATE_WRITE_RGB
        | EFF3D_STATE_WRITE_A
        | EFF3D_STATE_BLEND_MASK
        | EFF3D_STATE_BLEND_EQUATION_MASK
        | EFF3D_STATE_ALPHA_REF_MASK
        | EFF3D_STATE_PT_MASK
        | EFF3D_STATE_POINT_SIZE_MASK
        | EFF3D_STATE_MSAA) & changedFlags)
    {
        if (EFF3D_STATE_CULL_MASK & changedFlags)
        {
            effUINT32 cull = (newFlags & EFF3D_STATE_CULL_MASK) >> EFF3D_STATE_CULL_SHIFT;
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_CULLMODE, s_cullMode[cull]));
        }

        if (EFF3D_STATE_WRITE_Z & changedFlags)
        {
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_ZWRITEENABLE, !!(EFF3D_STATE_WRITE_Z & newFlags)));
        }

        if (EFF3D_STATE_DEPTH_TEST_MASK & changedFlags)
        {
            effUINT32 func = (newFlags & EFF3D_STATE_DEPTH_TEST_MASK) >> EFF3D_STATE_DEPTH_TEST_SHIFT;
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_ZENABLE, 0 != func));

            if (0 != func)
            {
                DX_CHECK(D3D9Device->SetRenderState(D3DRS_ZFUNC, s_cmpFunc[func]));
            }
        }

        if (EFF3D_STATE_ALPHA_REF_MASK & changedFlags)
        {
            effUINT32 ref = (newFlags & EFF3D_STATE_ALPHA_REF_MASK) >> EFF3D_STATE_ALPHA_REF_SHIFT;
            //viewState.m_alphaRef = ref / 255.0f;
        }

        if ((EFF3D_STATE_PT_POINTS | EFF3D_STATE_POINT_SIZE_MASK) & changedFlags)
        {
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_POINTSIZE, (effUINT32)((newFlags & EFF3D_STATE_POINT_SIZE_MASK) >> EFF3D_STATE_POINT_SIZE_SHIFT)));
        }

        if (EFF3D_STATE_MSAA & changedFlags)
        {
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, (newFlags & EFF3D_STATE_MSAA) == EFF3D_STATE_MSAA));
        }

        if (EFF3D_STATE_LINEAA & changedFlags)
        {
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, !!(newFlags & EFF3D_STATE_LINEAA)));
        }

        if ((EFF3D_STATE_WRITE_A | EFF3D_STATE_WRITE_RGB) & changedFlags)
        {
            effUINT32 writeEnable = 0;
            writeEnable |= (newFlags & EFF3D_STATE_WRITE_R) ? D3DCOLORWRITEENABLE_RED : 0;
            writeEnable |= (newFlags & EFF3D_STATE_WRITE_G) ? D3DCOLORWRITEENABLE_GREEN : 0;
            writeEnable |= (newFlags & EFF3D_STATE_WRITE_B) ? D3DCOLORWRITEENABLE_BLUE : 0;
            writeEnable |= (newFlags & EFF3D_STATE_WRITE_A) ? D3DCOLORWRITEENABLE_ALPHA : 0;
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_COLORWRITEENABLE, writeEnable));
        }

        if (((EFF3D_STATE_BLEND_MASK
            | EFF3D_STATE_BLEND_EQUATION_MASK
            | EFF3D_STATE_BLEND_ALPHA_TO_COVERAGE
            ) & changedFlags)
            || BlendFactor != drawCommand.rgba)
        {
            effBOOL enabled = !!(EFF3D_STATE_BLEND_MASK & newFlags);
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, enabled));

            if (AtocSupport && EFF3D_STATE_BLEND_ALPHA_TO_COVERAGE & changedFlags)
            {
                DX_CHECK(D3D9Device->SetRenderState(D3DRS_ADAPTIVETESS_Y, !!(newFlags & EFF3D_STATE_BLEND_ALPHA_TO_COVERAGE) ? D3DFMT_ATOC : 0));
            }

            if (enabled)
            {
                const effUINT32 blend = uint32_t((newFlags & EFF3D_STATE_BLEND_MASK) >> EFF3D_STATE_BLEND_SHIFT);
                const effUINT32 equation = uint32_t((newFlags & EFF3D_STATE_BLEND_EQUATION_MASK) >> EFF3D_STATE_BLEND_EQUATION_SHIFT);

                const effUINT32 srcRGB = (blend) & 0xf;
                const effUINT32 dstRGB = (blend >> 4) & 0xf;
                const effUINT32 srcA = (blend >> 8) & 0xf;
                const effUINT32 dstA = (blend >> 12) & 0xf;

                const effUINT32 equRGB = (equation) & 0x7;
                const effUINT32 equA = (equation >> 3) & 0x7;

                DX_CHECK(D3D9Device->SetRenderState(D3DRS_SRCBLEND, s_blendFactor[srcRGB].m_src));
                DX_CHECK(D3D9Device->SetRenderState(D3DRS_DESTBLEND, s_blendFactor[dstRGB].m_dst));
                DX_CHECK(D3D9Device->SetRenderState(D3DRS_BLENDOP, s_blendEquation[equRGB]));

                const bool separate = srcRGB != srcA || dstRGB != dstA || equRGB != equA;

                DX_CHECK(D3D9Device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, separate));
                if (separate)
                {
                    DX_CHECK(D3D9Device->SetRenderState(D3DRS_SRCBLENDALPHA, s_blendFactor[srcA].m_src));
                    DX_CHECK(D3D9Device->SetRenderState(D3DRS_DESTBLENDALPHA, s_blendFactor[dstA].m_dst));
                    DX_CHECK(D3D9Device->SetRenderState(D3DRS_BLENDOPALPHA, s_blendEquation[equA]));
                }

                if ((s_blendFactor[srcRGB].m_factor || s_blendFactor[dstRGB].m_factor)
                    && BlendFactor != drawCommand.rgba)
                {
                    const effUINT rgba = drawCommand.rgba;
                    D3DCOLOR color = D3DCOLOR_RGBA(rgba >> 24
                        , (rgba >> 16) & 0xff
                        , (rgba >> 8) & 0xff
                        , (rgba) & 0xff
                    );
                    DX_CHECK(D3D9Device->SetRenderState(D3DRS_BLENDFACTOR, color));
                }
            }

            BlendFactor = drawCommand.rgba;
        }


		const effUINT64 primitiveType = renderDebugFlags & EFF3D_DEBUG_WIREFRAME ? EFF3D_STATE_PT_LINES : newFlags & EFF3D_STATE_PT_MASK;
		effBYTE primitiveIndex = effBYTE(primitiveType >> EFF3D_STATE_PT_SHIFT);

		primitiveInfo = s_primInfo[primitiveIndex];
    }
}

effVOID EFFD3D9Device::SetDepthStencilState(EFF3DDrawCommand & drawCommand)
{
    const effUINT64 newStencil = drawCommand.stencil;
    effUINT64 changedStencil = currentDrawCommand.stencil ^ drawCommand.stencil;
    currentDrawCommand.stencil = newStencil;

    if (0 != changedStencil)
    {
        effBOOL enable = 0 != newStencil;
        DX_CHECK(D3D9Device->SetRenderState(D3DRS_STENCILENABLE, enable));

        if (0 != newStencil)
        {
            effUINT fstencil = UnpackStencil(0, newStencil);
            effUINT bstencil = UnpackStencil(1, newStencil);
            effBYTE frontAndBack = bstencil != EFF3D_STENCIL_NONE && bstencil != fstencil;
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, 0 != frontAndBack));

            effUINT fchanged = UnpackStencil(0, changedStencil);
            if ((EFF3D_STENCIL_FUNC_REF_MASK | EFF3D_STENCIL_FUNC_RMASK_MASK) & fchanged)
            {
                effUINT ref = (fstencil & EFF3D_STENCIL_FUNC_REF_MASK) >> EFF3D_STENCIL_FUNC_REF_SHIFT;
                DX_CHECK(D3D9Device->SetRenderState(D3DRS_STENCILREF, ref));

                effUINT rmask = (fstencil & EFF3D_STENCIL_FUNC_RMASK_MASK) >> EFF3D_STENCIL_FUNC_RMASK_SHIFT;
                DX_CHECK(D3D9Device->SetRenderState(D3DRS_STENCILMASK, rmask));
            }

            // 						uint32_t bchanged = unpackStencil(1, changedStencil);
            // 						if (EFF3D_STENCIL_FUNC_RMASK_MASK & bchanged)
            // 						{
            // 							uint32_t wmask = (bstencil&EFF3D_STENCIL_FUNC_RMASK_MASK)>>EFF3D_STENCIL_FUNC_RMASK_SHIFT;
            // 							DX_CHECK(device->SetRenderState(D3DRS_STENCILWRITEMASK, wmask) );
            // 						}

            for (effBYTE ii = 0, num = frontAndBack + 1; ii < num; ++ii)
            {
                effUINT stencil = UnpackStencil(ii, newStencil);
                effUINT changed = UnpackStencil(ii, changedStencil);

                if ((EFF3D_STENCIL_TEST_MASK | EFF3D_STENCIL_FUNC_REF_MASK | EFF3D_STENCIL_FUNC_RMASK_MASK) & changed)
                {
                    effUINT func = (stencil & EFF3D_STENCIL_TEST_MASK) >> EFF3D_STENCIL_TEST_SHIFT;
                    DX_CHECK(D3D9Device->SetRenderState(s_stencilFuncRs[ii], s_cmpFunc[func]));
                }

                if ((EFF3D_STENCIL_OP_FAIL_S_MASK | EFF3D_STENCIL_OP_FAIL_Z_MASK | EFF3D_STENCIL_OP_PASS_Z_MASK) & changed)
                {
                    effUINT sfail = (stencil & EFF3D_STENCIL_OP_FAIL_S_MASK) >> EFF3D_STENCIL_OP_FAIL_S_SHIFT;
                    DX_CHECK(D3D9Device->SetRenderState(s_stencilFailRs[ii], s_stencilOp[sfail]));

                    effUINT zfail = (stencil & EFF3D_STENCIL_OP_FAIL_Z_MASK) >> EFF3D_STENCIL_OP_FAIL_Z_SHIFT;
                    DX_CHECK(D3D9Device->SetRenderState(s_stencilZFailRs[ii], s_stencilOp[zfail]));

                    effUINT zpass = (stencil & EFF3D_STENCIL_OP_PASS_Z_MASK) >> EFF3D_STENCIL_OP_PASS_Z_SHIFT;
                    DX_CHECK(D3D9Device->SetRenderState(s_stencilZPassRs[ii], s_stencilOp[zpass]));
                }
            }
        }
    }
}

static effVOID SetSamplerStateReal(LPDIRECT3DDEVICE9EX device, DWORD stage, D3DSAMPLERSTATETYPE type, DWORD value)
{
    DX_CHECK(device->SetSamplerState(stage, type, value));
    if (4 > stage)
    {
        DX_CHECK(device->SetSamplerState(D3DVERTEXTEXTURESAMPLER0 + stage, type, value));
    }
}

inline effBOOL NeedBorderColor(effUINT flags)
{
    return EFF3D_TEXTURE_U_BORDER == (flags & EFF3D_TEXTURE_U_BORDER)
        || EFF3D_TEXTURE_V_BORDER == (flags & EFF3D_TEXTURE_V_BORDER)
        || EFF3D_TEXTURE_W_BORDER == (flags & EFF3D_TEXTURE_W_BORDER);
}

effVOID EFFD3D9Device::SetSamplerState(effUINT stage, effUINT flags)
{
    const effUINT newFlags = flags & ((~EFF3D_TEXTURE_RESERVED_MASK) | EFF3D_TEXTURE_SAMPLER_BITS_MASK | EFF3D_TEXTURE_SRGB);

    if (samplerFlags[stage] != newFlags)
    {
        samplerFlags[stage] = newFlags;

        D3DTEXTUREADDRESS tau = s_textureAddress[(flags & EFF3D_TEXTURE_U_MASK) >> EFF3D_TEXTURE_U_SHIFT];
        D3DTEXTUREADDRESS tav = s_textureAddress[(flags & EFF3D_TEXTURE_V_MASK) >> EFF3D_TEXTURE_V_SHIFT];
        D3DTEXTUREADDRESS taw = s_textureAddress[(flags & EFF3D_TEXTURE_W_MASK) >> EFF3D_TEXTURE_W_SHIFT];
        D3DTEXTUREFILTERTYPE minFilter = s_textureFilter[(flags & EFF3D_TEXTURE_MIN_MASK) >> EFF3D_TEXTURE_MIN_SHIFT];
        D3DTEXTUREFILTERTYPE magFilter = s_textureFilter[(flags & EFF3D_TEXTURE_MAG_MASK) >> EFF3D_TEXTURE_MAG_SHIFT];
        D3DTEXTUREFILTERTYPE mipFilter = s_textureFilter[(flags & EFF3D_TEXTURE_MIP_MASK) >> EFF3D_TEXTURE_MIP_SHIFT];

        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_ADDRESSU, tau);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_ADDRESSV, tav);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_ADDRESSW, taw);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_MINFILTER, minFilter);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_MAGFILTER, magFilter);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_MIPFILTER, mipFilter);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_MAXANISOTROPY, maxAnisotropy);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_SRGBTEXTURE, 0 != (newFlags & EFF3D_TEXTURE_SRGB));
    }
}

effVOID EFFD3D9Device::SetTextures(EFF3DDrawCommand & drawCommand)
{

    for (effINT stage = 0; stage < EFF3D_CONFIG_MAX_TEXTURE_SAMPLERS; ++stage)
    {
        const EFF3DBinding & textureBind = drawCommand.textureBind.binds[stage];
        EFF3DBinding & currentTextureBind = currentDrawCommand.textureBind.binds[stage];

        if (currentTextureBind.handle != textureBind.handle
            || currentTextureBind.data.draw.flags != textureBind.data.draw.flags
            || programChanged)
        {

            EFFD3D9Texture * texture = (EFFD3D9Texture *)textureManager->GetResource(textureBind.handle);
            if (texture != NULL)
            {
                texture->Commit(stage, textureBind.data.draw.flags);
            }
            else
            {
                DX_CHECK(D3D9Device->SetTexture(stage, NULL));
            }
        }

        currentTextureBind = textureBind;
    }
}

/*effBOOL EFFD3D9Device::SetTextureStageState(effUINT stage, EFF3DTEXTURESTAGESTATETYPE type, effUINT value)
{
	return SUCCEEDED(D3D9Device->SetTextureStageState(stage, (D3DTEXTURESTAGESTATETYPE)type, value));
}

effBOOL EFFD3D9Device::SetSamplerState(effUINT Sampler, EFF3DSAMPLERSTATETYPE Type, effUINT Value)
{
	return SUCCEEDED(D3D9Device->SetSamplerState(Sampler, (D3DSAMPLERSTATETYPE)Type, Value)); 
}*/

effBOOL EFFD3D9Device::SetRenderTarget(effUINT renderTargetIndex, EFF3DTextureHandle renderTargetHandle)
{
    //EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)renderTarget;
    EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)textureManager->GetResource(renderTargetHandle);
    currentRenderTarget = effD3D9Texture;

    LPDIRECT3DSURFACE9 surface = NULL;
    effD3D9Texture->texture->GetSurfaceLevel(0, &surface);

	//EFFD3D9Surface * effD3D9Surface = (EFFD3D9Surface *)renderTarget;
    effHRESULT hr = D3D9Device->SetRenderTarget(renderTargetIndex, surface);

    surface->Release();



	return SUCCEEDED(hr);
}

effBOOL EFFD3D9Device::SetTexture(effUINT sampler, EFF3DTextureHandle textureHandle)
{
    EFFD3D9Texture * texture = NULL;

	if ( texture != NULL )
	{
		if ( texture->GetImageInfo().type == EFF3DResourceType_Texture2D )
		{
			EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)texture;
			return SUCCEEDED(D3D9Device->SetTexture(sampler, (IDirect3DTexture9 *)effD3D9Texture->texture));
		}
		return effTRUE;
	}
	else
	{
		return SUCCEEDED(D3D9Device->SetTexture(sampler, NULL));
	}
}

effBOOL EFFD3D9Device::SetDepthStencil(EFF3DTextureHandle depthStencilHandle)
{
    //EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)depthStencil;
    EFFD3D9Texture * effD3D9Texture = NULL;


    LPDIRECT3DSURFACE9 surface = NULL;
    effD3D9Texture->texture->GetSurfaceLevel(0, &surface);


	effHRESULT hr = D3D9Device->SetDepthStencilSurface(surface);

    surface->Release();

    return SUCCEEDED(hr);
}

effBOOL EFFD3D9Device::SetShader(EFF3DShader * shader)
{
	EFFD3D9Shader * effD3D9Shader = (EFFD3D9Shader *)shader;

	//cgD3D9BindProgram(effD3D9Shader->GetVertexShader());
	//cgD3D9BindProgram(effD3D9Shader->GetPixelShader());

	return effTRUE;
}

effBOOL EFFD3D9Device::SetScissorRect(const EFFRect * rect)
{
	RECT longRect;
	longRect.left = (long)rect->left;
	longRect.top = (long)rect->top;
	longRect.right = (long)rect->right;
	longRect.bottom = (long)rect->bottom;

	return SUCCEEDED(D3D9Device->SetScissorRect(&longRect));
}

effVOID EFFD3D9Device::Release()
{
	delete this;
}

EFF3DTextureHandle EFFD3D9Device::GetRenderTarget(effUINT index)
{
    return currentRenderTarget->id;
}


/*effBOOL EFFD3D9Device::GetViewport(EFF3DVIEWPORT9 * viewport)
{
	return SUCCEEDED(D3D9Device->GetViewport((D3DVIEWPORT9 *)viewport));
}*/


/*effBOOL	EFFD3D9Device::CheckFormatSupport(effUINT * width, effUINT * height, effUINT * numMipLevels, effUINT usage, EFF3DFORMAT * format, EFF3DPOOL pool)
{
	return SUCCEEDED(D3DXCheckTextureRequirements(D3D9Device, width, height, numMipLevels, usage, (D3DFORMAT *)format, (D3DPOOL)pool));
}*/

effBOOL EFFD3D9Device::HasVertexStreamChanged()
{
	if (currentDrawCommand.streamMask != newDrawCommand->streamMask
		|| currentDrawCommand.instanceDataBufferHandle != newDrawCommand->instanceDataBufferHandle
		|| currentDrawCommand.instanceDataOffset != newDrawCommand->instanceDataOffset
		|| currentDrawCommand.instanceDataStride != newDrawCommand->instanceDataStride)
	{
		return effTRUE;
	}

	for (effUINT idx = 0, streamMask = newDrawCommand->streamMask, ntz = effUINT_cnttz(streamMask)
		; 0 != streamMask
		; streamMask >>= 1, idx += 1, ntz = effUINT_cnttz(streamMask)
		)
	{
		streamMask >>= ntz;
		idx += ntz;

		if (currentDrawCommand.stream[idx].vertexBufferHandle != newDrawCommand->stream[idx].vertexBufferHandle
			|| currentDrawCommand.stream[idx].startVertex != newDrawCommand->stream[idx].startVertex)
		{
			return effTRUE;
		}
	}

	return effFALSE;
}


