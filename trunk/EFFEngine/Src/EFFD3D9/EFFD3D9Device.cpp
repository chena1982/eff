/******************************************************************************
	created:	2008-12-2   0:21
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Device.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFD3D9Device.h"

#define new EFFNEW


effVOID InitWindow(effINT nWidth,effINT nHeight,D3DPRESENT_PARAMETERS *d3dpp,effBOOL bFSA,effDWORD dwDSFormat)  
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


effBOOL effCreate3DDevice(EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA /* = effFALSE */,effBOOL bMultiThread /* = effFALSE */,effDWORD dwDSFormat/* = EFF3DFMT_D16*/)
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
	effDWORD dwBehaviorFlags;
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