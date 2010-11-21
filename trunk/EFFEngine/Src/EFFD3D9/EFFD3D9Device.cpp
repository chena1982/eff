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

#define new EFFNEW


effVOID InitWindow(effINT nWidth,effINT nHeight,D3DPRESENT_PARAMETERS *d3dpp,effBOOL bFSA,effUINT dwDSFormat)  
{
	d3dpp->Windowed = effTRUE;
	d3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp->BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp->BackBufferWidth= nWidth;
	d3dpp->BackBufferHeight=nHeight;
	d3dpp->AutoDepthStencilFormat = (D3DFORMAT)dwDSFormat;
	if( bFSA )
	{
		d3dpp->MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
	}

	d3dpp->EnableAutoDepthStencil = effTRUE;
	d3dpp->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

effVOID InitFullScreen(effINT nWidth,effINT nHeight,D3DPRESENT_PARAMETERS *d3dpp,HWND hWnd)
{

	d3dpp->Windowed=effFALSE;
	d3dpp->BackBufferCount=1;
	d3dpp->BackBufferFormat=D3DFMT_X8R8G8B8;
	d3dpp->BackBufferWidth= nWidth;
	d3dpp->BackBufferHeight=nHeight;
	d3dpp->hDeviceWindow=hWnd;
	d3dpp->SwapEffect=D3DSWAPEFFECT_DISCARD;

	d3dpp->AutoDepthStencilFormat=D3DFMT_D16;
	d3dpp->EnableAutoDepthStencil=TRUE;
}


effBOOL effCreate3DDevice(EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA /* = effFALSE */,effBOOL bMultiThread /* = effFALSE */,effUINT dwDSFormat/* = EFF3DFMT_D16*/)
{

	LPDIRECT3D9 pD3D;
	if( (pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL )
	{
		return effFALSE;
	}

	EFFD3D9Device *pD3DDevice = new EFFD3D9Device;
	pD3DDevice->SetD3D(pD3D);
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));



	UINT AdapterToUse = D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;
#ifdef SHIPPING_VERSION
	// When building a shipping version, disable NVPerfHUD (opt-out)
#else
	// Look for 'NVIDIA NVPerfHUD' adapter
	// If it is present, override default settings
	for ( UINT Adapter=0; Adapter < pD3D->GetAdapterCount(); Adapter++ )
	{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT Res;
		Res = pD3D->GetAdapterIdentifier(Adapter,0,&Identifier);

		if ( strcmp(Identifier.Description,"NVIDIA PerfHUD") == 0 )
		{
			AdapterToUse=Adapter;
			DeviceType=D3DDEVTYPE_REF;

			break;
		}
	}
#endif


	D3DADAPTER_IDENTIFIER9 Identifier;

	pD3D->GetAdapterIdentifier(AdapterToUse,0,&Identifier);
	//pD3DDevice->m_strDeviceName = Identifier.Description;

	D3DCAPS9 d3dCaps;
	pD3D->GetDeviceCaps(AdapterToUse,DeviceType,&d3dCaps);
	effUINT dwBehaviorFlags;
	if ( (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 )
	{
		dwBehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else
	{
		dwBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	if ( bMultiThread ) 
	{
		dwBehaviorFlags |= D3DCREATE_MULTITHREADED;
	}



	if( !bWindow )
	{
		InitFullScreen(nWidth,nHeight,&d3dpp,hWnd);


		if( FAILED(pD3D->CreateDevice( AdapterToUse,DeviceType,hWnd,dwBehaviorFlags,&d3dpp,&pD3DDevice->GetD3DDevice())) )
		{
			d3dpp.BackBufferFormat=D3DFMT_X8R8G8B8;

			//如果创建D3DFMT_X8R8G8B8缓冲模式失败，试试D3DFMT_X1R5G5B5模式
			if( FAILED(pD3D ->CreateDevice(AdapterToUse,DeviceType,hWnd,dwBehaviorFlags,&d3dpp,&pD3DDevice->GetD3DDevice())) )
			{
				SF_RELEASE(pD3DDevice);
				return effFALSE;
			}
		}
	}
	else
	{
		InitWindow(nWidth,nHeight,&d3dpp,bFSA,dwDSFormat);

		if( FAILED(pD3D->CreateDevice(AdapterToUse,DeviceType,hWnd,dwBehaviorFlags,&d3dpp,&pD3DDevice->GetD3DDevice())) )
		{
			SF_RELEASE(pD3DDevice);
			return effFALSE;
		}
	}

	IDirect3DSurface9 * pBackBuffer = NULL;
	if ( FAILED(pD3DDevice->GetD3DDevice()->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer)) )
	{
		SF_RELEASE(pD3DDevice);
		return effFALSE;
	};
	D3DSURFACE_DESC stBackBufferDesc;
	pBackBuffer->GetDesc(&stBackBufferDesc);
	pD3DDevice->SetBackBufferSize(stBackBufferDesc.Width,stBackBufferDesc.Height);
	SF_RELEASE(pBackBuffer);

	*lpp3DDevice =	pD3DDevice;

	return effTRUE;
}

EFFD3D9Device::EFFD3D9Device()
{
	m_pD3D = NULL;       
	m_pD3DDevice = NULL;
}

EFFD3D9Device::~EFFD3D9Device()
{
}


effHRESULT EFFD3D9Device::BeginScene()
{
	return m_pD3DDevice->BeginScene();
}

effHRESULT EFFD3D9Device::EndScene()
{
	return m_pD3DDevice->EndScene();
}

effHRESULT EFFD3D9Device::Clear(effUINT Count,const EFFRect * pRects,effUINT Flags,EFF3DCOLOR Color,effFLOAT Z,effUINT Stencil)
{
	return m_pD3DDevice->Clear(Count,(const D3DRECT *)pRects,Flags,Color,Z,Stencil);
}

effHRESULT EFFD3D9Device::Present(const EFFRect * pSourceRect,const EFFRect * pDestRect)
{
	return m_pD3DDevice->Present((const RECT *)pSourceRect,(const RECT *)pDestRect,NULL,NULL);
}

EFF3DIResource * EFFD3D9Device::CreateEmptyResource(EFF3DRESOURCETYPE resourceType)
{
	EFF3DIResource * pRet = NULL;
	
	switch(resourceType)
	{
	case EFF3DRTYPE_TEXTURE:
		{
			pRet = new EFFD3D9Texture();
		}
		break;
	default:
		break;
	}

	return pRet;
}

effHRESULT EFFD3D9Device::CreateTexture(effUINT Width,effUINT Height,effUINT Levels,effUINT Usage,EFF3DFORMAT Format,EFF3DPOOL Pool, EFF3DTexture** ppTexture)
{
	assert(ppTexture != NULL);

	EFFD3D9Texture * pTexture = new EFFD3D9Texture;

	effHRESULT hr;

	if( FAILED(hr = m_pD3DDevice->CreateTexture(Width,Height,Levels,Usage,(D3DFORMAT)Format,(D3DPOOL)Pool,&pTexture->m_pTexture,NULL)) )
	{
		SF_DELETE(pTexture);
		return hr;
	}

	pTexture->m_ImageInfo.Width = Width;
	pTexture->m_ImageInfo.Height = Height;
	pTexture->m_ImageInfo.MipLevels = Levels;
	pTexture->m_ImageInfo.Usage = Usage;
	pTexture->m_ImageInfo.Format = (EFF3DFORMAT)Format;
	pTexture->m_ImageInfo.Pool = (EFF3DPOOL)Pool;


	//m_pImageManager->AddFirstCreateResource(pTexture,std::string(_ucT("Texture")));

	*ppTexture = pTexture;
	return hr;	
}

effHRESULT EFFD3D9Device::CreateRenderTarget(effUINT Width, effUINT Height, EFF3DFORMAT Format, EFF3DMULTISAMPLE_TYPE MultiSample, effUINT MultisampleQuality, effBOOL Lockable, EFF3DSurface **ppSurface)
{
	assert(ppSurface != NULL);

	EFFD3D9Surface * pSurface = new EFFD3D9Surface;

	effHRESULT hr;
	if ( FAILED(hr = m_pD3DDevice->CreateRenderTarget(Width,Height,(D3DFORMAT)Format,(D3DMULTISAMPLE_TYPE)MultiSample,MultisampleQuality,Lockable,&pSurface->m_pSurface,NULL)) )
	{
		SF_DELETE(pSurface);
		return hr;
	}

	pSurface->m_ImageInfo.SurfaceType = RenderTo_Surface;
	*ppSurface = pSurface;
	return hr;
}

effHRESULT EFFD3D9Device::CreateDepthStencilSurface(effUINT Width, effUINT Height, EFF3DFORMAT Format, EFF3DMULTISAMPLE_TYPE MultiSample, effUINT MultisampleQuality, effBOOL Discard, EFF3DSurface **ppSurface)
{
	assert(ppSurface != NULL);

	EFFD3D9Surface * pSurface = new EFFD3D9Surface;

	effHRESULT hr;
	if ( FAILED(hr = m_pD3DDevice->CreateDepthStencilSurface(Width,Height,(D3DFORMAT)Format,(D3DMULTISAMPLE_TYPE)MultiSample,MultisampleQuality,Discard,&pSurface->m_pSurface,NULL)) )
	{
		SF_DELETE(pSurface);
		return hr;
	}

	pSurface->m_ImageInfo.SurfaceType = DepthStencil_Surface;
	*ppSurface = pSurface;
	return hr;
}

effHRESULT EFFD3D9Device::CreateIndexBuffer(effUINT Length, effUINT Usage, EFF3DFORMAT Format, EFF3DPOOL Pool, EFF3DIndexBuffer **ppIndexBuffer)
{
	assert(ppIndexBuffer != NULL);
	
	EFFD3D9IndexBuffer * pIndexBuffer = new EFFD3D9IndexBuffer;
	effHRESULT hr;
	if ( FAILED(hr = m_pD3DDevice->CreateIndexBuffer(Length,Usage,(D3DFORMAT)Format,(D3DPOOL)Pool,&pIndexBuffer->m_pBuf,NULL)) )
	{
		SF_DELETE(pIndexBuffer);
		return hr;
	}

	*ppIndexBuffer = pIndexBuffer;
	return hr;
}

effHRESULT EFFD3D9Device::CreateVertexBuffer(effUINT Length, effUINT Usage, effUINT FVF, EFF3DPOOL Pool, EFF3DVertexBuffer **ppVertexBuffer)
{
	assert(ppVertexBuffer != NULL);

	EFFD3D9VertexBuffer * pVertexBuffer = new EFFD3D9VertexBuffer;
	effHRESULT hr;
	if ( FAILED(hr = m_pD3DDevice->CreateVertexBuffer(Length,Usage,FVF,(D3DPOOL)Pool,&pVertexBuffer->m_pBuf,NULL)) )
	{
		SF_DELETE(pVertexBuffer);
		return hr;
	}

	*ppVertexBuffer = pVertexBuffer;
	return hr;
}

effHRESULT EFFD3D9Device::CreateVertexDeclaration(const EFF3DVERTEXELEMENT *pVertexElements, EFF3DVertexDeclaration **ppDecl)
{
	assert(ppDecl != NULL);

	EFFD3D9VertexDeclaration * pDecl = new EFFD3D9VertexDeclaration;

	*ppDecl = pDecl;
	return S_OK;
}

effHRESULT EFFD3D9Device::DrawIndexedPrimitive(EFF3DPRIMITIVETYPE Type, effINT BaseVertexIndex, effUINT MinIndex, effUINT NumVertices, effUINT StartIndex, effUINT PrimitiveCount)
{
	return m_pD3DDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)Type,BaseVertexIndex,MinIndex,NumVertices,StartIndex,PrimitiveCount);
}

