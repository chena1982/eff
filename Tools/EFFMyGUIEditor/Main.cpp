
#include "EFFMyGUIEditorPCH.h"

#include "Main.h"
#include "MainPanelControl.h"
#include "EditorWidgets.h"
#include "EFFEditorMessageCenter.h"

class EFFEditor;

template <> EFFEditor * MyGUI::Singleton<EFFEditor>::msInstance = NULL;
template <> const char * MyGUI::Singleton<EFFEditor>::mClassTypeName = "EFFEditor";


EFFApplication * app = NULL;
EFFEditor * editor = NULL;

EFFEditor::EFFEditor()
{
	mMainPanelControl = NULL;
}

EFFEditor::~EFFEditor()
{
}

void EFFEditor::createScene()  
{  
    setWindowCaption(L"EFFEditor");  
    //MyGUI::VectorWidgetPtr windows = MyGUI::LayoutManager::getInstance().loadLayout("MainPanel.layout");


	MyGUI::ResourceManager::getInstance().load("EditorLayers.xml");
	//MyGUI::ResourceManager::getInstance().load("MyGUI_CommonSkins.xml");
	MyGUI::ResourceManager::getInstance().load("MyGUI_DarkSkin.xml");
	//MyGUI::ResourceManager::getInstance().load("SelectionSkins.xml");
	//MyGUI::ResourceManager::getInstance().load("EditorReplaceSkins.xml");
	MyGUI::ResourceManager::getInstance().load("MyGUI_DarkTemplate.xml");
	//MyGUI::ResourceManager::getInstance().load("UnitySkin.xml");
	//MyGUI::ResourceManager::getInstance().load("UnityTemplate.xml");
	MyGUI::ResourceManager::getInstance().load("EditorLanguage.xml");
	MyGUI::ResourceManager::getInstance().load("FrameworkFonts.xml");
	//MyGUI::ResourceManager::getInstance().load("PointersW32.xml");
	MyGUI::ResourceManager::getInstance().load("EditorTheme.xml");
		
		

	new EditorWidgets();
	EditorWidgets::getInstance().initialise();

	new SettingsManager();
	SettingsManager::getInstance().loadSettingsFile(MyGUI::DataManager::getInstance().getDataPath("Settings.xml"));
	std::string userSettingsFileName = SettingsManager::getInstance().getValue("Editor/UserSettingsFileName");
	if (!userSettingsFileName.empty())
		SettingsManager::getInstance().loadUserSettingsFile(userSettingsFileName);

	mMainPanelControl = new MainPanelControl();

	//InitClient();

	//eventUpdate += MyGUI::newDelegate(&ReceiveMsg);
}  


void EFFEditor::destroyScene()
{
}  
  
void EFFEditor::setupResources()  
{  
    base::BaseManager::setupResources();

    addResourceLocation(getRootMedia() + "/Tools/EFFEditor");
    addResourceLocation(getRootMedia() + "/Tools/EFFEditor/Skin");
	addResourceLocation(getRootMedia() + "/Common/Themes");
	addResourceLocation(getRootMedia() + "/Common/Tools");
	//addResourceLocation(getRootMedia() + "/Common/Base");

	addResourceLocation(getRootMedia() + "/Layout");
	addResourceLocation(getRootMedia() + "/Skin");
	addResourceLocation(getRootMedia() + "/MyGUI_Media");
	addResourceLocation(getRootMedia() + "/Language");
	addResourceLocation(getRootMedia() + "/Themes");
	addResourceLocation(getRootMedia() + "/Settings");
}
 

void EFFEditor::windowResized()
{
	BaseManager::windowResized();

	SendWindowPosAndSize();
}

void EFFEditor::windowMove()
{
	SendWindowPosAndSize();
}


effBYTE buffer[256];

void EFFEditor::ReceiveMsg(effFLOAT elapsedTime)
{
	if (GetClient()->ReceiveMsg(buffer, 256))
	{
		effINT id = *((effINT *)buffer);
		if (id == RequestGameWindowPosAndSize)
		{
			SendWindowPosAndSize();
		}
		else
		{
			int z = 0;
		}
	}
}

/*int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	EFFApplication app;
	app.Init(effTRUE, 1024, 768, effTRUE, effTRUE);


	EFFEditor editor;
	editor.create(app.GetHWND());

	app.OnRenderGUI += EFFEventCall((base::BaseManager *)&editor, &base::BaseManager::drawOneFrame);


	app.Run();


 	return 0;
}*/


VOID InitEditor(EFFApplication * app)
{


	editor = new EFFEditor();
	editor->create(app->GetHWND());

	app->OnRenderGUI += EFFEventCall((base::BaseManager *)editor, &base::BaseManager::drawOneFrame);
}


VOID DestroyEditor()
{

}

VOID CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	if (GetClient()->ReceiveMsg(buffer, 256))
	{
		effINT id = *((effINT *)buffer);
		if (id == RequestGameWindowPosAndSize)
		{
			SendWindowPosAndSize();
		}
		else if (id == SendApp)
		{
			ApplicationInfo * appInfo = (ApplicationInfo *)buffer;
			InitEditor((EFFApplication *)appInfo->app);
		}
		else
		{
			int z = 0;
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			GetClient()->SendCmd(RequestApp);
			SetTimer(NULL, 1, 100, TimerProc);
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{
			DestroyEditor();
		}
		break;
	}

	return TRUE;
}