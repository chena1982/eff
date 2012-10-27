/******************************************************************************
	created:	2008-12-2   0:21
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Device.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFD3D9Device.h"
#include "EFFD3D9Texture.h"
#include "EFFD3D9Surface.h"
#include "EFFD3D9IndexBuffer.h"
#include "EFFD3D9VertexBuffer.h"
#include "EFFD3D9VertexDeclaration.h"
#include "EFFD3D9Shader.h"

//#define new EFFNEW


effVOID InitWindow(effINT width, effINT height, D3DPRESENT_PARAMETERS *d3dpp, effBOOL enableAA, effUINT depthStencilFormat)  
{
	d3dpp->Windowed = TRUE;
	d3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp->BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp->BackBufferWidth = width;
	d3dpp->BackBufferHeight = height;
	d3dpp->AutoDepthStencilFormat = (D3DFORMAT)depthStencilFormat;
	if( enableAA )
	{
		d3dpp->MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
	}

	d3dpp->EnableAutoDepthStencil = TRUE;
	d3dpp->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

effVOID InitFullScreen(effINT width, effINT height, D3DPRESENT_PARAMETERS * d3dpp, HWND hWnd)
{

	d3dpp->Windowed = effFALSE;
	d3dpp->BackBufferCount = 1;
	d3dpp->BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp->BackBufferWidth = width;
	d3dpp->BackBufferHeight = height;
	d3dpp->hDeviceWindow = hWnd;
	d3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp->AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp->EnableAutoDepthStencil = TRUE;
}


effBOOL effCreate3DDevice(EFF3DDevice ** eff3DDevice, effBOOL window, HWND hWnd, effINT width, effINT height)
{

	LPDIRECT3D9 D3D;
	if ( (D3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL )
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

	D3D->GetAdapterIdentifier(AdapterToUse,0,&Identifier);
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



	if( !window )
	{
		InitFullScreen(width, height, &d3dpp, hWnd);


		if( FAILED(D3D->CreateDevice(AdapterToUse, DeviceType, hWnd, behaviorFlags, &d3dpp, &D3DDevice->GetD3D9Device())) )
		{
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;

			//如果创建D3DFMT_X8R8G8B8缓冲模式失败，试试D3DFMT_X1R5G5B5模式
			if( FAILED(D3D->CreateDevice(AdapterToUse, DeviceType, hWnd, behaviorFlags, &d3dpp, &D3DDevice->GetD3D9Device())) )
			{
				SF_RELEASE(D3DDevice);
				return effFALSE;
			}
		}
	}
	else
	{
		InitWindow(width, height, &d3dpp, effFALSE, (effUINT)EFF3DFMT_D24S8);

		if( FAILED(D3D->CreateDevice(AdapterToUse, DeviceType, hWnd, behaviorFlags, &d3dpp, &D3DDevice->GetD3D9Device())) )
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

	CGcontext cgContext = cgCreateContext();
	D3DDevice->SetCGContex(cgContext);
	cgD3D9SetDevice(D3DDevice->GetD3D9Device());

	*eff3DDevice =	D3DDevice;

	return effTRUE;
}

EFFD3D9Device::EFFD3D9Device()
{
	D3D9 = NULL;       
	D3D9Device = NULL;
}

EFFD3D9Device::~EFFD3D9Device()
{
	cgD3D9SetDevice(NULL);
	cgDestroyContext(cgContext);

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
	return SUCCEEDED(D3D9Device->Present((const RECT *)sourceRect, (const RECT *)destRect, NULL, NULL));
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

	std::map<effUINT, EFFObject *> & images = GetImageManager()->GetObjects();
	std::map<effUINT, EFFObject *>::iterator imageIt = images.begin();
	for ( ; imageIt != images.end(); imageIt++ )
	{
		EFF3DImage * image = reinterpret_cast<EFF3DImage *>(imageIt->second);
		EFF3DIMAGE_INFO & imageInfo = image->GetImageInfo();

		if ( imageInfo.pool == EFF3DPOOL_DEFAULT )
		{
			image->Unload();
			continue;
		}

		if ( imageInfo.resourceType == EFF3DRTYPE_SURFACE )
		{
			if ( imageInfo.surfaceType == DepthStencil_Surface || imageInfo.surfaceType == GetFromTexture_Surface )
			{
				EFF3DSurface * surface = reinterpret_cast<EFF3DSurface *>(imageIt->second);
				surface->Unload();
				continue;
			}

			/*if ( imageInfo.surfaceType == RenderTo_Surface )
			{
				EFF3DRenderToSurface * renderToSurface = reinterpret_cast<EFF3DRenderToSurface *>(it->second);
				renderToSurface->UnloadResource();
				continue;
			}*/

		}
		//it->second->UnloadResource();
	}


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

	imageIt = images.begin();
	for ( ; imageIt != images.end(); imageIt++ )
	{
		EFF3DImage * image = reinterpret_cast<EFF3DImage *>(imageIt->second);
		if ( image->IsUnloaded() )
		{
			if ( !image->Reload() )
			{
				return effFALSE;
			}
		}
	}

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

EFF3DResource * EFFD3D9Device::CreateEmptyResource(EFF3DRESOURCETYPE resourceType)
{
	EFF3DResource * resource = NULL;
	
	switch(resourceType)
	{
	case EFF3DRTYPE_TEXTURE:
		{
			resource = EFFNEW EFFD3D9Texture();
		}
		break;
	default:
		break;
	}

	return resource;
}

