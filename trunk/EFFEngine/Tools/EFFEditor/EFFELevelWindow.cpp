/******************************************************************************
	created:	2008-12-24   1:41
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\EFFELevelWindow.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFELevelWindow.h"



EFFELevelWindow::EFFELevelWindow()
{
	m_pDevice = NULL;
}


EFFELevelWindow::~EFFELevelWindow()
{
	SF_RELEASE(m_pDevice);
}


effBOOL EFFELevelWindow::Init(HWND hWnd)
{
#ifdef _DEBUG
	Create3DDevice(_effT("EFFD3D9UD.dll"),&m_pDevice,effTRUE,hWnd,0,0);
#else
	Create3DDevice(_effT("EFFD3D9U.dll"),&m_pDevice,effTRUE,hWnd,0,0);
#endif
	EFF3DMaterial * pMaterial = new EFF3DMaterial();

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('..\\Material Lib')");
	pMaterial->CreateFromFile("material");
	
	//m_pDevice->GetImageManager()->CreateFromFile(_effT(""),EFF3DRTYPE_TEXTURE);


	return effTRUE;
}