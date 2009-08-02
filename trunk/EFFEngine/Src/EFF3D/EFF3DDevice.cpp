/******************************************************************************
	created:	2008-12-1   23:24
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DDevice.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DDevice.h"

#include "EFF3DResourceManager.h"
#include "EFF3DTexture.h"


#define new EFFNEW

EFF3D_BEGIN

effBOOL EFF3D_API  Create3DDevice(effLPCTSTR pszDllName,EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA/* = effFALSE*/,effBOOL bMultiThread/* = effFALSE*/,effDWORD dwDSFormat/* = EFF3DFMT_D16*/)
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

	m_pImageManager = new EFF3DImageManager();

	m_pImageManager->CreateFromFile(_effT(""),EFF3DRTYPE_TEXTURE);
}

EFF3DDevice::~EFF3DDevice()
{
	Py_Finalize();//调用Py_Finalize，这个根Py_Initialize相对应的。

	SF_DELETE(m_pImageManager);
}


EFF3D_END