/******************************************************************************
created:	2017-07-07   21:49
file path:	d:\EFF\Include\EFFGUI\EFFGUIWidget.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFGUIWidget_2017_07_07__
#define __EFFGUIWidget_2017_07_07__

#include "EFFGUICoord.h"

EFFGUI_BEGIN

enum WidgetStyle
{
    Child = 0,
    Popup,
    Window,
};



struct EFFGUI_API ControllerInfo
{
    effString type;
    MAP<effString, effString> properties;
};


struct EFFGUI_API WidgetInfo
{
public:
    WidgetInfo();
    ~WidgetInfo();

public:
    VECTOR<WidgetInfo> childWidgetsInfo;
    // not map because there might be several properties with same name (for example List items)
    MAP<effStringA, effStringA> properties;
    MAP<effStringA, effStringA> userStrings;
    VECTOR<ControllerInfo> controllers;
    effStringA type;
    effStringA skin;
    effUINT align;
    WidgetStyle style;
    effStringA name;
    effStringA layer;


    PositionType positionType;
    IntCoord intCoord;
    FloatCoord floatCoord;

	effINT id;
};

class EFFGUI_API WidgetManager
{
public:
	WidgetManager();
	~WidgetManager();

public:
	effVOID LoadLayout(const effString & filePath);

protected:
	effVOID LoadWidget(const pugi::xml_node & widgetNode);

private:
	EFFFastIdMap<WidgetInfo> widgetInfos;
};

EFFGUI_END

#endif