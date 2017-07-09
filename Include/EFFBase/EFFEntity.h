/******************************************************************************
created:	2017-07-07   20:50
file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFEntity.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFEntity_2017_07_07__
#define __EFFEntity_2017_07_07__


EFFBASE_BEGIN

const effUINT ENTITY_INDEX_BITS = 24;
const effUINT ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;

const effUINT ENTITY_GENERATION_BITS = 8;
const effUINT ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;

const effUINT MINIMUM_FREE_INDICES = 1024;

class EFFBASE_API Entity
{
public:
    Entity() { id = 0; }
    ~Entity() {}
public:
    inline effUINT index() const { return id & ENTITY_INDEX_MASK; }
    inline effUINT generation() const { return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }
public:
    effUINT id;
};

class EFFBASE_API EntityManager
{
public:
    EntityManager();
    ~EntityManager();
public:
    Entity Create();
    
    inline effBOOL Alive(Entity e) const
    {
        return generation[e.index()] == e.generation();
    }

    effVOID Destroy(Entity e);


private:
    VECTOR<effBYTE> generation;
    DEQUE<effUINT> free_indices;
};

EFFBASE_END


#endif