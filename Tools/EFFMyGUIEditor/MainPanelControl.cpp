
#include "EFFMyGUIEditorPCH.h"

#include "MainPanelControl.h"
#include "MainMenuControl.h"
#include "MainWorkspaceControl.h"

MainPanelControl::MainPanelControl() :
	wraps::BaseLayout("MainPanel.layout"),
	mMainMenuControl(NULL),
	mMainWorkspaceControl(NULL)
{
	assignBase(mMainMenuControl, "MainMenuControl");
	assignBase(mMainWorkspaceControl, "MainWorkspaceControl");
}

MainPanelControl::~MainPanelControl()
{
}

void MainPanelControl::setVisible(bool _value)
{
	mMainWidget->setVisible(_value);
}