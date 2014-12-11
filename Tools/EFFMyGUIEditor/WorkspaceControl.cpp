
#include "EFFMyGUIEditorPCH.h"

#include "WorkspaceControl.h"

#include "ScenePanelControl.h"


WorkspaceControl::WorkspaceControl(MyGUI::Widget* _parent) :
	wraps::BaseLayout("WorkspaceControl.layout", _parent),
	mScenePanelControl(NULL),
	mSeparatorControl(NULL)
{
	assignBase(mScenePanelControl, "ScenePanelControl");

	assignWidget(mSeparatorControl, "SeparatorH");
}

WorkspaceControl::~WorkspaceControl()
{
}