effHRESULT EFFD3D9Device::DrawIndexedPrimitiveUP(EFF3DPRIMITIVETYPE PrimitiveType, effUINT MinVertexIndex, effUINT NumVertices, effUINT PrimitiveCount, const void *pIndexData, EFF3DFORMAT IndexDataFormat, const void *pVertexStreamZeroData, effUINT VertexStreamZeroStride)
{
	return m_pD3DDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)PrimitiveType,MinVertexIndex,NumVertices,PrimitiveCount,pIndexData,(D3DFORMAT)IndexDataFormat,pVertexStreamZeroData,VertexStreamZeroStride);
}

effHRESULT EFFD3D9Device::DrawPrimitive(EFF3DPRIMITIVETYPE PrimitiveType, effUINT StartVertex, effUINT PrimitiveCount)
{
	return m_pD3DDevice->DrawPrimitive((D3DPRIMITIVETYPE)PrimitiveType,StartVertex,PrimitiveCount);
}

effHRESULT EFFD3D9Device::DrawPrimitiveUP(EFF3DPRIMITIVETYPE PrimitiveType, effUINT PrimitiveCount, const void *pVertexStreamZeroData, effUINT VertexStreamZeroStride)
{
	return m_pD3DDevice->DrawPrimitiveUP((D3DPRIMITIVETYPE)PrimitiveType,PrimitiveCount,pVertexStreamZeroData,VertexStreamZeroStride);
}

