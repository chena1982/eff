
#include "EFFMyGUIEditorPCH.h"

#include "MainWorkspaceControl.h"
#include "ToolsControl.h"
#include "WorkspaceControl.h"



MainWorkspaceControl::MainWorkspaceControl(MyGUI::Widget* _parent) :
	wraps::BaseLayout("MainWorkspaceControl.layout", _parent),
	mToolsControl(nullptr),
	mWorkspaceControl(nullptr),
	mSeparatorControl(NULL)
{
	//assignBase(mToolsControl, "ToolsControl");
	assignBase(mWorkspaceControl, "WorkspaceControl");

	//assignWidget(mSeparatorControl, "SeparatorV");
}

MainWorkspaceControl::~MainWorkspaceControl()
{
}

