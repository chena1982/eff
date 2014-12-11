#pragma once

#include "BaseLayout/BaseLayout.h"



class ToolsControl;
class WorkspaceControl;

class MainWorkspaceControl : public wraps::BaseLayout
{
public:
	MainWorkspaceControl(MyGUI::Widget* _parent);
	virtual ~MainWorkspaceControl();

	WorkspaceControl * GetWorkspaceControl() { return mWorkspaceControl; }
private:
	ToolsControl* mToolsControl;
	WorkspaceControl* mWorkspaceControl;
	MyGUI::SeparatorV * mSeparatorControl;
};

