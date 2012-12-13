/******************************************************************************
	created:	2012-10-22   23:43
	file path:	d:\EFF\EFFEngine\Src\MMOEncapsulation\MMOApplication.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "MMOApplication.h"


MMOApplication::MMOApplication()
{
	device = NULL;
}

MMOApplication::~MMOApplication()
{
	SF_RELEASE(device);
}

effBOOL MMOApplication::Init(effBOOL window, HWND hWnd, effINT width, effINT height)
{
	Create3DDevice(L"EFFD3D9UD.dll", &device, window, hWnd, width, height);

	if ( device == NULL )
	{
		return effFALSE;
	}

	return effTRUE;
}

effVOID	MMOApplication::Update()
{
}


effVOID MMOApplication::Render()
{
	static UINT color = 0xFF000000;
	color += 10;
	device->Clear(0, NULL, EFF3DCLEAR_TARGET | EFF3DCLEAR_TARGET, color, 1.0f, 0);
	if ( device->BeginScene() )
	{
		device->EndScene();
		device->Present(NULL, NULL);
	}
}