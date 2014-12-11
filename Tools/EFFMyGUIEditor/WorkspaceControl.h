#pragma once


#include "BaseLayout/BaseLayout.h"


class ScenePanelControl;

class WorkspaceControl :
	public wraps::BaseLayout
{
public:
	WorkspaceControl(MyGUI::Widget* _parent = nullptr);
	virtual ~WorkspaceControl();


	ScenePanelControl * GetScenePanelControl() { return mScenePanelControl; }
private:
	ScenePanelControl * mScenePanelControl;
	MyGUI::SeparatorH * mSeparatorControl;
};


