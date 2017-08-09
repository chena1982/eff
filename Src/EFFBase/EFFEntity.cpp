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
    if (free_indices.size() > MINIMUM_FREE_INDICES)
    {
        idx = free_indices.front();
        free_indices.pop_front();
    }
    else
    {
        generation.push_back(0);
        idx = (effUINT)generation.size() - 1;
        assert(idx < (1 << INDEX_BITS));
    }

    EFFId id;
    id.id = (idx << INDEX_BITS) + generation[idx];

    return id;
}

effVOID EFFIdManager::Destroy(EFFId e)
{
    const effUINT idx = e.Index();
    generation[idx] += 1;
    free_indices.push_back(idx);
}

EFFBASE_END