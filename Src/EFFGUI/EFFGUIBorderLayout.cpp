/******************************************************************************
created:	2017-07-13   22:18
file path:	d:\EFF\Include\EFFGUI\EFFGUIBorderLayout.h
author:		ChenA

purpose:
******************************************************************************/

#include "EFFGUIPCH.h"
#include "EFFGUIBorderLayout.h"
#include "EFFGUILayoutInfo.h"


EFFGUI_BEGIN


effVOID BorderLayoutCalculateSize(EFFGUILayoutInfo * layoutInfo, EFFGUILayoutInfo * childLayoutInfo, effUINT childCount, LayoutCalculateSizeContext * context)
{
    FloatCoord & clientRect = context->clientRect;

    for (effUINT i = 0; i < childCount; i++)
    {
        if (childLayoutInfo->align & Align_Top)
        {
            childLayoutInfo->floatCoord = clientRect.point();

            effFLOAT minHeight = childLayoutInfo->floatMinSize.height;

            childLayoutInfo->floatCoord.width = clientRect.width;
            childLayoutInfo->floatCoord.height = minHeight;

            clientRect.top += minHeight;
            clientRect.height -= minHeight;
        }
        else if (childLayoutInfo->align & Align_Left)
        {
            childLayoutInfo->floatCoord = clientRect.point();

            effFLOAT minWidth = childLayoutInfo->floatMinSize.width;

            childLayoutInfo->floatCoord.width = minWidth;
            childLayoutInfo->floatCoord.height = clientRect.height;

            clientRect.left += minWidth;
            clientRect.width -= minWidth;
        }
        else if (childLayoutInfo->align & Align_Right)
        {
            effFLOAT minWidth = childLayoutInfo->floatMinSize.width;

            childLayoutInfo->floatCoord.left = clientRect.right() - minWidth;
            childLayoutInfo->floatCoord.top = clientRect.top;

            childLayoutInfo->floatCoord.width = minWidth;
            childLayoutInfo->floatCoord.height = clientRect.height;

            clientRect.width -= minWidth;
        }
        else if (childLayoutInfo->align & Align_Bottom)
        {
            effFLOAT minHeight = childLayoutInfo->floatMinSize.height;

            childLayoutInfo->floatCoord.left = clientRect.left;
            childLayoutInfo->floatCoord.top = clientRect.bottom() - minHeight;

            childLayoutInfo->floatCoord.width = clientRect.width;
            childLayoutInfo->floatCoord.height = minHeight;

            clientRect.height -= minHeight;
        }
        else if ((childLayoutInfo->align & Align_HCenter) && (childLayoutInfo->align & Align_VCenter))
        {
            childLayoutInfo->floatCoord = clientRect;
        }

        childLayoutInfo++;
    }
}

EFFGUI_END