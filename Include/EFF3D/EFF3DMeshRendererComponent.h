/******************************************************************************
created:	2018-04-03   23:29
file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DMeshRendererComponent.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFF3DMeshRendererComponent_2018_04_03__
#define __EFF3DMeshRendererComponent_2018_04_03__

#include "EFF3DRenderable.h"

EFF3D_BEGIN

class EFF3DMeshRendererComponent : public EFF3DRenderable
{
protected:
    EFF3DMeshRendererComponent();
    virtual ~EFF3DMeshRendererComponent();
private:

    EFF3DVertexDeclarationHandle    vertexDecl;
    EFF3DVertexBufferHandle         vertexBuffer;
    EFF3DIndexBufferHandle          indexBuffer;
};

EFF3D_END

#endif
