/******************************************************************************
created:	2017-07-07   21:51
file path:	d:\EFF\Src\EFFGUI\EFFGUIWidget.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFFGUIPCH.h"
#include "EFFGUIWidget.h"


EFFGUI_BEGIN


WidgetInfo::WidgetInfo()
{
    align = Align_Default;
    style = WidgetStyle::Child;
    positionType = Relative;
}

WidgetInfo::~WidgetInfo()
{
}

WidgetManager::WidgetManager() : widgetInfos(100)
{
}

WidgetManager::~WidgetManager()
{
}

effVOID WidgetManager::LoadLayout(const effString & filePath)
{
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(EFFSTRING2ANSI(filePath));

	if (!result)
	{
		return;
	}

	pugi::xml_node root = doc.child("Root");
	if (!root || root.name() != "EFFGUI")
	{
		return;
	}

	pugi::xml_node widgetNode = root.child("Widget");
	while (widgetNode)
	{
		LoadWidget(widgetNode);

		widgetNode = widgetNode.next_sibling();

	}
}

effVOID WidgetManager::LoadWidget(const pugi::xml_node & widgetNode)
{
	WidgetInfo * widgetInfo = EFFNEW WidgetInfo();

	effINT id = widgetInfos.Add(widgetInfo);
	widgetInfo->id = id;


	effStringA tmp;

	widgetInfo->type = widgetNode.attribute("type").as_string();
	widgetInfo->skin = widgetNode.attribute("skin").as_string();
	widgetInfo->layer = widgetNode.attribute("layer").as_string();

	pugi::xml_attribute attr = widgetNode.attribute("align");
	if (attr)
	{
		//widgetInfo.align = Align::parse(tmp);
	}

	widgetInfo->name = widgetNode.attribute("name").as_string();

	attr = widgetNode.attribute("style");
	if (attr)
	{
		//widgetInfo.style = WidgetStyle::parse(tmp);
	}

	IntCoord coord;
	attr = widgetNode.attribute("position");
	if (attr)
	{
		widgetInfo->intCoord = IntCoord::parse(attr.as_string());
		widgetInfo->positionType = PositionType::Absolute;
	}

	attr = widgetNode.attribute("position_real");
	if (attr)
	{
		widgetInfo->floatCoord = FloatCoord::parse(attr.as_string());
		widgetInfo->positionType = PositionType::Relative;
	}

	pugi::xml_node widgetChildNode = widgetNode.first_child();
	while (widgetChildNode)
	{
		if (strcmp(widgetChildNode.name(), "Widget") == 0)
		{
			LoadWidget(widgetChildNode);
			//widgetInfo.childWidgetsInfo.push_back(parseWidget(node));
		}
		else if (strcmp(widgetChildNode.name(), "Property") == 0)
		{
			//widgetInfo.properties.push_back(PairString(node->findAttribute("key"), node->findAttribute("value")));
		}
		else if (strcmp(widgetChildNode.name(), "UserString") == 0)
		{
			//widgetInfo.userStrings[node->findAttribute("key")] = node->findAttribute("value");
		}
		else if (strcmp(widgetChildNode.name(), "Controller") == 0)
		{
			/*ControllerInfo controllerInfo;
			controllerInfo.type = node->findAttribute("type");

			xml::ElementEnumerator prop = node->getElementEnumerator();
			while (prop.next("Property"))
				controllerInfo.properties[prop->findAttribute("key")] = prop->findAttribute("value");

			widgetInfo.controllers.push_back(controllerInfo);*/
		}

		widgetChildNode = widgetChildNode.next_sibling();
	}
}

EFFGUI_END