
#include "EFFMyGUIEditorPCH.h"

#include "ScenePanelControl.h"
#include "EFFEditorMessageCenter.h"



ScenePanelControl::ScenePanelControl(MyGUI::Widget* _parent) :
	wraps::BaseLayout("ScenePanelControl.layout", _parent)
{
	assignWidget(sceneWindow, "SceneWindow");

	sceneWindow->eventChangeCoord += MyGUI::newDelegate(this, &ScenePanelControl::notifyChangeCoord);
}

ScenePanelControl::~ScenePanelControl()
{
}


effVOID ScenePanelControl::notifyChangeCoord(MyGUI::Widget* _sender)
{
	SendWindowPosAndSize();
}



