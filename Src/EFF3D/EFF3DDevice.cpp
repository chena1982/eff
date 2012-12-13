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

//#define new EFFNEW

EFF3D_BEGIN

EFF3DDevice * GetDevice()
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


EFF3DDevice::EFF3DDevice()
{
	//Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化

	//ilInit();



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
	//SF_DELETE(m_pWebCore);
}

effVOID EFF3DDevice::Init()
{
	imageManager = EFFNEW EFF3DImageManager();
	EFFRegisterObjectManager(EFF3DImageManager::GetThisClass(), imageManager);
	
	sceneManager = EFFNEW EFF3DSceneManager();
}

struct QuadVertex
{
	effFLOAT			x,y,z,rhw;
	effFLOAT			u,v;
	const static effUINT fvf = EFF3DFVF_XYZRHW | EFF3DFVF_TEX1;
};




effBOOL EFF3DDevice::DrawQuad(EFFRect * rect)
{

	static QuadVertex vertices[6];

	EFFRect quadRect;
	if ( rect == NULL )
	{
		quadRect.nLeft = 0;
		quadRect.nRight = width;
		quadRect.nTop = 0;
		quadRect.nBottom = height;
	}
	else
	{
		quadRect = *rect;
	}

	vertices[0].x = (effFLOAT)quadRect.nLeft - 0.5f;
	vertices[0].y = (effFLOAT)quadRect.nTop - 0.5f;
	vertices[0].z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	vertices[1].x = (effFLOAT)quadRect.nRight - 0.5f;
	vertices[1].y = (effFLOAT)quadRect.nTop - 0.5f;
	vertices[1].z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	vertices[2].x = (effFLOAT)quadRect.nRight - 0.5f;
	vertices[2].y = (effFLOAT)quadRect.nBottom - 0.5f;
	vertices[2].z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	vertices[3] = vertices[0];
	vertices[4] = vertices[2];


	vertices[5].x = (effFLOAT)quadRect.nLeft - 0.5f;
	vertices[5].y = (effFLOAT)quadRect.nBottom - 0.5f;
	vertices[5].z = 0.0f;
	vertices[5].rhw = 1.0f;
	vertices[5].u = 0.0f;
	vertices[5].v = 1.0f;

	SetFVF(QuadVertex::fvf);
	return DrawPrimitiveUP(EFF3DPT_TRIANGLELIST, 2, vertices, sizeof(QuadVertex));
}

effBOOL EFF3DDevice::DrawQuad(EFF3DTexture * texture, EFF3DMaterial * material /* = NULL */, EFFRect * rect /* = NULL */)
{
	SetTexture(0, texture);
	effHRESULT hr;


	if ( material != NULL )
	{
		EFF3DShader * shader = material->GetShader();

		if ( shader != NULL )
		{
			//shader->UpdateAutoParametersPerShader(this, autoParamDataSource);
			hr = DrawQuad(rect);
		}
	}
	else
	{
		SetTextureStageState(0, EFF3DTSS_COLOROP, EFF3DTOP_SELECTARG1);
		SetTextureStageState(0, EFF3DTSS_COLORARG1, EFF3DTA_TEXTURE);
		hr = DrawQuad(rect);
	}

	return SUCCEEDED(hr);
}

effVOID EFF3DDevice::SetBackBufferSize(effINT width, effINT height)
{
	this->width = width;
	this->height = height;
}

EFF3D_END