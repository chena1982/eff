/******************************************************************************
	created:	2014-2-6   16:52
	file path:	d:\EFF\Src\EFF3D\EFF3DHtmlWindow.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "../EFF3DPCH.h"
#include "EFF3DHtmlWindow.h"
#include "EFF3DDevice.h"
#include "EFF3DTexture.h"


/*#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/BitmapSurface.h>

EFF3D_BEGIN

EFF3DHtmlWindow::EFF3DHtmlWindow(effINT width, effINT height) : width(width), height(height)
{
	webView = NULL;
	texture = NULL;
}

EFF3DHtmlWindow::~EFF3DHtmlWindow()
{
    if ( webView != NULL )
    {
        webView->Destroy();
        webView = NULL;
    }
}

effVOID EFF3DHtmlWindow::Initialise()
{

	EFF3DDevice * device = EFF3DGetDevice();

	if ( device == NULL )
	{
		return;
	}

	Awesomium::WebCore * webCore = device->GetWebCore();

    if ( webView == NULL)
    {
        webView = webCore->CreateWebView(width, height);

		EFFInputManager * inputManager = EFF3DGetDevice()->GetInputManager();

		inputManager->OnKeyPressed += EFFEventCall(this, &EFF3DWebGUIWindow::KeyPressed);
		inputManager->OnKeyReleased += EFFEventCall(this, &EFF3DWebGUIWindow::KeyReleased);

		inputManager->OnMouseMove += EFFEventCall(this, &EFF3DWebGUIWindow::MouseMoved);
		inputManager->OnMouseButtonPressed += EFFEventCall(this, &EFF3DWebGUIWindow::MousePressed);
		inputManager->OnMouseButtonReleased += EFFEventCall(this, &EFF3DWebGUIWindow::MouseReleased);

		clientHandler.onTitleChanged = OnTitleChanged;
		clientHandler.onURLChanged = OnURLChanged;
		webView->setClientHandler(&clientHandler);
    }


}

effVOID EFF3DHtmlWindow::Resize(effINT width, effINT height)
{
	if ( webView == NULL )
	{
		return;
	}

	//webView->resize(width, height);
	//webView->setDirty(true);

	SF_RELEASE(texture);

	this->width = width;
	this->height = height;

	EFF3DDevice * device = EFF3DGetDevice();
	if ( FAILED(device->CreateTexture(width, height, 1, EFF3DUSAGE_DYNAMIC, EFF3DFMT_A8R8G8B8, EFF3DPOOL_DEFAULT, &texture)) )
	{
		return;
	}


}

effBOOL EFF3DHtmlWindow::LoadHTML(const effString & fileName)
{
	if ( webView == NULL )
	{
		return effFALSE;
	}

	Awesomium::WebURL url(Awesomium::WSLit(EFFSTRING2ANSI(fileName)));
	webView->LoadURL(url);



	return effTRUE;
}


effVOID EFF3DHtmlWindow::Render()
{

	if ( webView == NULL || texture == NULL )
	{
		return;
	}


	Awesomium::BitmapSurface * surface = (Awesomium::BitmapSurface *)webView->surface();

	

	if ( surface != NULL)
	{

		static bool runFirst = true;

		//if ( runFirst )
		{
			surface->SaveToPNG(Awesomium::WSLit("11.png"), true);
			runFirst = false;
		}

		EFF3DLOCKED_RECT rc;
		if ( FAILED(texture->LockRect(0, &rc, NULL, EFF3DLOCK_DISCARD)) )
		{
			return;
		}

		surface->CopyTo((unsigned char *)rc.pBits, rc.Pitch, 4, false, false);

		texture->Unlock();
	}

	EFF3DDevice * device = EFF3DGetDevice();

	device->SetRenderState(EFF3DRS_ALPHABLENDENABLE, effTRUE);
	device->SetRenderState(EFF3DRS_SRCBLEND, EFF3DBLEND_SRCALPHA);
	device->SetRenderState(EFF3DRS_DESTBLEND, EFF3DBLEND_INVSRCALPHA);
	device->DrawQuad(NULL, texture, effTRUE);
	device->SetRenderState(EFF3DRS_ALPHABLENDENABLE, effFALSE);
}

effVOID EFF3DHtmlWindow::SetTransparent(effBOOL transparent)
{
	//去除编译器警告，直接强转有警告
	bool webTransparent = transparent ? true : false;
	webView->SetTransparent(webTransparent);
}


EFF3D_END*/