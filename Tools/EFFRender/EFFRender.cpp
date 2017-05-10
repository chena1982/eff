#include "EFFRenderPCH.h"
#include "EFFRender.h"



EFFApplication * application = NULL;
EFFNetClient * client = NULL;
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

void InitClient()
{
	client = EFFNEW EFFNetClient();
	client->Init();
	client->Connect(_effT("tcp://127.0.0.1:5555"));
}



effBYTE buffer[256];

effVOID ReceiveMsg(effFLOAT elapsedTime)
{
	if ( server->ReceiveMsg(buffer, 256) )
	{
		effINT id = *((effINT *)buffer);
		if ( id == SendGameWindowPosAndSize )
		{
			ReceiveWindowPosAndSize();
		}
		else if (id == RequestApp)
		{
			ApplicationInfo info;
			info.id = SendApp;
			info.app = application;

			server->SendMsg(info.id, &info, sizeof(ApplicationInfo));
		}
		else
		{
			int z = 0;
		}
	}
}

effVOID ReceiveWindowPosAndSize()
{
	GameWindowPosAndSize * gwpas = (GameWindowPosAndSize *)buffer;
	application->MoveWindow(gwpas->x, gwpas->y, gwpas->width, gwpas->height);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//InitServer();
	//InitClient();
	//client->SendCmd(RequestGameWindowPosAndSize);

	EFFApplication app;
	application = &app;
	app.Init(effTRUE, 1024, 768, effFALSE, effFALSE);
	app.SetBackGroundColor(0x00FF0000);
	//app.OnRenderGUI += EFFEventCall(&ReceiveMsg);

	//LoadLibrary(_effT("EFFMyGUIEditor_d.dll"));

	app.Run();


 	return 0;
}