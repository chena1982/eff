/******************************************************************************
	created:	2008-12-1   23:24
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DDevice.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DDevice.h"

#define new EFFNEW


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
}

EFF3DDevice::~EFF3DDevice()
{
}