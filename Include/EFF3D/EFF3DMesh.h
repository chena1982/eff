/******************************************************************************
created:	2018-04-03   23:29
file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DMesh.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFF3DMesh_2018_04_03__
#define __EFF3DMesh_2018_04_03__


EFF3D_BEGIN

class EFF3DSubmesh;

class EFF3DMesh
{
protected:
    EFF3DMesh();
    virtual ~EFF3DMesh();
private:


    VECTOR<EFF3DSubmesh *>   submeshes;
};

EFF3D_END

#endif
