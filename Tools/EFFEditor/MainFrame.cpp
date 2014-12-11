#include "stdafx.h"

#include "MainFrame.h"

extern EFF3DObject * rootObject;
//extern EFF3DWebGUIWindow * mainWindow;
extern EFF3DHtmlWindow * mainWindow;

extern EFFNetServer * server;

effVOID RenderSceneWindow()
{

	effINT scroll = 10;
	imguiBeginScrollArea(_effT("Scene"), 0, 0, 300, 400, &scroll);
	imguiButton(_effT("我晕"), effTRUE);
	imguiTree(rootObject, effTRUE);
	imguiEndScrollArea();
}


effVOID RenderGUI(effFLOAT elapsedTime)
{
	/*EFF3DInputManager * inputManager = EFF3DGetDevice()->GetInputManager();
	imguiBeginFrame(inputManager->GetPosX(), inputManager->GetPosY(), inputManager->IsLeftButtonDown(), 0);

	RenderMenu();
	RenderSceneWindow();

	imguiEndFrame();
	imguiRenderDraw();*/


	mainWindow->Render();

}

effVOID ReceiveMsg(effFLOAT elapsedTime)
{
	if (server != NULL)
	{
		server->ReceiveMsg();
	}
}