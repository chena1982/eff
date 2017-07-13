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

class EFFBASE_API EFFEntity
{
public:
    EFFEntity() { id = -1; }
    ~EFFEntity() {}
public:
    inline effBOOL IsValid() { return id != -1;  }
    inline effUINT Index() const { return id & ENTITY_INDEX_MASK; }
    inline effUINT Generation() const { return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }

    inline bool operator < (const EFFEntity & entity) const
    {
        return Index() < entity.Index();
    }
public:
    effUINT id;
};

class EFFBASE_API EFFEntityManager
{
public:
    EFFEntityManager();
    ~EFFEntityManager();
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