/******************************************************************************
created:	2017-07-07   20:50
file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFEntity.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFEntity_2017_07_07__
#define __EFFEntity_2017_07_07__


EFFBASE_BEGIN

const effUINT ENTITY_INDEX_BITS = 20;
const effUINT ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;


const effUINT ENTITY_GENERATION_BITS = 8;
const effUINT ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;

const effUINT MINIMUM_FREE_INDICES = 1024;

class EFFBASE_API EFFEntity
{
public:
    EFFEntity() { id = 0; }
    ~EFFEntity() {}
public:
    inline effUINT Index() const { return id & ENTITY_INDEX_MASK; }
    inline effUINT Generation() const { return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }
public:
    effUINT id;
};

class EFFBASE_API EntityManager
{
public:
    EntityManager();
    ~EntityManager();
public:
    EFFEntity Create();
    
    inline effBOOL Alive(EFFEntity e) const
    {
        return generation[e.Index()] == e.Generation();
    }

    effVOID Destroy(EFFEntity e);


private:
    VECTOR<effBYTE> generation;
    DEQUE<effUINT> free_indices;
};

EFFBASE_END


#endif