effHRESULT EFFD3D9Device::SetTransform(EFF3DTRANSFORMSTATETYPE State,const EFFMatrix4 * pMatrix)
{
	return m_pD3DDevice->SetTransform((D3DTRANSFORMSTATETYPE)State,(const D3DMATRIX *)pMatrix);
}

effHRESULT EFFD3D9Device::SetFVF(effUINT FVF)
{
	return m_pD3DDevice->SetFVF(FVF);
}

effHRESULT EFFD3D9Device::SetVertexDeclaration(EFF3DVertexDeclaration * pDecl)
{
	EFFD3D9VertexDeclaration * pEFFD3D9Decl = (EFFD3D9VertexDeclaration *)pDecl;
	return m_pD3DDevice->SetVertexDeclaration(pEFFD3D9Decl->GetD3D9VertexDeclaration(this));
}

effHRESULT EFFD3D9Device::SetStreamSource(effUINT StreamNumber,EFF3DVertexBuffer * pStreamData,effUINT OffsetInBytes,effUINT Stride)
{
	EFFD3D9VertexBuffer * pEFFD3D9VB = (EFFD3D9VertexBuffer *)pStreamData;
	return m_pD3DDevice->SetStreamSource(StreamNumber,pEFFD3D9VB->m_pBuf,OffsetInBytes,Stride);
}

