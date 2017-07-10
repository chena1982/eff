/******************************************************************************
	created:	2008-12-1   23:33
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFComponent.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFComponent_2008_12_1__
#define __EFFComponent_2008_12_1__


#include "EFFObject.h"
#include "EFFFastIdMap.h"

EFFBASE_BEGIN

class EFFEntity;
class EFFProperty;


class EFFBASE_API EFFComponent : public EFFObjectBase
{
	RTTI_DECLARE_CUSTOM_SAVE(EFFComponent, EFFObjectBase)
public:
	virtual ~EFFComponent() {}
public:
	inline effBOOL		GetEnabled() { return enabled; }
	inline effVOID		SetEnabled(effBOOL enabled) { this->enabled = enabled; }

	inline EFFObject *	GetObject() { return object; }
	inline effVOID		SetObject(EFFObject * object) { this->object = object; }
protected:
	EFFComponent();

protected:
	effBOOL			enabled;
	EFFObject *		object;
};

template<typename C>
class EFFBASE_API EFFTreeNode
{
public:
    EFFTreeNode()
    {
        parent = NULL;
        firstChild = NULL;
        nextSibling = NULL;
        preSibling = NULL;
        depth = 0;
    }

    ~EFFTreeNode() {}
public:
    C * parent;
    C * firstChild;
    C * nextSibling;
    C * preSibling;
    effUINT depth;
};

#define TreeComponent_MaxDepth 8

template<class C>
class EFFBASE_API EFFTreeComponentManager
{
public:
    EFFTreeComponentManager() {}
    virtual ~EFFTreeComponentManager() {}

public:
    effVOID AddComponent(EFFEntity * parent)
    {
        if (parent != NULL)
        {
            effUINT parentDepth = GetDepth(entity);

            C component;
            component.

            datas[parentDepth].push_back(C());

            datas.insert((C *)components[parent->index()]);
        }
    }


    virtual C * GetParent(EFFEntity entity) = 0;
    virtual C * GetFirstChild(EFFEntity entity) = 0;
    virtual C * GetLastChild(EFFEntity entity) = 0;
    virtual effUINT GetDepth(EFFEntity entity) = 0;
    virtual effUINT SetDepth(EFFEntity entity, effUINT depth) = 0;

protected:
    EFFFastIdMap<C> components;
    VECTOR<C> datas[TreeComponent_MaxDepth];
};

EFFBASE_END

#endif