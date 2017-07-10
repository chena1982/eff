/******************************************************************************
created:	2017-07-10   22:29
file path:	d:\EFF\Include\EFFGUI\EFFGUICommonLayout.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFGUICommonLayout_2017_07_10__
#define __EFFGUICommonLayout_2017_07_10__

#include "EFFGUICoord.h"

EFFGUI_BEGIN

class EFFGUICommonLayout
{
public:
    EFFGUICommonLayout()
    {
        align = Align_Default;
        sizeMode = SizeMode_HStretch | SizeMode_VStretch;
    }

    ~EFFGUICommonLayout() {}

public:
	effUINT align;
	effUINT sizeMode;
    IntCoord margin;

    EFFTreeNode<EFFGUICommonLayout> treeNode;
};

class EFFGUICommonLayoutManager : public EFFTreeComponentManager<EFFGUICommonLayout>
{
public:
    EFFGUICommonLayoutManager();
    virtual ~EFFGUICommonLayoutManager();
public:
    EFFGUICommonLayout *    GetParent(EFFEntity entity);
    EFFGUICommonLayout *    GetFirstChild(EFFEntity entity);
    EFFGUICommonLayout *    GetLastChild(EFFEntity entity);
    effUINT                 GetDepth(EFFEntity entity);
    effUINT                 SetDepth(EFFEntity entity, effUINT depth)
    {
        EFFGUICommonLayout * layout = (EFFGUICommonLayout *)components[entity.Index()];
        if (layout != NULL)
        {
            layout->treeNode.depth = depth;
        }
    }
};


EFFGUI_END

#endif
