
#include "EFFEditorPCH.h"
#include "EFFEditor.h"
#include "EFFEditorMainFrame.h"
#include "EFFEditorMessageCenter.h"
#include <Shellapi.h>

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



//EFF3DWebGUIWindow * mainWindow = NULL;
//EFF3DHtmlWindow * mainWindow = NULL;


EFF3DObject * rootObject;


effVOID Init()
{
	//mainWindow = new EFF3DWebGUIWindow();
	//mainWindow = new EFF3DHtmlWindow(1024, 768);
	//mainWindow->Initialise();
	//mainWindow->Resize(1024, 768);
	//mainWindow->LoadHTML(_effT("EditorUI\\ui.html?game"));
	//mainWindow->LoadHTML(_effT("EditorUI\\webix.html"));
	//mainWindow->LoadHTML(_effT("file:///D:/EFF/Build/Bin/EditorUI/webix.html"));
	//mainWindow->LoadHTML(_effT("http://news.163.com"));
	//mainWindow->LoadHTML(_effT("html\\mac-osx-lion.html"));

	//mainWindow->SetTransparent(effTRUE);

	/*effString buffer;
	rootObject->GetPropertyJason(_effT("name"), buffer);
	mainWindow->SendMessageToJS(buffer);*/


}


EFFApplication * application = NULL;
EFFEditor * editor = NULL;


EFFEditor::EFFEditor()
{
}

EFFEditor::~EFFEditor()
{
}

void EFFEditor::CreateScene()
{
}


void EFFEditor::DestroyScene()
{
}

void EFFEditor::SetupResources()
{

}







void EFFEditor::ReceiveMsg()
{
	//effBYTE buffer[256];
}

void StartRender()
{
	ShellExecute(NULL, NULL, _effT("EFFRender_d.exe"), NULL, NULL, SW_HIDE);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);




	EFFApplication app;
	application = &app;
	app.Init(effTRUE, 1024, 768, effTRUE, effTRUE);
	app.SetBackGroundColor(0x00000000);

	//StartRender();

	InitEditor();

	app.OnUpdate += EFFEventCall(&app, &EFFApplication::ReceiveMsg);
	app.OnRenderGUI += EFFEventCall(&RenderGUI);

	app.Run();

	app.SendCmd(QuitApp);

 	return 0;
}

VOID InitEditor()
{
	editor = new EFFEditor();
	//application->OnRenderGUI += EFFEventCall((base::BaseManager *)editor, &base::BaseManager::drawOneFrame);
}



