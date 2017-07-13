/******************************************************************************
created:	2017-07-10   22:29
file path:	d:\EFF\Include\EFFGUI\EFFGUILayoutInfo.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFFGUIPCH.h"
#include "EFFGUILayoutInfo.h"

#include "EFFGUIFlowLayout.h"
#include "EFFGUIBorderLayout.h"
#include "EFFGUIGridLayout.h"

EFFGUI_BEGIN

EFFGUILayoutInfoManager::EFFGUILayoutInfoManager()
{
}

EFFGUILayoutInfoManager::~EFFGUILayoutInfoManager()
{
}



effVOID EFFGUILayoutInfoManager::CalculateSize()
{
    EFF3DDevice * device = EFF3DGetDevice();
    effINT clientWidth = device->GetBackBufferWidth();
    effINT clientHeight = device->GetBackBufferHeight();



    LayoutCalculateSizeContext context;


    for (effUINT i = 0; i < TreeComponent_MaxDepth - 1; i++)
    {
        effINT hStretchCount, vStretchCount = 0;

        for (effUINT j = 0; j < (effUINT)datas[i].size(); j++)
        {
            EFFGUILayoutInfo & layoutInfo = datas[i][j];
            EFFTreeNode * treeNode = (EFFTreeNode *)&layoutInfo;
            EFFGUILayoutInfo * firstChild = (EFFGUILayoutInfo *)&datas[i + 1][treeNode->firstChild];


            context.clientRect = layoutInfo.floatCoord;

            if (layoutInfo.type == BorderLayout)
            {
                BorderLayoutCalculateSize(&layoutInfo, firstChild, treeNode->lastChild - treeNode->firstChild, &context);
            }

            if (layoutInfo.sizeMode & SizeMode_HStretch)
            {
                hStretchCount += 1;
            }

            if (layoutInfo.sizeMode & SizeMode_VStretch)
            {
                vStretchCount += 1;
            }
        }
    }
}

EFFGUI_END