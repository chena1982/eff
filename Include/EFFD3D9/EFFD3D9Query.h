/******************************************************************************
created:	20017-06-24   00:43
file path:	d:\EFF\Include\EFFD3D9Query.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFD3D9Query_2017_06_24__
#define __EFFD3D9Query_2017_06_24__



class EFFD3D9Query : public EFF3DQuery
{
    friend class EFFD3D9Device;
public:
    EFFD3D9Query();
    virtual ~EFFD3D9Query();
public:
    virtual effHRESULT Issue(effUINT flag);
    virtual effHRESULT GetData(effVOID * data, effUINT size, effUINT flag);
protected:
    IDirect3DQuery9 * query;
};

#endif