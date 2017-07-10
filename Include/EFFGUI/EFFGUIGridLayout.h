/******************************************************************************
created:	2017-07-10   22:29
file path:	d:\EFF\Include\EFFGUI\EFFGUIGridLayout.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFGUIGridLayout_2017_07_10__
#define __EFFGUIGridLayout_2017_07_10__

#include "EFFGUICoord.h"

EFFGUI_BEGIN

class EFFGUIGridLayout
{
public:
    EFFGUIGridLayout();
	~EFFGUIGridLayout();
public:
    effUINT align;
    effUINT sizeMode;
    IntCoord margin;
    effINT row;
    effINT column;
};



EFFGUI_END

#endif
