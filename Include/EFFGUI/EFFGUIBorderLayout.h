/******************************************************************************
created:	2017-07-13   22:18
file path:	d:\EFF\Include\EFFGUI\EFFGUIBorderLayout.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFGUIBorderLayout_2017_07_13__
#define __EFFGUIBorderLayout_2017_07_13__


EFFGUI_BEGIN

class EFFGUILayoutInfoManager;
class EFFGUILayoutInfo;
struct LayoutCalculateSizeContext;

effVOID BorderLayoutCalculateSize(EFFGUILayoutInfo * layoutInfo, EFFGUILayoutInfo * childLayoutInfo, effUINT childCount, LayoutCalculateSizeContext * context);

EFFGUI_END


#endif
