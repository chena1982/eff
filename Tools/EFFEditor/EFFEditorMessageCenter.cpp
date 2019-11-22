

#include "EFFEditorPCH.h"

#include "EFFEditorMessageCenter.h"
#include "EFFEditor.h"


EFFNetClient * client = NULL;
EFFNetServer * server = NULL;

extern EFFApplication * application;

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

void InitClient()
{
	client = EFFNEW EFFNetClient();
	client->Init();
	client->Connect(_effT("tcp://127.0.0.1:5555"));
}

EFFNetClient * GetClient()
{
	if (client == NULL)
	{
		InitClient();
	}

	return client;
}

effVOID ReceiveMsg(effFLOAT elapsedTime)
{
	effBYTE buffer[256];
	if ( client->ReceiveMsg(buffer, 256) )
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
	if (server == NULL)
	{
		return;
	}

	GameWindowPosAndSize gwpas;
	gwpas.id = SendGameWindowPosAndSize;

	HWND hWnd = application->GetHWND();
	RECT rc;
	GetClientRect(hWnd, &rc);
	ClientToScreen(hWnd, reinterpret_cast<POINT *>(&rc.left));
	ClientToScreen(hWnd, reinterpret_cast<POINT *>(&rc.right));


	gwpas.x = rc.left;
	gwpas.y = rc.top;
	gwpas.width = rc.right - rc.left;
	gwpas.height = rc.bottom - rc.top;
	server->SendMsg(gwpas.id, &gwpas, sizeof(gwpas));

	//MainPanelControl * mpc = EFFEditor::getInstance().GetMainPanelControl();
	/*if ( mpc != NULL )
	{
		MyGUI::Widget * sceneWindow = mpc->GetMainWorkspaceControl()->GetWorkspaceControl()->GetScenePanelControl()->GetSceneWindow();
		gwpas.x = sceneWindow->getAbsoluteLeft() + rc.left;
		gwpas.y = sceneWindow->getAbsoluteTop() + rc.top;
		gwpas.width = sceneWindow->getWidth();
		gwpas.height = sceneWindow->getHeight();
		server->SendMsg(gwpas.id, &gwpas, sizeof(gwpas));
	}*/
}