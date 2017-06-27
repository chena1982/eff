/******************************************************************************
created:	20017-06-24   00:43
file path:	d:\EFF\Include\EFFD3D9Query.h
author:		ChenA

purpose:
******************************************************************************/



#include "EFFD3D9PCH.h"
#include "EFFD3D9Query.h"

EFFD3D9Query::EFFD3D9Query()
{
    query = NULL;
}


EFFD3D9Query::~EFFD3D9Query()
{
    SF_RELEASE(query);
}

effHRESULT EFFD3D9Query::Issue(effUINT flag)
{
    if (query == NULL)
    {
        return EFF3DERR_INVALIDCALL;
    }

    return query->Issue(flag);
}

effHRESULT EFFD3D9Query::GetData(effVOID * data, effUINT size, effUINT flag)
{
    if (query == NULL)
    {
        return EFF3DERR_INVALIDCALL;
    }

    return query->GetData(data, size, flag);
}