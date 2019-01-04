/******************************************************************************
	created:	2008-12-10   0:48
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFObject.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFObject_2008_12_10__
#define __EFFObject_2008_12_10__



EFFBASE_BEGIN

class EFFProperty;
class EFFComponent;

class EFFBASE_API EFFReferenceCount
{
	RTTI_DECLARE_BASE(EFFReferenceCount)
public:
	virtual	~EFFReferenceCount() {}
protected:
	EFFReferenceCount() { refCount = 0; }

public:
	effUINT			GetRef() { return refCount; }
	effUINT			AddRef();
	effVOID			Release();
protected:
	effUINT			refCount;
};



class EFFBASE_API EFFObjectBase : public EFFReferenceCount
{
	friend effVOID EFFBaseInitProperty();
	RTTI_DECLARE_CUSTOM_SAVE(EFFObjectBase, EFFReferenceCount)
public:
	virtual ~EFFObjectBase();
public:
	effUINT				GetID() { return id; }
	EFFProperty *		GetProperty(const effString & name);

	template<typename PropertyType>
	effBOOL	GetPropertyValue(const effString & propertyName, PropertyType & result)
	{
		EFFProperty * Property = GetProperty(propertyName);

		if ( Property == NULL )
		{
			return effFALSE;
		}

		return Property->GetValue(this, result);
	}

	effVOID				GetPropertyJason(const effString & propertyName, effString & jasonString);
protected:
	EFFObjectBase();

	effVOID				AddPropertyToJson(const effString & propertyName, Json::Value * jsonValue);
	effVOID				SetID(effUINT id) { this->id = id; }
protected:
	effUINT				id;
};

class EFFBASE_API EFFObject : public EFFObjectBase
{
	friend effVOID EFFBaseInitProperty();
	friend class EFFObjectManager;
	RTTI_DECLARE_CUSTOM_SAVE(EFFObject, EFFObjectBase)
public:
	virtual ~EFFObject();
public:
	virtual effVOID		SaveComponentsToFile(EFFFile * file, effBOOL isBinary);
public:
	effBOOL				AddChild(EFFObject * object);
	effBOOL				RemoveChild(EFFObject * object);
	EFFObject *			GetChild(effUINT index);
	effSIZE				GetChildrenCount() { return children.size(); }
	EFFObject *			GetParent() { return parent; }

	EFFComponent *		AddComponent(EFFComponent * component);
	EFFComponent *		AddComponent(const ClassID & classID);
	EFFComponent *		GetComponent(const ClassID & classID);
	EFFComponent *		GetComponent(effINT index);
	effSIZE				GetComponentCount() { return components.size(); }


protected:
	EFFObject();
	effVOID				DetachFromTree();
	//����object tree�ҵ�������Ҫ�����objects��property�в���pod���͵�properties��property�������pod��Ҫ��EFFComponent����
	effVOID				TraverseObjectTree(VECTOR<EFFObject *> & objects, VECTOR<EFFComponent *> & properties);
protected:
	VECTOR<EFFObject *>			children;
	EFFObject *					parent;
	VECTOR<EFFComponent *>		components;
};

effVOID EFFBaseInitProperty();

EFFBASE_END

#endif

