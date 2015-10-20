
#include "EFFMyGUIEditorPCH.h"

#include "Main.h"
#include "MainPanelControl.h"
#include "EditorWidgets.h"
#include "EFFEditorMessageCenter.h"

class EFFEditor;

template <> EFFEditor * MyGUI::Singleton<EFFEditor>::msInstance = NULL;
template <> const char * MyGUI::Singleton<EFFEditor>::mClassTypeName = "EFFEditor";


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
	MyGUI::ResourceManager::getInstance().load("MyGUI_CommonSkins.xml");
	MyGUI::ResourceManager::getInstance().load("MyGUI_DarkSkin.xml");
	//MyGUI::ResourceManager::getInstance().load("SelectionSkins.xml");
	//MyGUI::ResourceManager::getInstance().load("EditorReplaceSkins.xml");
	MyGUI::ResourceManager::getInstance().load("MyGUI_DarkTemplate.xml");
	MyGUI::ResourceManager::getInstance().load("UnitySkin.xml");
	MyGUI::ResourceManager::getInstance().load("UnityTemplate.xml");
	MyGUI::ResourceManager::getInstance().load("EditorLanguage.xml");
	MyGUI::ResourceManager::getInstance().load("PointersW32.xml");
		
		

	new EditorWidgets();
	EditorWidgets::getInstance().initialise();

	mMainPanelControl = new MainPanelControl();

	InitServer();

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
	//addResourceLocation(getRootMedia() + "/Common/Base");
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
	if (GetServer()->ReceiveMsg(buffer, 256))
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

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	EFFApplication app;
	app.Init(effTRUE, 1024, 768, effFALSE, effTRUE);

	EFFEditor editor;
	editor.create(app.GetHWND());

	app.OnRenderGUI += EFFEventCall((base::BaseManager *)&editor, &base::BaseManager::drawOneFrame);
	//app.OnRenderGUI += EFFEventCall(&editor, &EFFEditor::ReceiveMsg);

	//app.OnWindowMove += EFFEventCall(&editor, &EFFEditor::windowMove);

	app.Run();


 	return 0;
}