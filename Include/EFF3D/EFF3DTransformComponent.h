/******************************************************************************
created:	2018-03-01   22:39
file path:	d:\EFF\Include\EFF3D\EFF3DTransformComponent.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFF3DTransformComponent_2018_3_1__
#define __EFF3DTransformComponent_2018_3_1__


EFF3D_BEGIN

class EFF3DTransformComponent : public EFFComponent
{
    friend class EFF3DStaticMeshComponentManager;
    RTTI_DECLARE(EFF3DTransformComponent, EFFComponent)
public:
    virtual ~EFF3DTransformComponent();

protected:
    EFF3DTransformComponent();

};

class EFF3DTransformComponentManager : public EFFComponentManager<EFF3DTransformComponent>
{
public:
    EFF3DTransformComponentManager();
    virtual ~EFF3DTransformComponentManager();

};


EFF3D_END


#endif
