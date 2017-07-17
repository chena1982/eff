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
}

EFFGUIRenderer::~EFFGUIRenderer()
{

}

effVOID EFFGUIRenderer::GenerateDrawList(EFFGUILayoutInfoManager * layoutManager, EFFGUIImageManager * imageManager, EFFGUITextManager * textManager)
{
    effUINT topWindowCount = (effUINT)layoutManager->datas[0].size();

    imageDrawLists.resize(topWindowCount);
    textDrawLists.resize(topWindowCount);

    //遍历所有的top layoutinfo
    for (effUINT i = 0; i < topWindowCount; i++)
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

            EFFGUILayoutInfo * childLayoutInfo = &layoutManager->datas[j][firstChild];
            EFFGUILayoutInfo * lastLayoutInfo = &layoutManager->datas[j][lastChild];

            effUINT imageFirstChild = imageManager->GetComponent(childLayoutInfo->treeNode.entity);
            effUINT imageLastChild = imageManager->GetComponent(childLayoutInfo->treeNode.entity);

            if (imageFirstChild != -1)
            {
                for (effUINT k = imageFirstChild; k <= imageLastChild; k++)
                {
                    EFFGUIImage * image = imageManager->GetComponentPoint(k);
                }
            }

            effUINT textFirstChild = textManager->GetComponent(childLayoutInfo->treeNode.entity);
            effUINT textLastChild = textManager->GetComponent(childLayoutInfo->treeNode.entity);

            if (textFirstChild != -1)
            {
                for (effUINT k = textFirstChild; k <= textLastChild; k++)
                {
                    EFFGUIText * text = textManager->GetComponentPoint(k);
                }
            }


            firstChild = childLayoutInfo->treeNode.firstChild;
            lastChild = childLayoutInfo->treeNode.lastChild;
        }
    }
}


EFFGUI_END