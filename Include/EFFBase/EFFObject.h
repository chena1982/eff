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



class EFFBASE_API EFFObject : public EFFReferenceCount
{
	friend class EFFObjectManager;
	RTTI_DECLARE(EFFObject, EFFReferenceCount)
public:
	virtual ~EFFObject();


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

	effBOOL				AddChild(EFFObject * object);
	effBOOL				RemoveChild(EFFObject * object);
	EFFObject *			GetChild(effUINT index);
	effUINT				GetChildrenCount() { return children.size(); }
	EFFObject *			GetParent() { return parent; }

	effVOID				GetPropertyJason(const effString & propertyName, effString & jasonString);
protected:
	EFFObject();

	effVOID				AddPropertyToJson(const effString & propertyName, Json::Value * jsonValue);
	effVOID				SetID(effUINT id) { this->id = id; }
	effVOID				DetachFromTree();

protected:
	effUINT						id;
	std::vector<EFFObject *>	children;
	EFFObject *					parent;
};



EFFBASE_END

#endif

