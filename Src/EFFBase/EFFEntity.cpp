/******************************************************************************
created:	2017-07-07   20:50
file path:	d:\EFF\Include\EFFBase\EFFEntity.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFEntity.h"

EFFBASE_BEGIN


EFFEntityManager::EFFEntityManager()
{
}


EFFEntityManager::~EFFEntityManager()
{
}

EFFEntity EFFEntityManager::Create()
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
        assert(idx < (1 << ENTITY_INDEX_BITS));
    }

    EFFEntity entity;
    entity.id = (idx << ENTITY_INDEX_BITS) + generation[idx];

    return entity;
}

effVOID EFFEntityManager::Destroy(EFFEntity e)
{
    const effUINT idx = e.Index();
    generation[idx] += 1;
    free_indices.push_back(idx);
}

EFFBASE_END