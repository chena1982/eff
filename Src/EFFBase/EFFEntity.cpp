/******************************************************************************
created:	2017-07-07   20:50
file path:	d:\EFF\Include\EFFBase\EFFEntity.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFEntity.h"

EFFBASE_BEGIN


RTTI_IMPLEMENT_BASE(EFFIdManager, 0)

EFFIdManager::EFFIdManager()
{
}


EFFIdManager::~EFFIdManager()
{
}

EFFId EFFIdManager::Create()
{
    effUINT idx;
    if (freeIndices.size() > MINIMUM_FREE_INDICES)
    {
        idx = freeIndices.front();
        freeIndices.pop_front();
    }
    else
    {
        generation.push_back(0);
        idx = (effUINT)generation.size() - 1;
        assert(idx < (1 << INDEX_BITS));
    }

    EFFId id;
    id.id = (generation[idx] << INDEX_BITS) + idx;

    return id;
}

effVOID EFFIdManager::Destroy(EFFId e)
{
    const effUINT idx = e.Index();
    generation[idx] += 1;
    freeIndices.push_back(idx);
}

EFFBASE_END