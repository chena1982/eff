
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

	eventUpdate += MyGUI::newDelegate(&ReceiveMsg);
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

MYGUI_APP(EFFEditor)  