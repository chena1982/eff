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

protected:
	EFFObject();
	void				SetID(effUINT id) { this->id = id; }
public:
	effUINT				GetID() { return id; }
	EFFProperty *		GetProperty(const effString & name);

	template<typename PropertyType>
	effBOOL	GetPropertyValue(const effString & name, PropertyType & result)
	{
		EFFProperty * Property = GetProperty(name);

		if ( Property == NULL )
		{
			return effFALSE;
		}

		return Property->GetValue(this, result);
	}
protected:
	effUINT				id;
};



EFFBASE_END

#endif

