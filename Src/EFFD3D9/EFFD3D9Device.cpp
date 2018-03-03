/******************************************************************************
	created:	2008-12-2   0:21
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Device.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D9PCH.h"
#include "EFFD3D9Device.h"
#include "EFFD3D9Texture.h"
#include "EFFD3D9Surface.h"
#include "EFFD3D9IndexBuffer.h"
#include "EFFD3D9VertexBuffer.h"
#include "EFFD3D9VertexDeclaration.h"
#include "EFFD3D9Shader.h"
#include "EFFD3D9Query.h"

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
	d3dpp->AutoDepthStencilFormat = (D3DFORMAT)depthStencilFormat;



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

	d3dpp->AutoDepthStencilFormat = (D3DFORMAT)EFF3DFMT_D24S8;
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


		if ( FAILED(D3D->CreateDeviceEx(AdapterToUse, DeviceType, hWnd, behaviorFlags, &d3dpp, NULL, &D3DDevice->GetD3D9Device())) )
		{
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;

			//如果创建D3DFMT_X8R8G8B8缓冲模式失败，试试D3DFMT_X1R5G5B5模式
			if( FAILED(D3D->CreateDeviceEx(AdapterToUse, DeviceType, hWnd, behaviorFlags, &d3dpp, NULL, &D3DDevice->GetD3D9Device())) )
			{
				SF_RELEASE(D3DDevice);
				return effFALSE;
			}
		}
	}
	else
	{
		InitWindow(width, height, &d3dpp, effFALSE, (effUINT)EFF3DFMT_D24S8);

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
    EFF3DImageInfo & imageInfo = image->GetImageInfo();

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
		{
			resource = EFFNEW EFFD3D9Texture();
		}
		break;
	default:
		break;
	}

	return resource;
}

effBOOL EFFD3D9Device::CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT flag, EFF3DTextureFormat format, EFF3DResourceType resourceType, EFF3DTexture * texture)
{
	assert(texture != NULL);

    EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)texture;

	effHRESULT hr;

    effUINT usage = 0;

    effHANDLE shareHandle = NULL;
    if (texture->userData != NULL)
    {
        shareHandle = texture->userData;
    }

	if( FAILED(hr = D3D9Device->CreateTexture(width, height, levels, usage, s_textureFormat[format].m_fmt, D3DPOOL_DEFAULT, &effD3D9Texture->texture, &shareHandle)) )
	{
		SF_DELETE(effD3D9Texture);
		return effFALSE;
	}

    texture->userData = shareHandle;

	effD3D9Texture->imageInfo.width = width;
	effD3D9Texture->imageInfo.height = height;
	effD3D9Texture->imageInfo.mipLevels = levels;
	effD3D9Texture->imageInfo.format = format;
	effD3D9Texture->imageInfo.type = resourceType;

	effD3D9Texture->AddRef();


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
                                            EFF3DTextureFormat format, EFF3DResourceType resourceType, EFF3DTexture * texture)
{
	assert(texture != NULL);

	effINT pixelSize = EFF3DGetPixelSizeFromFormat(format);
	if ( width * height * pixelSize > (effINT)srcDataSize )
	{
		return effFALSE;
	}


	EFFD3D9Texture * effD3D9Texture = EFFNEW EFFD3D9Texture();
    effHANDLE shareHandle = NULL;
	if ( FAILED(D3D9Device->CreateTexture(width, height, level, 0, s_textureFormat[format].m_fmt, D3DPOOL_DEFAULT, &effD3D9Texture->texture, &shareHandle)) )
	{
		SF_DELETE(effD3D9Texture);
		return effFALSE;
	}

    texture->userData = shareHandle;

	EFF3DLOCKED_RECT rc;
	if ( FAILED(effD3D9Texture->LockRect(0, &rc, NULL, 0)) )
	{
		SF_DELETE(effD3D9Texture);
		return effFALSE;
	}

	if ( rc.Pitch < pixelSize * width )
	{
		SF_DELETE(effD3D9Texture);
		effD3D9Texture->UnlockRect(0);
		return effFALSE;
	}

	for ( effINT i = 0; i < height; i++ )
	{
		memcpy(((effBYTE *)rc.pBits) + rc.Pitch * i, ((effBYTE *)srcData) + i * width * pixelSize, width * pixelSize);
	}

	effD3D9Texture->UnlockRect(0);

	effD3D9Texture->imageInfo.width = width;
	effD3D9Texture->imageInfo.height = height;
	//effD3D9Texture->imageInfo.usage = 0;
	effD3D9Texture->imageInfo.format = format;
	//effD3D9Texture->imageInfo.pool = EFF3DPOOL_DEFAULT;
	effD3D9Texture->imageInfo.type = resourceType;


	effD3D9Texture->AddRef();

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

effBOOL EFFD3D9Device::CreateIndexBuffer(effUINT length, effUINT usage, EFF3DFORMAT format, EFF3DPOOL pool, EFF3DIndexBuffer ** indexBuffer)
{
	assert(indexBuffer != NULL);
	
	EFFD3D9IndexBuffer * effD3D9IndexBuffer = EFFNEW EFFD3D9IndexBuffer();
	effHRESULT hr;
	if ( FAILED(hr = D3D9Device->CreateIndexBuffer(length, usage, (D3DFORMAT)format, (D3DPOOL)pool, &effD3D9IndexBuffer->m_pBuf, NULL)) )
	{
		SF_DELETE(effD3D9IndexBuffer);
		return effFALSE;
	}

	*indexBuffer = effD3D9IndexBuffer;
	return effTRUE;
}

effBOOL EFFD3D9Device::CreateVertexBuffer(effUINT length, effUINT usage, effUINT FVF, EFF3DPOOL pool, EFF3DVertexBuffer ** vertexBuffer)
{
	assert(vertexBuffer != NULL);

	EFFD3D9VertexBuffer * effD3D9VertexBuffer = EFFNEW EFFD3D9VertexBuffer();
	effHRESULT hr;
	if ( FAILED(hr = D3D9Device->CreateVertexBuffer(length, usage, FVF, (D3DPOOL)pool, &effD3D9VertexBuffer->m_pBuf, NULL)) )
	{
		SF_DELETE(effD3D9VertexBuffer);
		return effFALSE;
	}

	*vertexBuffer = effD3D9VertexBuffer;
	return effTRUE;
}

effBOOL EFFD3D9Device::CreateVertexDeclaration(const EFF3DVERTEXELEMENT * vertexElements, EFF3DVertexDeclaration ** decl)
{
	assert(decl != NULL);

	EFFD3D9VertexDeclaration * effD3D9Decl = EFFNEW EFFD3D9VertexDeclaration();

	*decl = effD3D9Decl;
	return effTRUE;
}

effBOOL EFFD3D9Device::CreateQuery(EFF3DQUERYTYPE type, effUINT flag, EFF3DQuery ** query)
{

    EFFD3D9Query * effD3D9Query = EFFNEW EFFD3D9Query();

    effHRESULT hr;
    if (FAILED(hr = D3D9Device->CreateQuery((D3DQUERYTYPE)type, &effD3D9Query->query)))
    {
        SF_DELETE(effD3D9Query);
        return effFALSE;
    }

    *query = effD3D9Query;
    return effTRUE;
}

effBOOL EFFD3D9Device::DrawIndexedPrimitive(EFF3DPRIMITIVETYPE type, effINT baseVertexIndex, effUINT minIndex, effUINT numVertices, effUINT startIndex, effUINT primitiveCount)
{
	return SUCCEEDED(D3D9Device->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount));
}

effBOOL EFFD3D9Device::DrawIndexedPrimitiveUP(EFF3DPRIMITIVETYPE primitiveType, effUINT minVertexIndex, effUINT numVertices, effUINT primitiveCount, const effVOID * indexData, EFF3DFORMAT indexDataFormat, const effVOID * vertexStreamZeroData, effUINT vertexStreamZeroStride)
{
	return SUCCEEDED(D3D9Device->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)primitiveType, minVertexIndex, numVertices, primitiveCount, indexData, (D3DFORMAT)indexDataFormat, vertexStreamZeroData, vertexStreamZeroStride));
}

effBOOL EFFD3D9Device::DrawPrimitive(EFF3DPRIMITIVETYPE primitiveType, effUINT startVertex, effUINT primitiveCount)
{
	HRESULT hr = D3D9Device->DrawPrimitive((D3DPRIMITIVETYPE)primitiveType, startVertex, primitiveCount);
	return SUCCEEDED(hr);
}

effBOOL EFFD3D9Device::DrawPrimitiveUP(EFF3DPRIMITIVETYPE primitiveType, effUINT primitiveCount, const effVOID * vertexStreamZeroData, effUINT vertexStreamZeroStride)
{
	return SUCCEEDED(D3D9Device->DrawPrimitiveUP((D3DPRIMITIVETYPE)primitiveType, primitiveCount, vertexStreamZeroData, vertexStreamZeroStride));
}

effBOOL EFFD3D9Device::SetTransform(EFF3DTRANSFORMSTATETYPE state, const EFFMatrix4 * matrix)
{
	return SUCCEEDED(D3D9Device->SetTransform((D3DTRANSFORMSTATETYPE)state, (const D3DMATRIX *)matrix));
}

effBOOL EFFD3D9Device::SetFVF(effUINT FVF)
{
	return SUCCEEDED(D3D9Device->SetFVF(FVF));
}

effBOOL EFFD3D9Device::SetVertexDeclaration(EFF3DVertexDeclaration * decl)
{
	EFFD3D9VertexDeclaration * effD3D9Decl = (EFFD3D9VertexDeclaration *)decl;
	return SUCCEEDED(D3D9Device->SetVertexDeclaration(effD3D9Decl->GetD3D9VertexDeclaration(this)));
}

effBOOL EFFD3D9Device::SetStreamSource(effUINT streamNumber, EFF3DVertexBuffer * streamData, effUINT offsetInBytes, effUINT stride)
{
	EFFD3D9VertexBuffer * effD3D9VB = (EFFD3D9VertexBuffer *)streamData;
	return SUCCEEDED(D3D9Device->SetStreamSource(streamNumber, effD3D9VB->m_pBuf, offsetInBytes, stride));
}

effBOOL EFFD3D9Device::SetIndices(EFF3DIndexBuffer * indexData)
{
	EFFD3D9IndexBuffer * effD3D9IB = (EFFD3D9IndexBuffer *)indexData;
	return SUCCEEDED(D3D9Device->SetIndices(effD3D9IB->m_pBuf));
}

//effBOOL EFFD3D9Device::SetRenderState(EFF3DRENDERSTATETYPE state, effUINT value)
//{
//	return SUCCEEDED(D3D9Device->SetRenderState((D3DRENDERSTATETYPE)state, value));
//}

effVOID EFFD3D9Device::SetRenderState(effUINT64 State, effUINT32 Color)
{

    if (CurrentState == State)
    {
        return;
    }

    effUINT64 changedFlags = CurrentState ^ State;

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
            effUINT32 cull = (State & EFF3D_STATE_CULL_MASK) >> EFF3D_STATE_CULL_SHIFT;
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_CULLMODE, s_cullMode[cull]));
        }

        if (EFF3D_STATE_WRITE_Z & changedFlags)
        {
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_ZWRITEENABLE, !!(EFF3D_STATE_WRITE_Z & State)));
        }

        if (EFF3D_STATE_DEPTH_TEST_MASK & changedFlags)
        {
            effUINT32 func = (State & EFF3D_STATE_DEPTH_TEST_MASK) >> EFF3D_STATE_DEPTH_TEST_SHIFT;
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_ZENABLE, 0 != func));

            if (0 != func)
            {
                DX_CHECK(D3D9Device->SetRenderState(D3DRS_ZFUNC, s_cmpFunc[func]));
            }
        }

        if (EFF3D_STATE_ALPHA_REF_MASK & changedFlags)
        {
            effUINT32 ref = (State & EFF3D_STATE_ALPHA_REF_MASK) >> EFF3D_STATE_ALPHA_REF_SHIFT;
            //viewState.m_alphaRef = ref / 255.0f;
        }

        if ((EFF3D_STATE_PT_POINTS | EFF3D_STATE_POINT_SIZE_MASK) & changedFlags)
        {
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_POINTSIZE, (effUINT32)((State & EFF3D_STATE_POINT_SIZE_MASK) >> EFF3D_STATE_POINT_SIZE_SHIFT)));
        }

        if (EFF3D_STATE_MSAA & changedFlags)
        {
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, (State & EFF3D_STATE_MSAA) == EFF3D_STATE_MSAA));
        }

        if (EFF3D_STATE_LINEAA & changedFlags)
        {
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, !!(State & EFF3D_STATE_LINEAA)));
        }

        if ((EFF3D_STATE_WRITE_A | EFF3D_STATE_WRITE_RGB) & changedFlags)
        {
            effUINT32 writeEnable = 0;
            writeEnable |= (State & EFF3D_STATE_WRITE_R) ? D3DCOLORWRITEENABLE_RED : 0;
            writeEnable |= (State & EFF3D_STATE_WRITE_G) ? D3DCOLORWRITEENABLE_GREEN : 0;
            writeEnable |= (State & EFF3D_STATE_WRITE_B) ? D3DCOLORWRITEENABLE_BLUE : 0;
            writeEnable |= (State & EFF3D_STATE_WRITE_A) ? D3DCOLORWRITEENABLE_ALPHA : 0;
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_COLORWRITEENABLE, writeEnable));
        }

        if (((EFF3D_STATE_BLEND_MASK
            | EFF3D_STATE_BLEND_EQUATION_MASK
            | EFF3D_STATE_BLEND_ALPHA_TO_COVERAGE
            ) & changedFlags)
            || BlendFactor != Color)
        {
            effBOOL enabled = !!(EFF3D_STATE_BLEND_MASK & State);
            DX_CHECK(D3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, enabled));

            if (AtocSupport && EFF3D_STATE_BLEND_ALPHA_TO_COVERAGE & changedFlags)
            {
                DX_CHECK(D3D9Device->SetRenderState(D3DRS_ADAPTIVETESS_Y, !!(State & EFF3D_STATE_BLEND_ALPHA_TO_COVERAGE) ? D3DFMT_ATOC : 0));
            }

            if (enabled)
            {
                const effUINT32 blend = uint32_t((State & EFF3D_STATE_BLEND_MASK) >> EFF3D_STATE_BLEND_SHIFT);
                const effUINT32 equation = uint32_t((State & EFF3D_STATE_BLEND_EQUATION_MASK) >> EFF3D_STATE_BLEND_EQUATION_SHIFT);

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
                    && BlendFactor != Color)
                {
                    const effUINT32 rgba = Color;
                    D3DCOLOR color = D3DCOLOR_RGBA(rgba >> 24
                        , (rgba >> 16) & 0xff
                        , (rgba >> 8) & 0xff
                        , (rgba) & 0xff
                    );
                    DX_CHECK(D3D9Device->SetRenderState(D3DRS_BLENDFACTOR, color));
                }
            }

            BlendFactor = Color;
        }
    }

    CurrentState = State;
}

effBOOL EFFD3D9Device::SetTextureStageState(effUINT stage, EFF3DTEXTURESTAGESTATETYPE type, effUINT value)
{
	return SUCCEEDED(D3D9Device->SetTextureStageState(stage, (D3DTEXTURESTAGESTATETYPE)type, value));
}

effBOOL EFFD3D9Device::SetSamplerState(effUINT Sampler, EFF3DSAMPLERSTATETYPE Type, effUINT Value)
{
	return SUCCEEDED(D3D9Device->SetSamplerState(Sampler, (D3DSAMPLERSTATETYPE)Type, Value)); 
}

effBOOL EFFD3D9Device::SetRenderTarget(effUINT renderTargetIndex, EFF3DTexture * renderTarget)
{
    EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)renderTarget;
    CurrentRenderTarget = effD3D9Texture;

    LPDIRECT3DSURFACE9 surface = NULL;
    effD3D9Texture->texture->GetSurfaceLevel(0, &surface);

	//EFFD3D9Surface * effD3D9Surface = (EFFD3D9Surface *)renderTarget;
    effHRESULT hr = D3D9Device->SetRenderTarget(renderTargetIndex, surface);

    surface->Release();



	return SUCCEEDED(hr);
}

effBOOL EFFD3D9Device::SetTexture(effUINT sampler, EFF3DImage * texture)
{
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

effBOOL EFFD3D9Device::SetDepthStencil(EFF3DTexture * depthStencil)
{
    EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)depthStencil;

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
    return CurrentRenderTarget->id;
}


effBOOL EFFD3D9Device::GetViewport(EFF3DVIEWPORT9 * viewport)
{
	return SUCCEEDED(D3D9Device->GetViewport((D3DVIEWPORT9 *)viewport));
}


effBOOL	EFFD3D9Device::CheckFormatSupport(effUINT * width, effUINT * height, effUINT * numMipLevels, effUINT usage, EFF3DFORMAT * format, EFF3DPOOL pool)
{
	return SUCCEEDED(D3DXCheckTextureRequirements(D3D9Device, width, height, numMipLevels, usage, (D3DFORMAT *)format, (D3DPOOL)pool));
}