effBOOL EFFD3D9Device::CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT usage, EFF3DFORMAT format, EFF3DPOOL pool, EFF3DTexture** texture)
{
	assert(texture != NULL);

	EFFD3D9Texture * effD3D9Texture = new EFFD3D9Texture();

	effHRESULT hr;

	if( FAILED(hr = D3D9Device->CreateTexture(width, height, levels, usage, (D3DFORMAT)format, (D3DPOOL)pool, &effD3D9Texture->m_pTexture, NULL)) )
	{
		SF_DELETE(effD3D9Texture);
		return effFALSE;
	}

	effD3D9Texture->m_ImageInfo.width = width;
	effD3D9Texture->m_ImageInfo.height = height;
	effD3D9Texture->m_ImageInfo.mipLevels = levels;
	effD3D9Texture->m_ImageInfo.usage = usage;
	effD3D9Texture->m_ImageInfo.format = (EFF3DFORMAT)format;
	effD3D9Texture->m_ImageInfo.pool = (EFF3DPOOL)pool;
	effD3D9Texture->m_ImageInfo.resourceType = EFF3DRTYPE_TEXTURE;

	effD3D9Texture->AddRef();
	*texture = effD3D9Texture;

	return effTRUE;	
}

effBOOL	EFFD3D9Device::CreateTextureFromFile(const effString & filePath, EFF3DTexture ** texture)
{
	assert(texture != NULL);

	EFFD3D9Texture * effD3D9Texture = EFFNEW EFFD3D9Texture();

	if( FAILED(D3DXCreateTextureFromFile(D3D9Device, filePath.c_str(), &effD3D9Texture->m_pTexture)) )
	{
		SF_DELETE(effD3D9Texture);
		return effFALSE;
	}

	D3DSURFACE_DESC desc;
	effD3D9Texture->m_pTexture->GetLevelDesc(0, &desc);
	effD3D9Texture->m_ImageInfo.width = desc.Width;
	effD3D9Texture->m_ImageInfo.height = desc.Height;
	effD3D9Texture->m_ImageInfo.usage = desc.Usage;
	effD3D9Texture->m_ImageInfo.format = (EFF3DFORMAT)desc.Format;
	effD3D9Texture->m_ImageInfo.pool = (EFF3DPOOL)desc.Pool;
	effD3D9Texture->m_ImageInfo.resourceType = (EFF3DRESOURCETYPE)desc.Type;


	effD3D9Texture->AddRef();
	*texture = effD3D9Texture;

	return effTRUE;	
}

effBOOL EFFD3D9Device::CreateRenderTarget(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample, effUINT multisampleQuality, effBOOL lockable, EFF3DSurface ** surface)
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
}

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
	return SUCCEEDED(D3D9Device->DrawPrimitive((D3DPRIMITIVETYPE)primitiveType, startVertex, primitiveCount));
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

effBOOL EFFD3D9Device::SetRenderState(EFF3DRENDERSTATETYPE state, effUINT value)
{
	return SUCCEEDED(D3D9Device->SetRenderState((D3DRENDERSTATETYPE)state, value));
}

effBOOL EFFD3D9Device::SetTextureStageState(effUINT stage, EFF3DTEXTURESTAGESTATETYPE type, effUINT value)
{
	return SUCCEEDED(D3D9Device->SetTextureStageState(stage, (D3DTEXTURESTAGESTATETYPE)type, value));
}

effBOOL EFFD3D9Device::SetRenderTarget(effUINT renderTargetIndex, EFF3DSurface * renderTarget)
{
	EFFD3D9Surface * effD3D9Surface = (EFFD3D9Surface *)renderTarget;
	return SUCCEEDED(D3D9Device->SetRenderTarget(renderTargetIndex, effD3D9Surface->m_pSurface));
}

effBOOL EFFD3D9Device::SetTexture(effUINT sampler, EFF3DBaseTexture * texture)
{
	if ( texture != NULL )
	{
		if ( texture->GetImageInfo().resourceType  == EFF3DRTYPE_TEXTURE )
		{
			EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)texture;
			return SUCCEEDED(D3D9Device->SetTexture(sampler, (IDirect3DTexture9 *)effD3D9Texture->m_pTexture));
		}
		return effTRUE;
	}
	else
	{
		return SUCCEEDED(D3D9Device->SetTexture(sampler, NULL));
	}
}

effBOOL EFFD3D9Device::SetDepthStencilSurface(EFF3DSurface * newZStencil)
{
	EFFD3D9Surface * effD3D9Surface = (EFFD3D9Surface *)newZStencil;
	return SUCCEEDED(D3D9Device->SetDepthStencilSurface(effD3D9Surface->m_pSurface));
}

effBOOL EFFD3D9Device::SetShader(EFF3DShader * shader)
{
	EFFD3D9Shader * effD3D9Shader = (EFFD3D9Shader *)shader;

	cgD3D9BindProgram(effD3D9Shader->GetVertexShader());
	cgD3D9BindProgram(effD3D9Shader->GetPixelShader());

	return effTRUE;
}

effVOID EFFD3D9Device::Release()
{
	delete this;
}