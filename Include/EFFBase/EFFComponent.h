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
#include "EFFEntity.h"

EFFBASE_BEGIN


class EFFProperty;


class EFFBASE_API EFFComponent
{
    RTTI_DECLARE_BASE_CUSTOM_SAVE(EFFComponent)
public:
	virtual ~EFFComponent() {}
public:
	inline effBOOL		GetEnabled() { return enabled; }
	inline effVOID		SetEnabled(effBOOL enabled) { this->enabled = enabled; }

	inline EFFEntity    GetObject() { return object; }
	inline effVOID		SetObject(EFFEntity object) { this->object = object; }

    inline effUINT      GetID() { return id; }
    inline effVOID      SetID(effUINT id) { this->id = id; }
protected:
	EFFComponent();

protected:
	effBOOL			enabled;
    EFFEntity       object;
    effUINT         id;
};




template<typename C>
class EFFComponentManager
{
public:
    EFFComponentManager() {}
    virtual ~EFFComponentManager() {}


public:
    effVOID AddComponent(EFFEntity entity)
    {
        C c;
        datas.push_back(c);

        indices[entity] = (effUINT)(datas.size() - 1);
    }

    inline effUINT GetComponent(EFFEntity entity)
    {
        return indices[entity];
    }

protected:
    //只有声明为友元的manager才能调用这个函数
    inline C * GetComponentPoint(effUINT index)
    {
        return &datas[index];
    }

private:
    MAP<EFFEntity, effUINT> indices;
    VECTOR<C> datas;
};

/*template<typename C>
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
};*/

class EFFTreeNode
{
public:
    EFFTreeNode()
    {
        firstChild = -1;
        lastChild = -1;
        //preSibling = -1;
        //nextSibling = -1;
    }

    ~EFFTreeNode() {}

public:
    EFFEntity parent;
    EFFEntity entity;
    effUINT firstChild;
    effUINT lastChild;
    //effUINT preSibling;
    //effUINT nextSibling;
};

#define TreeComponent_MaxDepth 8

const effUINT COMPONENT_INDEX_BITS = 24;
const effUINT COMPONENT_INDEX_MASK = (1 << COMPONENT_INDEX_BITS) - 1;


const effUINT COMPONENT_DEPTH_BITS = 3;
const effUINT COMPONENT_DEPTH_MASK = (1 << COMPONENT_DEPTH_BITS) - 1;


template<typename C>
class EFFTreeComponentManager
{
public:
    EFFTreeComponentManager()
    {
    }

    virtual ~EFFTreeComponentManager() {}

public:
    effVOID AddComponent(EFFEntity entity, EFFEntity parentEntity)
    {
        if (parentEntity.IsValid())
        {
            effUINT parentDepth = Depth(parentEntity);

            effUINT depth = parentDepth + 1;



            C * parentComponent = GetComponentPoint(parentEntity);
            EFFTreeNode * parentTreeNode = (EFFTreeNode *)&parentComponent;

            effUINT index = parentTreeNode->lastChild + 1;

            C component;
            AddNode(index, (EFFTreeNode *)&component, parentTreeNode, parentDepth);


            datas[depth].insert(datas[depth].begin() + index, C);

            indices[entity] = index;
        }
        else
        {
            C component;
            AddNode((effUINT)datas[0].size(), (EFFTreeNode *)&component, NULL, 0);

            datas[0].push_back(C);
            indices[entity] = (effUINT)datas[0].size();
        }
    }

    inline effUINT GetComponent(EFFEntity entity)
    {
        return indices[entity];
    }

    template<typename CM>
    effVOID ForEach(EFFEntity entity, CM * manager, boost::function<effVOID (effVOID *, effUINT, effVOID *)> function, effVOID * userData)
    {
        effUINT topComponentStart;
        effUINT topComponentEnd;
        effUINT depth;
        
        if (!entity.IsValid())
        {
            depth = 0;
            topComponentStart = 0;
            topComponentEnd = (effUINT)datas[0].size() - 1;
        }
        else
        {
            depth = Depth(entity);
            topComponentStart = Index(entity);
            topComponentEnd = topComponentStart;
        }

        //遍历所有的top component
        for (effUINT i = topComponentStart; i <= topComponentEnd; i++)
        {
            C * component = &datas[depth][i];

            effUINT firstChild = component->treeNode.firstChild;
            effUINT lastChild = component->treeNode.lastChild;



            //遍历树的1-最下层
            for (effUINT j = depth + 1; j < TreeComponent_MaxDepth; j++)
            {
                if (firstChild == -1)
                {
                    continue;
                }


                C * firstChildComponent = &datas[j][firstChild];
                C * lastChildComponent = &datas[j][lastChild];

                effUINT c2FirstChild = manager->GetComponent(firstChildComponent->treeNode.entity);
                effUINT c2LastChild = manager->GetComponent(lastChildComponent->treeNode.entity);

                if (c2FirstChild != -1)
                {
                    for (effUINT k = c2FirstChild; k <= c2LastChild; k++)
                    {
                        function(manager, k, userData);
                    }
                }

                firstChild = firstChildComponent->treeNode.firstChild;
                lastChild = lastChildComponent->treeNode.lastChild;
            }
        }
    }

protected:
    inline effUINT Index(EFFEntity entity)
    {
        return indices[entity] & COMPONENT_INDEX_MASK;
    }

    inline effUINT Depth(EFFEntity entity)
    {
        return (indices[entity] >> COMPONENT_INDEX_BITS) & COMPONENT_DEPTH_MASK;
    }

    inline C * GetComponentPoint(EFFEntity entity)
    {
        effUINT index = Index(entity);
        effUINT depth = Depth(entity);


        return &datas[depth][index];
    }

    effVOID AddNode(effUINT index, EFFTreeNode * node, EFFTreeNode * root, effUINT rootDepth)
    {
        if (root != NULL && root->firstChild == -1)
        {
            root->firstChild = index;

            //node->preSibling = index;
            //node->nextSibling = index;
        }
        else
        {
            //EFFTreeNode * firstChildNode = (EFFTreeNode *)&datas[rootDepth + 1][root->firstChild];
            //firstChildNode->preSibling = index;
            //node->nextSibling = root->firstChild;
        }


        if (root != NULL)
        {
            root->lastChild = index;
        }

        /*if (root->lastChild == -1)
        {
            root->lastChild = index;
        }
        else
        {
            EFFTreeNode * lastChildNode = (EFFTreeNode *)&datas[rootDepth + 1][root->lastChild];
            lastChildNode->nextSibling = index;

            node->preSibling = root->lastChild;
            root->lastChild = index;
        }*/
    }

protected:
    MAP<EFFEntity, effUINT> indices;
    //MAP<EFFEntity, effUINT> indices[TreeComponent_MaxDepth];
    //EFFFastIdMap<C> indices;
    VECTOR<C> datas[TreeComponent_MaxDepth];

    //EFFTreeNode root;
};

EFFBASE_END

#endif