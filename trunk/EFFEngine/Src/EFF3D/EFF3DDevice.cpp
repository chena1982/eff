/******************************************************************************
	created:	2008-12-1   23:24
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DDevice.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"

#include "EFF3DResourceManager.h"
#include "EFF3DAsyncLoader.h"
#include "EFF3DTexture.h"
#include "EFF3DAutoParamDataSource.h"
#include "EFF3DMaterial.h"
#include "EFF3DEffect.h"
#include "EFF3DDevice.h"

//#define new EFFNEW

EFF3D_BEGIN

effBOOL EFF3D_API  Create3DDevice(effLPCTSTR pszDllName,EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA/* = effFALSE*/,effBOOL bMultiThread/* = effFALSE*/,effUINT dwDSFormat/* = EFF3DFMT_D16*/)
{
	HMODULE hDLL = LoadLibrary(pszDllName);
	if ( hDLL != NULL )
	{
		effCREATE3DDEVICE realCreate3DDevice = (effCREATE3DDEVICE)GetProcAddress((HMODULE)hDLL,"effCreate3DDevice");
		if (realCreate3DDevice==0)
		{
			return effFALSE;
		}
		return realCreate3DDevice(lpp3DDevice,bWindow,hWnd,nWidth,nHeight,bFSA,bMultiThread,dwDSFormat);
	}
	return effFALSE;
}


EFF3DDevice::EFF3DDevice()
{
	Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化

	ilInit();

	m_pImageManager = new EFF3DImageManager();
	
	m_pAsyncLoader = new EFF3DAsyncLoader(this,1);

	//m_pWebCore = new Awesomium::WebCore();

	m_nWidth = 0;
	m_nHeight = 0;
	//m_pImageManager->CreateFromFile(_effT(""),EFF3DRTYPE_TEXTURE);

	m_pAutoParamDataSource = new EFF3DAutoParamDataSource;
}

EFF3DDevice::~EFF3DDevice()
{
	Py_Finalize();//调用Py_Finalize，这个根Py_Initialize相对应的。

	SF_DELETE(m_pImageManager);

	//SF_DELETE(m_pWebCore);

	SF_DELETE(m_pAutoParamDataSource);
}

struct QuadVertex
{
	effFLOAT			x,y,z,rhw;
	effFLOAT			u,v;
	const static effUINT	fvf = EFF3DFVF_XYZRHW | EFF3DFVF_TEX1;
};



effHRESULT EFF3DDevice::DrawQuad(EFFRect * pRect)
{

	static QuadVertex vertices[6];

	EFFRect rect;
	if ( pRect == NULL )
	{
		rect.nLeft = 0;
		rect.nRight = m_nWidth;
		rect.nTop = 0;
		rect.nBottom = m_nHeight;
	}
	else
	{
		rect = *pRect;
	}

	vertices[0].x = (effFLOAT)rect.nLeft - 0.5f;
	vertices[0].y = (effFLOAT)rect.nTop - 0.5f;
	vertices[0].z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	vertices[1].x = (effFLOAT)rect.nRight - 0.5f;
	vertices[1].y = (effFLOAT)rect.nTop - 0.5f;
	vertices[1].z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	vertices[2].x = (effFLOAT)rect.nRight - 0.5f;
	vertices[2].y = (effFLOAT)rect.nBottom - 0.5f;
	vertices[2].z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	vertices[3] = vertices[0];
	vertices[4] = vertices[2];


	vertices[5].x = (effFLOAT)rect.nLeft - 0.5f;
	vertices[5].y = (effFLOAT)rect.nBottom - 0.5f;
	vertices[5].z = 0.0f;
	vertices[5].rhw = 1.0f;
	vertices[5].u = 0.0f;
	vertices[5].v = 1.0f;

	SetFVF(QuadVertex::fvf);
	return DrawPrimitiveUP(EFF3DPT_TRIANGLELIST,2,vertices,sizeof(QuadVertex));
}

effHRESULT EFF3DDevice::DrawQuad(EFF3DTexture * pTexture,EFF3DMaterial * pMaterial /* = NULL */,EFFRect * pRect /* = NULL */)
{
	SetTexture(0,pTexture);
	effHRESULT hr;


	if ( pMaterial != NULL )
	{
		EFF3DEffect * pEffect = pMaterial->GetEffect();

		if ( pEffect != NULL )
		{
			pEffect->UpdateAutoParametersPerEffect(this,m_pAutoParamDataSource);

			effUINT uiPass;
			pEffect->Begin(&uiPass,0);
			for ( effUINT i = 0; i < uiPass; i++ )
			{
				pEffect->BeginPass(i);
				hr = DrawQuad(pRect);
				pEffect->EndPass();
			}
			pEffect->End();
		}
	}
	else
	{
		SetTextureStageState(0,EFF3DTSS_COLOROP,EFF3DTOP_SELECTARG1);
		SetTextureStageState(0,EFF3DTSS_COLORARG1,EFF3DTA_TEXTURE);
		hr = DrawQuad(pRect);
	}

	return hr;
}

effVOID EFF3DDevice::SetBackBufferSize(effINT nWidth,effINT nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

EFF3D_END