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

effVOID ReceiveMsg()
{
	if ( client->ReceiveMsg(buffer, 256) )
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
	InitClient();

    MetaInitialize(UsingModule(EFFReflection));

    // type representing the SlideType enum
    Type sliderTypeEnumType = typeof(LayoutType);

    // enum representing the SliderType
    const Enum &sliderTypeEnum = sliderTypeEnumType.GetEnum();

    LayoutType zz = FlowLayout;

    std::string keyName = sliderTypeEnum.GetKey(zz);


	EFFApplication app;
	application = &app;
	app.Init(effTRUE, 1024, 768, effFALSE, effFALSE);
	app.SetBackGroundColor(0x00FF0000);
	//app.OnRenderGUI += EFFEventCall(&ReceiveMsg);

    app.OnUpdate += EFFEventCall(&ReceiveMsg);

    EFF3DSharedTexture * sharedRenderTarget = EFF3DGetDevice()->GetSharedRenderTarget();
    if (sharedRenderTarget != NULL)
    {
        SharedTextureInfo sharedTextureInfo;
        sharedTextureInfo.id = SendSharedTexture;

        sharedRenderTarget->GetSharedTextureInfo(&sharedTextureInfo);

        client->SendMsg(SendSharedTexture, &sharedTextureInfo, sizeof(SharedTextureInfo));
    }

	//LoadLibrary(_effT("EFFMyGUIEditor_d.dll"));

	app.Run();


 	return 0;
}