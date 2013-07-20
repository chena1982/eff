/******************************************************************************
	created:	2008-12-1   23:24
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DDevice.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"

#include "EFF3DResourceManager.h"
#include "EFF3DSceneManager.h"
#include "EFF3DTexture.h"
#include "EFF3DMaterial.h"
#include "EFF3DShader.h"
#include "EFF3DDevice.h"
#include "EFF3DObject.h"
#include "EFF3DFont.h"
#include "EFF3DInputManager.h"

//#define new EFFNEW

EFF3D_BEGIN

static EFF3DDevice * device = NULL;

EFF3DDevice * EFF3DGetDevice()
{
	return device;
}

typedef effBOOL (*effCREATE3DDEVICE)(EFF3DDevice ** eff3DDevice, effBOOL window, HWND hWnd, effINT width, effINT height);

effBOOL EFF3D_API Create3DDevice(const effString & dllName, EFF3DDevice ** eff3DDevice, effBOOL window, HWND hWnd, effINT width, effINT height)
{
	HMODULE hDLL = LoadLibrary(dllName.c_str());
	if ( hDLL != NULL )
	{
		effCREATE3DDEVICE realCreate3DDevice = (effCREATE3DDEVICE)GetProcAddress((HMODULE)hDLL, "effCreate3DDevice");
		if ( realCreate3DDevice == NULL )
		{
			return effFALSE;
		}

		effBOOL succeed = realCreate3DDevice(eff3DDevice, window, hWnd, width, height);
		if ( succeed )
		{
			device = *eff3DDevice;
			device->Init();
		}
	}
	return effFALSE;
}


struct EFF3DFormatInfo
{
	effUINT bpp;
	effUINT rmask;
	effUINT gmask;
	effUINT bmask;
	effUINT amask;
	EFF3DFORMAT format;
};


EFF3DFormatInfo formatInfo[] =
{
	{ 1, 0xe0, 0x1c, 0x03, 0, EFF3DFMT_R3G3B2 },
	{ 1, 0xFF, 0, 0, 0, EFF3DFMT_A8 },
	{ 2, 0xf800, 0x07e0, 0x001f, 0x0000, EFF3DFMT_R5G6B5 },
	{ 2, 0x7c00, 0x03e0, 0x001f, 0x8000, EFF3DFMT_A1R5G5B5 },
	{ 2, 0x7c00, 0x03e0, 0x001f, 0x0000, EFF3DFMT_X1R5G5B5 },
	{ 2, 0x0f00, 0x00f0, 0x000f, 0xf000, EFF3DFMT_A4R4G4B4 },
	{ 2, 0x0f00, 0x00f0, 0x000f, 0x0000, EFF3DFMT_X4R4G4B4 },
	{ 2, 0x00e0, 0x001c, 0x0003, 0xff00, EFF3DFMT_A8R3G3B2 },
	{ 3, 0xff0000, 0x00ff00, 0x0000ff, 0x000000, EFF3DFMT_R8G8B8 },
	{ 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000, EFF3DFMT_A8R8G8B8 },
	{ 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000, EFF3DFMT_X8R8G8B8 },
	{ 4, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000, EFF3DFMT_A2B10G10R10 },
	{ 4, 0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000, EFF3DFMT_A2R10G10B10 },
	{ 4, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000, EFF3DFMT_G16R16 },
	{ 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000, EFF3DFMT_A8B8G8R8 },
	{ 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000, EFF3DFMT_X8B8G8R8 },
};

effINT EFF3DGetPixelSizeFromFormat(EFF3DFORMAT format)	
{
	effINT elementCount = sizeof(formatInfo) / sizeof(EFF3DFormatInfo);
	for ( effINT i = 0; i < elementCount; i++ )
	{
		if ( formatInfo[i].format == format )
		{
			return formatInfo[i].bpp;
		}
	}

	return 0;
}

EFF3DDevice::EFF3DDevice()
{
	//Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化

	ilInit();



	//m_pWebCore = new Awesomium::WebCore();

	width = 0;
	height = 0;
	//m_pImageManager->CreateFromFile(_effT(""),EFF3DRTYPE_TEXTURE);


}

EFF3DDevice::~EFF3DDevice()
{
	//Py_Finalize();//调用Py_Finalize，这个根Py_Initialize相对应的。

	SF_DELETE(imageManager);
	SF_DELETE(sceneManager);

	SF_DELETE(fontManager);
	SF_DELETE(inputManager);
	//SF_DELETE(m_pWebCore);
}

effVOID EFF3DDevice::Init()
{
	imageManager = EFFNEW EFF3DImageManager();
	EFFRegisterObjectManager(EFF3DImage::GetThisClass(), imageManager);
	
	sceneManager = EFFNEW EFF3DSceneManager();

	InitProperty();

	SetRenderState(EFF3DRS_LIGHTING, effFALSE);


	fontManager = EFFNEW EFF3DFontManager();
	fontManager->CreateFromFile(_effT("Font\\simsun.ttc"), 16);
	//fontManager->CreateFromFile(_effT("Font\\msyh.ttf"), 16);

	inputManager = EFFNEW EFF3DInputManager();
}

effVOID EFF3DDevice::InitProperty()
{
	EFF3DObjectInitProperty();
}




effBOOL EFF3DDevice::DrawQuad(EFFRect * rect)
{

	static QuadVertex vertices[6];

	EFFRect quadRect;
	if ( rect == NULL )
	{
		quadRect.left = 0.0f;
		quadRect.right = (effFLOAT)width;
		quadRect.top = 0.0f;
		quadRect.bottom = (effFLOAT)height;
	}
	else
	{
		quadRect = *rect;
	}

	vertices[0].x = (effFLOAT)quadRect.left - 0.5f;
	vertices[0].y = (effFLOAT)quadRect.top - 0.5f;
	vertices[0].z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	vertices[1].x = (effFLOAT)quadRect.right - 0.5f;
	vertices[1].y = (effFLOAT)quadRect.top - 0.5f;
	vertices[1].z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	vertices[2].x = (effFLOAT)quadRect.right - 0.5f;
	vertices[2].y = (effFLOAT)quadRect.bottom - 0.5f;
	vertices[2].z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	vertices[3] = vertices[0];
	vertices[4] = vertices[2];


	vertices[5].x = (effFLOAT)quadRect.left - 0.5f;
	vertices[5].y = (effFLOAT)quadRect.bottom - 0.5f;
	vertices[5].z = 0.0f;
	vertices[5].rhw = 1.0f;
	vertices[5].u = 0.0f;
	vertices[5].v = 1.0f;

	SetFVF(QuadVertex::fvf);
	return DrawPrimitiveUP(EFF3DPT_TRIANGLELIST, 2, vertices, sizeof(QuadVertex));
}

effBOOL EFF3DDevice::DrawQuad(EFFRect * rect, effDWORD color)
{
	SetRenderState(EFF3DRS_ALPHABLENDENABLE, effFALSE);
	SetRenderState(EFF3DRS_TEXTUREFACTOR, color);

	SetTextureStageState(0, EFF3DTSS_COLOROP, EFF3DTOP_SELECTARG1);
	SetTextureStageState(0, EFF3DTSS_COLORARG1, EFF3DTA_TFACTOR);

	effHRESULT hr = DrawQuad(rect);
	return SUCCEEDED(hr);
}

effBOOL EFF3DDevice::DrawQuad(EFFRect * rect, EFF3DTexture * texture, effBOOL blend)
{
	if ( blend )
	{
		SetRenderState(EFF3DRS_ALPHABLENDENABLE, effTRUE);
		SetRenderState(EFF3DRS_SRCBLEND, EFF3DBLEND_SRCALPHA);
		SetRenderState(EFF3DRS_DESTBLEND, EFF3DBLEND_INVSRCALPHA);

		SetTextureStageState(0, EFF3DTSS_COLOROP, EFF3DTOP_SELECTARG1);
		SetTextureStageState(0, EFF3DTSS_COLORARG1, EFF3DTA_TEXTURE);

		SetTextureStageState(0, EFF3DTSS_ALPHAOP, EFF3DTOP_SELECTARG1);
		SetTextureStageState(0, EFF3DTSS_ALPHAARG1, EFF3DTA_TEXTURE);
	}
	else
	{
		SetRenderState(EFF3DRS_ALPHABLENDENABLE, effFALSE);
	}

	SetTexture(0, texture);
	effHRESULT hr = DrawQuad(rect);
	return SUCCEEDED(hr);
}

effBOOL EFF3DDevice::DrawQuad(EFFRect * rect, EFF3DMaterial * material, EFF3DTexture * texture)
{
	if ( material == NULL )
	{
		return effFALSE;
	}

	EFF3DShader * shader = material->GetShader();

	if ( shader == NULL )
	{
		return effFALSE;
	}

	effHRESULT hr;
	SetTexture(0, texture);
	//shader->UpdateAutoParametersPerShader(this, autoParamDataSource);
	hr = DrawQuad(rect);
	return SUCCEEDED(hr);
}

effVOID EFF3DDevice::SetBackBufferSize(effINT width, effINT height)
{
	this->width = width;
	this->height = height;
}

EFF3D_END