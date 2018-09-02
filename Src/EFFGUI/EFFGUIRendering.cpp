/******************************************************************************
created:	2017-07-14   23:35
file path:	d:\EFF\Include\EFFGUI\EFFGUIRendering.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFFGUIPCH.h"
#include "EFFGUIRendering.h"
#include "EFFGUILayoutInfo.h"
#include "EFFGUIImage.h"
#include "EFFGUIText.h"


EFFGUI_BEGIN



EFFGUIRenderer::EFFGUIRenderer()
{
    //boost::function<effVOID(EFFGUIImageManager *, effUINT)> f = GDL;
}

EFFGUIRenderer::~EFFGUIRenderer()
{

}

struct GenerateRenderCmdUserData
{
    EFFGUILayoutInfoManager * layoutManager;
    EFFGUIDrawList * drawList;
};

effVOID EFFGUIRenderer::GenerateImageRenderCmd(effVOID * manager, effUINT index, effVOID * userData)
{
    EFFGUIImageManager * imageManager = (EFFGUIImageManager *)manager;
    EFFGUIImage * image = imageManager->GetComponentPoint(index);

    GenerateRenderCmdUserData * grcUserData = (GenerateRenderCmdUserData *)userData;

    EFFGUILayoutInfoManager * layoutManager = grcUserData->layoutManager;
    EFFGUIDrawList * drawList = grcUserData->drawList;


    EFFGUILayoutInfo * layoutInfo = layoutManager->GetComponentPoint(image->entity);

    EFFGUIDrawCmd * currentCmd = NULL;

    const effWCHAR * textureName = EFF3DGetDevice()->GetStaticStringManager()->GetString(image->textureNameId);
    EFFId textureId = EFF3DGetDevice()->GetTextureManager()->AsyncCreateFromFile(textureName, EFF3DResourceType_Texture2D);

    if (drawList->CmdBuffer.size() == 0)
    {
        EFFGUIDrawCmd cmd;
        cmd.textureId = textureId;

        drawList->CmdBuffer.push_back(cmd);
        currentCmd = &drawList->CmdBuffer[0];
    }
    else
    {
        currentCmd = &drawList->CmdBuffer[drawList->CmdBuffer.size() - 1];

        if (textureId != currentCmd->textureId)
        {
            EFFGUIDrawCmd cmd;
            cmd.textureId = textureId;

            drawList->CmdBuffer.push_back(cmd);
            currentCmd = &drawList->CmdBuffer[drawList->CmdBuffer.size() - 1];
        }
    }


    effUINT vertexCount = (effUINT)drawList->VtxBuffer.size();

    EFFGUIDrawVert vertex;

    // 0------1
    // |      |
    // |      |
    // 2------3

    vertex.pos.x = layoutInfo->floatCoord.left;
    vertex.pos.y = layoutInfo->floatCoord.top;
    vertex.uv.x = 0.0f;
    vertex.uv.y = 0.0f;
    drawList->VtxBuffer.push_back(vertex);

    vertex.pos.x = layoutInfo->floatCoord.right();
    vertex.pos.y = layoutInfo->floatCoord.top;
    vertex.uv.x = 1.0f;
    vertex.uv.y = 0.0f;
    drawList->VtxBuffer.push_back(vertex);

    vertex.pos.x = layoutInfo->floatCoord.left;
    vertex.pos.y = layoutInfo->floatCoord.bottom();
    vertex.uv.x = 0.0f;
    vertex.uv.y = 1.0f;
    drawList->VtxBuffer.push_back(vertex);

    vertex.pos.x = layoutInfo->floatCoord.right();
    vertex.pos.y = layoutInfo->floatCoord.bottom();
    vertex.uv.x = 1.0f;
    vertex.uv.y = 1.0f;
    drawList->VtxBuffer.push_back(vertex);




    drawList->IdxBuffer.push_back(vertexCount + 0);
    drawList->IdxBuffer.push_back(vertexCount + 1);
    drawList->IdxBuffer.push_back(vertexCount + 2);
    drawList->IdxBuffer.push_back(vertexCount + 1);
    drawList->IdxBuffer.push_back(vertexCount + 2);
    drawList->IdxBuffer.push_back(vertexCount + 3);

    currentCmd->elemCount += 2;
}

effVOID EFFGUIRenderer::GenerateTextRenderCmd(effVOID * manager, effUINT index, effVOID * userData)
{
    EFFGUITextManager * textManager = (EFFGUITextManager *)manager;
    EFFGUIText * text = textManager->GetComponentPoint(index);

    GenerateRenderCmdUserData * grcUserData = (GenerateRenderCmdUserData *)userData;

    EFFGUILayoutInfoManager * layoutManager = grcUserData->layoutManager;
    EFFGUIDrawList * drawList = grcUserData->drawList;


    EFFGUILayoutInfo * layoutInfo = layoutManager->GetComponentPoint(text->entity);

    EFFGUIDrawCmd * currentCmd = NULL;


    EFFId textureId = EFF3DGetDevice()->GetFontManager()->GetFontTextureId(text->fontId);

    if (drawList->CmdBuffer.size() == 0)
    {
        EFFGUIDrawCmd cmd;
        cmd.textureId = textureId;

        drawList->CmdBuffer.push_back(cmd);
        currentCmd = &drawList->CmdBuffer[0];
    }
    else
    {
        currentCmd = &drawList->CmdBuffer[drawList->CmdBuffer.size() - 1];

        if (textureId != currentCmd->textureId)
        {
            EFFGUIDrawCmd cmd;
            cmd.textureId = textureId;

            drawList->CmdBuffer.push_back(cmd);
            currentCmd = &drawList->CmdBuffer[drawList->CmdBuffer.size() - 1];
        }
    }

    effUINT vertexCount = (effUINT)drawList->VtxBuffer.size();


    EFFStaticStringManager * strManager = EFF3DGetDevice()->GetStaticStringManager();


    effUINT textLength = strManager->Length(text->fontId);

    drawList->VtxBuffer.resize(vertexCount + textLength * 4);

    effUINT indexCount = (effUINT)drawList->IdxBuffer.size();
    drawList->IdxBuffer.resize(indexCount + textLength * 6);


    EFF3DGetDevice()->GetFontManager()->GenerateTextRenderCmd(text->fontId, &drawList->VtxBuffer[vertexCount], &drawList->IdxBuffer[indexCount],
        vertexCount, text->textId, layoutInfo->floatCoord.left, layoutInfo->floatCoord.top, layoutInfo->floatCoord.width, layoutInfo->floatCoord.height, 0);

    currentCmd->elemCount += textLength * 2;
}

effVOID EFFGUIRenderer::GenerateDrawList(EFFGUILayoutInfoManager * layoutManager, EFFGUIImageManager * imageManager, EFFGUITextManager * textManager)
{



    effUINT topWindowCount = (effUINT)layoutManager->datas[0].size();

    imageDrawLists.resize(topWindowCount);
    textDrawLists.resize(topWindowCount);


    for (effUINT i = 0; i < topWindowCount; i++)
    {
        EFFGUILayoutInfo * topLayoutInfo = &layoutManager->datas[0][i];


        GenerateRenderCmdUserData grcUserData;
        grcUserData.layoutManager = layoutManager;
        grcUserData.drawList = &imageDrawLists[i];

        layoutManager->ForEach(topLayoutInfo->treeNode.entity, imageManager, boost::bind(&EFFGUIRenderer::GenerateImageRenderCmd, this, _1, _2, _3), &grcUserData);

        layoutManager->ForEach(topLayoutInfo->treeNode.entity, textManager, boost::bind(&EFFGUIRenderer::GenerateTextRenderCmd, this, _1, _2, _3), &grcUserData);
    }



    //遍历所有的top layoutinfo
    /*for (effUINT i = 0; i < topWindowCount; i++)
    {
        EFFGUILayoutInfo * topLayoutInfo = &layoutManager->datas[0][i];

        effUINT firstChild = topLayoutInfo->treeNode.firstChild;
        effUINT lastChild = topLayoutInfo->treeNode.lastChild;

        

        //遍历树的1-最下层
        for (effUINT j = 1; j < TreeComponent_MaxDepth; j++)
        {
            if (firstChild == -1)
            {
                continue;
            }

            EFFGUILayoutInfo * firstChildLayoutInfo = &layoutManager->datas[j][firstChild];
            EFFGUILayoutInfo * lastChildLayoutInfo = &layoutManager->datas[j][lastChild];

            effUINT imageFirstChild = imageManager->GetComponent(firstChildLayoutInfo->treeNode.entity);
            effUINT imageLastChild = imageManager->GetComponent(lastChildLayoutInfo->treeNode.entity);

            if (imageFirstChild != -1)
            {
                for (effUINT k = imageFirstChild; k <= imageLastChild; k++)
                {
                    EFFGUIImage * image = imageManager->GetComponentPoint(k);
                }
            }

            effUINT textFirstChild = textManager->GetComponent(firstChildLayoutInfo->treeNode.entity);
            effUINT textLastChild = textManager->GetComponent(lastChildLayoutInfo->treeNode.entity);

            if (textFirstChild != -1)
            {
                for (effUINT k = textFirstChild; k <= textLastChild; k++)
                {
                    EFFGUIText * text = textManager->GetComponentPoint(k);
                }
            }


            firstChild = firstChildLayoutInfo->treeNode.firstChild;
            lastChild = lastChildLayoutInfo->treeNode.lastChild;
        }
    }*/
}


effVOID EFFGUIRenderer::DrawOneFrame()
{

}

EFFGUI_END