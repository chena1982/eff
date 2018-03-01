/******************************************************************************
created:	2018-03-01   22:39
file path:	d:\EFF\Include\EFF3D\EFF3DStaticMeshComponent.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFF3DStaticMeshComponent_2018_3_1__
#define __EFF3DStaticMeshComponent_2018_3_1__


EFF3D_BEGIN

class EFF3DStaticMeshComponent : public EFFComponent
{
    friend class EFF3DStaticMeshComponentManager;
    RTTI_DECLARE(EFF3DStaticMeshComponent, EFFComponent)
public:
    virtual ~EFF3DStaticMeshComponent();

protected:
    EFF3DStaticMeshComponent();

};

class EFF3DStaticMeshComponentManager : public EFFComponentManager<EFF3DStaticMeshComponent>
{
public:
    EFF3DStaticMeshComponentManager();
    virtual ~EFF3DStaticMeshComponentManager();

};


EFF3D_END


#endif
