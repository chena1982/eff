#include "stdafx.h"
#include "Game.h"



EFFApplication * application = NULL;
EFFNetClient * client = NULL;

void InitClient()
{
	client = EFFNEW EFFNetClient();
	client->Init();
	client->Connect(_effT("tcp://127.0.0.1:5555"));
}

effBYTE buffer[256];

effVOID ReceiveMsg(effFLOAT elapsedTime)
{
	if ( client->ReceiveMsg(buffer, 256) )
	{
		effINT id = *((effINT *)buffer);
		if ( id == SendGameWindowPosAndSize )
		{
			ReceiveWindowPosAndSize();
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


	InitClient();
	client->SendCmd(RequestGameWindowPosAndSize);

	EFFApplication app;
	application = &app;
	app.Init(effTRUE, 1024, 768, effTRUE, effFALSE);
	app.SetBackGroundColor(0x80FF0000);
	app.OnRenderGUI += EFFEventCall(&ReceiveMsg);

	app.Run();


 	return 0;
}