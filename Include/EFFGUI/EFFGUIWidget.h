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

enum PositionType
{
    Absolute = 0, 
    Relative, 
};

enum Align
{
    HCenter = 1,
    VCenter = 2,
    Center = HCenter | VCenter,

    Left = 4,
    Right = 8,
    HStretch = Left | Right,

    Top = 16,
    Bottom = 32,
    VStretch = Top | Bottom,

    Stretch = HStretch | VStretch,
    Default = Left | Top
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
    Align align;
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