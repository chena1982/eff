#include "EFFRenderPCH.h"
#include "EFFRender.h"



EFFApplication * application = NULL;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);



    //MetaInitialize(UsingModule(EFFReflection));

    // type representing the SlideType enum
    //Type sliderTypeEnumType = typeof(LayoutType);

    // enum representing the SliderType
    //const Enum &sliderTypeEnum = sliderTypeEnumType.GetEnum();

    //LayoutType zz = FlowLayout;

    //std::string keyName = sliderTypeEnum.GetKey(zz);


	EFFApplication app;
	application = &app;
	app.Init(effTRUE, 1024, 768, effFALSE, effFALSE);
	app.SetBackGroundColor(0x00FF0000);
	//app.OnRenderGUI += EFFEventCall(&ReceiveMsg);

	app.OnUpdate += EFFEventCall(&app, &EFFApplication::ReceiveMsg);

	app.SendCmd(RequestGameWindowPosAndSize);

    EFF3DSharedTexture * sharedRenderTarget = EFF3DGetDevice()->GetSharedRenderTarget();
    if (sharedRenderTarget != NULL)
    {
        SharedTextureInfo sharedTextureInfo;
        sharedTextureInfo.id = SendSharedTexture;

        sharedRenderTarget->GetSharedTextureInfo(&sharedTextureInfo);

		app.SendMsg(SendSharedTexture, &sharedTextureInfo, sizeof(SharedTextureInfo));
    }

	//LoadLibrary(_effT("EFFMyGUIEditor_d.dll"));

	app.Run();


 	return 0;
}