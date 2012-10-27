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

	effString			GetName() { return name; }
	effVOID				SetName(const effString & name) { this->name = name; }

	EFFProperty *		GetProperty(const effString & name);
protected:
	effUINT				id;
	effString			name;
};



EFFBASE_END

#endif

