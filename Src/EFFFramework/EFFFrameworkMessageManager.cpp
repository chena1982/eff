/******************************************************************************
	created:	2019-01-05   00:13
	file path:	C:\EFF\EFFEngine\Src\EFFFramework\EFFFrameworkMessageManager.cpp
	author:		ChenA

	purpose:
******************************************************************************/

#include "EFFFrameworkPCH.h"
#include "EFFApplication.h"
#include "EFFFrameworkMessageManager.h"


EFFFRAMEWORK_BEGIN

extern EFFApplication * application;


effVOID SendWindowPosAndSize()
{
	EFFNetServer * server = application->GetServer();
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

effVOID ReceiveWindowPosAndSize(effVOID * buffer)
{
	GameWindowPosAndSize * gwpas = (GameWindowPosAndSize *)buffer;
	application->MoveWindow(gwpas->x, gwpas->y, gwpas->width, gwpas->height);
}

effVOID ReceiveMsg(EFFNetServer * server, EFFNetClient * client)
{
	effBYTE buffer[1024];

	if (server != NULL)
	{
		if (server->ReceiveMsg(buffer, 1024))
		{
			effINT id = *((effINT *)buffer);
			if (id == RequestGameWindowPosAndSize)
			{
				application->connectedToClient = effTRUE;
				SendWindowPosAndSize();
			}
			else if (id == SendSharedTexture)
			{
				SharedTextureInfo * sharedTextureInfo = (SharedTextureInfo *)buffer;

				EFF3DGetDevice()->InitSharedTexture(sharedTextureInfo);

				SetActiveWindow(application->GetHWND());
			}
			else
			{
				int z = 0;
			}
		}
	}

	if (client != NULL)
	{
		if (client->ReceiveMsg(buffer, 1024))
		{
			effINT id = *((effINT *)buffer);
			if (id == SendGameWindowPosAndSize)
			{
				ReceiveWindowPosAndSize(buffer);
				application->connectedToHost = effTRUE;
			}
			else if (id == RequestApp)
			{
				ApplicationInfo info;
				info.id = SendApp;
				info.app = application;

				//server->SendMsg(info.id, &info, sizeof(ApplicationInfo));
			}
			else if (id == QuitApp)
			{
				application->Quit();
			}
			else
			{
				int z = 0;
			}
		}
	}

}

EFFFRAMEWORK_END