/******************************************************************************
created:	2017-07-10   22:29
file path:	d:\EFF\Include\EFFGUI\EFFGUIGridLayout.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFFGUIPCH.h"
#include "EFFGUIGridLayout.h"


EFFGUI_BEGIN

EFFGUIGridLayout::EFFGUIGridLayout()
{
    align = Align_Default;
    sizeMode = SizeMode_HStretch | SizeMode_VStretch;
    row = 1;
    column = 1;
}


EFFGUIGridLayout::~EFFGUIGridLayout()
{

}


EFFGUI_END