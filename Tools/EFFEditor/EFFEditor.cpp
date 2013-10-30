// EFFIMGUIDEditor.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "EFFEditor.h"
#include "MainFrame.h"


EFF3DObject * CreateObject()
{
	EFFObjectManager * objectManager = EFFGetObjectManager(EFF3DObject::GetThisClass());
	EFF3DObject * gameObject = (EFF3DObject *)objectManager->CreateObject(EFF3DObject::GetThisClass());

	gameObject->SetName(_effT("Root"));

	EFF3DObject * child = (EFF3DObject *)objectManager->CreateObject(EFF3DObject::GetThisClass());
	child->SetName(_effT("child"));
	gameObject->AddChild(child);

	EFF3DObject * child2 = (EFF3DObject *)objectManager->CreateObject(EFF3DObject::GetThisClass());
	child2->SetName(_effT("child2"));
	child->AddChild(child2);

	return gameObject;
}

EFF3DWebGUIWindow * mainWindow = NULL;



EFF3DObject * rootObject;


effVOID Init()
{
	mainWindow = new EFF3DWebGUIWindow();
	mainWindow->Initialise();
	mainWindow->Resize(1024, 768);
	mainWindow->LoadHTML(_effT("EditorUI\\ui.html?game"));
	//mainWindow->LoadHTML(_effT("html\\mac-osx-lion.html"));

	mainWindow->SetTransparent(effTRUE);

	effString buffer;
	rootObject->GetPropertyJason(_effT("name"), buffer);
	mainWindow->SendMessageToJS(buffer);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	rootObject = CreateObject();

	MMOApplication app;
	app.Init(effTRUE, 1024, 768, effFALSE, effFALSE);
	//app.SetBackGroundColor(0);

	Init();

	app.OnRenderGUI += EFFEventCall(&RenderGUI);

	app.Run();

 	return 0;
}



