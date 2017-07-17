/******************************************************************************
created:	2017-07-14   23:35
file path:	d:\EFF\Include\EFFGUI\EFFGUIText.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFGUIText_2017_07_14__
#define __EFFGUIText_2017_07_14__


EFFGUI_BEGIN

class EFFGUILayoutInfo;

effVOID RenderingText(EFFGUILayoutInfo * layoutInfo, EFF3DFont * font, const effString & text);

struct EFFGUIText
{
    effUINT textId;
    effUINT fontId;
};


class EFFGUITextManager : public EFFComponentManager<EFFGUIText>
{
    friend class EFFGUIRenderer;
public:
    EFFGUITextManager() {}
    virtual ~EFFGUITextManager() {}

public:

};

EFFGUI_END

#endif