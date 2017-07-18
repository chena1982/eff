/******************************************************************************
created:	2017-07-14   23:35
file path:	d:\EFF\Include\EFFGUI\EFFGUIRendering.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFGUIRendering_2017_07_14__
#define __EFFGUIRendering_2017_07_14__


EFFGUI_BEGIN



struct EFFGUIDrawCmd
{
    EFFGUIDrawCmd() { elemCount = 0; }


    effUINT   elemCount;
    effUINT   textureId;
};


struct EFFGUIDrawVert
{
    EFFVector2  pos;
    EFFVector2  uv;
    //effUINT     color;
};

struct EFFGUIDrawList
{
    VECTOR<EFFGUIDrawCmd>     CmdBuffer;          // Commands. Typically 1 command = 1 GPU draw call.
    VECTOR<effWORD>           IdxBuffer;          // Index buffer.
    VECTOR<EFFGUIDrawVert>    VtxBuffer;          // Vertex buffer.
};


class EFFGUILayoutInfoManager;
class EFFGUIImageManager;
class EFFGUITextManager;



class EFFGUIRenderer
{
public:
    EFFGUIRenderer();
    ~EFFGUIRenderer();

public:
    effVOID GenerateDrawList(EFFGUILayoutInfoManager * layoutManager, EFFGUIImageManager * imageManager, EFFGUITextManager * textManager);

    effVOID GenerateImageRenderCmd(effVOID * manager, effUINT index, effVOID * userData);
    effVOID GenerateTextRenderCmd(effVOID * manager, effUINT index, effVOID * userData);
private:
    VECTOR<EFFGUIDrawList> imageDrawLists;
    VECTOR<EFFGUIDrawList> textDrawLists;
};

EFFGUI_END

#endif
