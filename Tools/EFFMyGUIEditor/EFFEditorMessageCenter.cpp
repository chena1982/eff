

#include "EFFMyGUIEditorPCH.h"

#include "EFFEditorMessageCenter.h"
#include "Main.h"
#include "MainPanelControl.h"
#include "MainWorkspaceControl.h"
#include "WorkspaceControl.h"
#include "ScenePanelControl.h"

EFFNetServer * server = NULL;

void InitServer()
{
	server = new EFFNetServer();
	server->Init();
	server->Bind(_effT("tcp://127.0.0.1:5555"));
}

EFFNetServer * GetServer()
{
	if (server == NULL)
	{
		InitServer();
	}

	return server;
}

effVOID ReceiveMsg(effFLOAT elapsedTime)
{
	effBYTE buffer[256];
	if ( server->ReceiveMsg(buffer, 256) )
	{
		effINT id = *((effINT *)buffer);
		if ( id == RequestGameWindowPosAndSize )
		{
			SendWindowPosAndSize();
		}
	}
}

effVOID ShutdownServer()
{
	if (server != NULL)
	{
		server->Shutdown();
	}

	SF_DELETE(server);
}


effVOID SendWindowPosAndSize()
{
	GameWindowPosAndSize gwpas;
	gwpas.id = SendGameWindowPosAndSize;

	HWND hWnd = (HWND)EFFEditor::getInstance().getWindowHandle();
	RECT rc;
	GetClientRect(hWnd, &rc);
	ClientToScreen(hWnd, reinterpret_cast<POINT *>(&rc.left));
	ClientToScreen(hWnd, reinterpret_cast<POINT *>(&rc.right));

	MyGUI::Widget * sceneWindow = EFFEditor::getInstance().GetMainPanelControl()->GetMainWorkspaceControl()->GetWorkspaceControl()->GetScenePanelControl()->GetSceneWindow();
	gwpas.x = sceneWindow->getAbsoluteLeft() + rc.left;
	gwpas.y = sceneWindow->getAbsoluteTop() + rc.top;
	gwpas.width = sceneWindow->getWidth();
	gwpas.height = sceneWindow->getHeight();
	server->SendMsg(gwpas.id, &gwpas, sizeof(gwpas));
}