effHRESULT EFFD3D9Device::SetIndices(EFF3DIndexBuffer * pIndexData)
{
	EFFD3D9IndexBuffer * pEFFD3D9IB = (EFFD3D9IndexBuffer *)pIndexData;
	return m_pD3DDevice->SetIndices(pEFFD3D9IB->m_pBuf);
}

effHRESULT EFFD3D9Device::SetRenderState(EFF3DRENDERSTATETYPE State,effUINT Value)
{
	return m_pD3DDevice->SetRenderState((D3DRENDERSTATETYPE)State,Value);
}

effHRESULT EFFD3D9Device::SetTextureStageState(effUINT Stage,EFF3DTEXTURESTAGESTATETYPE Type,effUINT Value)
{
	return m_pD3DDevice->SetTextureStageState(Stage,(D3DTEXTURESTAGESTATETYPE)Type,Value);
}

effHRESULT EFFD3D9Device::SetRenderTarget(effUINT RenderTargetIndex,EFF3DSurface * pRenderTarget)
{
	EFFD3D9Surface * pEFFD3D9Surface = (EFFD3D9Surface *)pRenderTarget;
	return m_pD3DDevice->SetRenderTarget(RenderTargetIndex,pEFFD3D9Surface->m_pSurface);
}

effHRESULT EFFD3D9Device::SetTexture(effUINT Sampler,EFF3DBaseTexture * pTexture)
{
	if ( pTexture != NULL )
	{
		if ( pTexture->GetImageInfo().ResourceType  == EFF3DRTYPE_TEXTURE )
		{
			EFFD3D9Texture * pEFFD3D9Texture = (EFFD3D9Texture *)pTexture;
			return m_pD3DDevice->SetTexture(Sampler,(IDirect3DTexture9 *)pEFFD3D9Texture->m_pTexture);
		}
		return S_OK;
	}
	else
	{
		return m_pD3DDevice->SetTexture(Sampler,NULL);
	}
}

effHRESULT EFFD3D9Device::SetDepthStencilSurface(EFF3DSurface *pNewZStencil)
{
	EFFD3D9Surface * pEFFD3D9Surface = (EFFD3D9Surface *)pNewZStencil;
	return m_pD3DDevice->SetDepthStencilSurface(pEFFD3D9Surface->m_pSurface);
}

effVOID EFFD3D9Device::Release()
{
}