#pragma once

#include "BaseLayout/BaseLayout.h"

//class PropertiesPanelView;
//class WidgetsWindow;
//class ProjectControl;


class ToolsControl :
	public wraps::BaseLayout
{
public:
	ToolsControl(MyGUI::Widget* _parent = nullptr);
	virtual ~ToolsControl();

private:
	//PropertiesPanelView* mPropertiesPanelView;
	//WidgetsWindow* mWidgetsWindow;
	//ProjectControl* mProjectControl;
};

