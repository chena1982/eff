
#include "EFFMyGUIEditorPCH.h"

#include "ToolsControl.h"
//#include "PropertiesPanelView.h"
//#include "WidgetsWindow.h"
//#include "ProjectControl.h"

ToolsControl::ToolsControl(MyGUI::Widget* _parent) :
	wraps::BaseLayout("ToolsControl.layout", _parent)
	//mPropertiesPanelView(nullptr),
	//mWidgetsWindow(nullptr),
	//mProjectControl(nullptr)
{
	//assignBase(mPropertiesPanelView, "PropertiesControl");
	//assignBase(mWidgetsWindow, "WidgetsControl");
	//assignBase(mProjectControl, "ProjectControl");
}

ToolsControl::~ToolsControl()
{
}

