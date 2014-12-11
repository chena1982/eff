#pragma once

#include "BaseLayout/BaseLayout.h"


class MainMenuControl;
class MainWorkspaceControl;

class MainPanelControl :
	public wraps::BaseLayout
{
public:
	MainPanelControl();
	virtual ~MainPanelControl();

	void setVisible(bool _value);

	MainWorkspaceControl * GetMainWorkspaceControl() { return mMainWorkspaceControl; }
private:
	MainMenuControl * mMainMenuControl;
	MainWorkspaceControl* mMainWorkspaceControl;
};