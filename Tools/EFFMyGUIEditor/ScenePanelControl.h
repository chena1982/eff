#pragma once


#include "BaseLayout/BaseLayout.h"


class ScenePanelControl :
	public wraps::BaseLayout
{
public:
	ScenePanelControl(MyGUI::Widget* _parent = nullptr);
	virtual ~ScenePanelControl();

	MyGUI::Widget * GetSceneWindow() { return sceneWindow; }

protected:
	effVOID notifyChangeCoord(MyGUI::Widget* _sender);
private:
	MyGUI::Widget * sceneWindow;
};


