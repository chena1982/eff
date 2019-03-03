/******************************************************************************
created:	2017-07-07   20:50
file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFEntity.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFEntity_2017_07_07__
#define __EFFEntity_2017_07_07__

#include "metareflect.hxx"

EFFBASE_BEGIN

const effUINT INDEX_BITS = 24;
const effUINT INDEX_MASK = (1 << INDEX_BITS) - 1;


const effUINT GENERATION_BITS = 8;
const effUINT GENERATION_MASK = (1 << GENERATION_BITS) - 1;

const effUINT MINIMUM_FREE_INDICES = 1024;

class EFFBASE_API EFFId
{
public:
    EFFId() { id = -1; }
    ~EFFId() {}
public:
    inline effBOOL IsValid() const { return id != -1;  }
    inline effUINT Index() const { return id & INDEX_MASK; }
    inline effUINT Generation() const { return (id >> INDEX_BITS) & GENERATION_MASK; }

    inline bool operator < (const EFFId & id) const
    {
        return Index() < id.Index();
    }

    inline bool operator != (const EFFId & id) const
    {
        return this->id != id.id;
    }

    inline bool operator == (const EFFId & id) const
    {
        return this->id == id.id;
    }
public:
    effUINT id;
};

class EFFBASE_API EFFEntity : public EFFId
{
public:
    EFFEntity() {};
    ~EFFEntity() {};
};

CLASS() EFFBASE_API EFFIdManager
{
	META_OBJECT()
public:
    EFFIdManager();
    virtual ~EFFIdManager();
public:
	FUNCTION()
    EFFId Create();
    
    inline effBOOL Alive(EFFId id) const
    {
        return generation[id.Index()] == id.Generation();
    }

	FUNCTION()
    effVOID Destroy(EFFId e);


private:
	PROPERTY()
    VECTOR<effBYTE> generation;
    DEQUE<effUINT> freeIndices;

	PROPERTY(Min=0)
	effUINT test;

	PROPERTY()
	effString name;
};


class EFFBASE_API EFFEntityManager : public EFFIdManager
{
public:
    EFFEntityManager() {};
    ~EFFEntityManager() {};

public:
    effVOID SaveBinary();
};

EFFBASE_END


#endif