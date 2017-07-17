/******************************************************************************
created:	2017-07-10   22:29
file path:	d:\EFF\Include\EFFGUI\EFFGUILayoutInfo.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFGUILayoutInfo_2017_07_10__
#define __EFFGUILayoutInfo_2017_07_10__



EFFGUI_BEGIN

enum LayoutType
{
    FlowLayout = 0,
    FlexBoxLayout,
    GridLayout,
    BorderLayout
};

struct LayoutCalculateSizeContext
{
    FloatPoint currentPos;
    FloatCoord clientRect;
};

class EFFGUILayoutInfo
{
public:
    EFFGUILayoutInfo()
    {
        align = Align_Default;
        sizeMode = SizeMode_HStretch | SizeMode_VStretch;


        positionType = Position_Relative;
        type = FlowLayout;
    }

    ~EFFGUILayoutInfo() {}

public:
    EFFTreeNode treeNode;
	effUINT align;
	effUINT sizeMode;
    IntCoord margin;

    PositionType positionType;
    IntCoord intCoord;
    FloatCoord floatCoord;

    IntSize intMinSize;
    FloatSize floatMinSize;

    LayoutType type;
};

class EFFGUILayoutInfoManager : public EFFTreeComponentManager<EFFGUILayoutInfo>
{
    friend class EFFGUIRenderer;
public:
    EFFGUILayoutInfoManager();
    virtual ~EFFGUILayoutInfoManager();
public:

    effVOID CalculateSize();

};


EFFGUI_END

#endif
