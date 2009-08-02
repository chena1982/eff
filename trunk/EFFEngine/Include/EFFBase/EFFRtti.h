/******************************************************************************
	created:	2008-12-1   22:32
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFRtti.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFRtti_2008_12_1__
#define __EFFRtti_2008_12_1__

#include "EFFReflection.h"


#define new EFFNEW

EFFBASE_BEGIN

struct EFFBASE_API ClassID
{
	unsigned __int64 classId[2];

	bool operator == (const ClassID & c) const
	{
		return classId[0] == c.classId[0] && classId[1] == c.classId[1];
	}

	bool operator < (const ClassID & c) const
	{
		return classId[0] < c.classId[0];
	}
};

ClassID EFFBASE_API ClassIDFromString(const char *string);


#define RTTI_CLASS(T)	((EFFClass *)(&T::runtimeInfoClass##T))
#define RTTI_CLASSID(T)	((EFFClass *)(&T::runtimeInfoClass##T))->GetID()



#define	RTTI_DECLARE(CLASS,BASECLASS)\
public:\
	typedef BASECLASS	classBase;\
	typedef CLASS	classThis;\
	friend EFFClassImpl<CLASS>;\
	static EFFClassImpl<CLASS>	runtimeInfoClass##CLASS;\
	static EFFClass * GetThisClass();\
	virtual EFFClass * __stdcall GetRuntimeClass() const;\
	virtual void SaveToFile(EFFFile * pFile);\
protected:\
	template<class T>\
	inline void CLASS##Visit(T * pArg)\

#define	RTTI_DECLARE_PURE(CLASS,BASECLASS)\
public:\
	typedef BASECLASS classBase;\
	friend EFFRunTimeTypeInfoImplPureVirtual<CLASS>;\
	static EFFRunTimeTypeInfoImplPureVirtual<CLASS> runtimeInfoClass##CLASS;\
	static EFFClass* GetThisClass();\
	virtual EFFClass * __stdcall GetRuntimeClass() const;


#define RTTI_IMPLEMENT_NAME(CLASS,VERSION,NAME)\
	EFFClassImpl<CLASS> CLASS::runtimeInfoClass##CLASS(VERSION,NAME,CLASS::classBase::GetThisClass());\
	class CLASS##RegisterProperty\
	{\
	public:\
		CLASS##RegisterProperty()\
		{\
			CLASS::RegisterProperty();\
		}\
	};\
	static CLASS##RegisterProperty _##CLASS##RegisterProperty;\
	class CLASS##RegisterMethod\
	{\
		public:\
		CLASS##RegisterMethod()\
		{\
			CLASS::RegisterMethod();\
		}\
	};\
	static CLASS##RegisterMethod _##CLASS##RegisterMethod;\
	EFFClass* CLASS##::GetThisClass()\
	{\
		return RTTI_CLASS(CLASS);\
	}\
	EFFClass* __stdcall CLASS##::GetRuntimeClass() const\
	{\
		return GetThisClass();\
	}\
	void CLASS##::SaveToFile(EFFFile * pFile)\
	{\
		if ( pFile == NULL )\
		{\
			return;\
		}\
		ArgWriteBin awb;\
		awb.pFile = pFile;\
		CLASS##Visit(&awb);\
	}

#define RTTI_IMPLEMENT_PURE_NAME(CLASS,VERSION,NAME) \
	EFFRunTimeTypeInfoImplPureVirtual<CLASS> CLASS::runtimeInfoClass##CLASS = EFFRunTimeTypeInfoImplPureVirtual<CLASS>(VERSION,NAME,CLASS::classBase::GetThisClass());\
	EFFClass * CLASS::GetThisClass()\
	{\
		return RTTI_CLASS(CLASS);\
	}\
	EFFClass * __stdcall CLASS::GetRuntimeClass() const\
	{\
		return GetThisClass();\
	}

#define RTTI_IMPLEMENT(CLASS,VERSION)				RTTI_IMPLEMENT_NAME(CLASS,VERSION,#CLASS)
#define RTTI_IMPLEMENT_PURE(CLASS,VERSION)		RTTI_IMPLEMENT_PURE_NAME(CLASS,VERSION,#CLASS)



class EFFClass;

void EFFRegisterClass(EFFClass * pClass);

void EFFUnRegisterClass(EFFClass * pClass);

EFFBASE_API void * EFFCreateObject(const char * pszClassName);
EFFBASE_API void * EFFCreateObject(const ClassID & classID);

class EFFBASE_API EFFClass
{

public:
	EFFClass * GetBaseClass() const 
	{
		return m_pBaseClass;
	}

	effLPCSTR GetClassName()
	{
		return m_strClassName.c_str();
	}

	effUINT GetVersion() const
	{
		return m_uiVersion;
	}

	const ClassID & GetID() const
	{
		return m_Id; 
	}
	
	friend bool operator == (const EFFClass & crt1,const EFFClass & crt2)
	{
		return crt1.m_Id == crt2.m_Id && crt1.m_uiVersion == crt2.m_uiVersion;
	}

	bool IsKindOf(const EFFClass * pClass)
	{
		EFFClass * p = this;
		while(p)
		{
			if( (*p) == (*pClass) )
			{
				return true;
			}
			p = p->GetBaseClass();
		}
		return false;
	}

	bool IsKindOf(ClassID & m_Id)
	{
		EFFClass * p = this;
		while( p )
		{
			if( p->m_Id == m_Id )
			{
				return true;
			}
			p = p->GetBaseClass();
		}
		return false;
	}

	effVOID AddMemberMethod(__callable__ * pMethod)
	{
		m_vMemberMethod.push_back(pMethod);
	}

	effVOID AddProperty(__property__ & property)
	{
		m_vProperty.push_back(property);
	}

public:
	EFFClass(effUINT uiVersion,effLPCSTR pName,EFFClass * pBaseClass)
		:m_uiVersion(uiVersion),m_strClassName(pName),m_pBaseClass(pBaseClass)
	{
		m_Id = ClassIDFromString(const_cast<char *>(pName));
		EFFRegisterClass(this);
	}
	EFFClass(const EFFClass & rhs)
		:m_Id(rhs.m_Id),m_uiVersion(rhs.m_uiVersion),m_strClassName(rhs.m_strClassName),m_pBaseClass(rhs.m_pBaseClass)
	{
		EFFRegisterClass(this);
	}

	EFFClass & operator = (const EFFClass & rhs)
	{
		m_Id = rhs.m_Id;
		m_uiVersion = rhs.m_uiVersion;
		m_strClassName = rhs.m_strClassName;
		m_pBaseClass = rhs.m_pBaseClass;
		return *this;
	}

	~EFFClass()
	{
		EFFUnRegisterClass(this);
	}
	//friend class __ucCompareRTTI;
	//friend class __ucCompareRTTIAndClassID;

	virtual effVOID * CreateObject() = 0;
protected:
	ClassID										m_Id;
	effUINT									m_uiVersion;
	std::string									m_strClassName;
	EFFClass *									m_pBaseClass;

	std::vector<__callable__ *>			m_vMemberMethod;
	std::vector<__property__ >		m_vProperty;
};


template<class T>
class EFFClassImpl : public EFFClass
{
public:
	virtual effVOID * CreateObject()
	{
		return new T;
	}

	EFFClassImpl(effUINT uiVersion,effLPCSTR pszName,EFFClass * pBaseClass) : EFFClass(uiVersion,pszName,pBaseClass) { }
	
	//EFFClassImpl() : EFFClass(0,0,0,NULL) { }

	EFFClassImpl(const EFFClassImpl<T> & rhs) : EFFClass(rhs) { }
	
	EFFClassImpl<T> & operator = (const EFFClassImpl<T> & rhs)
	{
		EFFClass::operator = (rhs);
		return *this;
	}
};


template<class T>
class EFFRunTimeTypeInfoImplPureVirtual : public EFFClass
{
public:
	virtual effVOID * CreateObject() { return NULL; }

	EFFRunTimeTypeInfoImplPureVirtual(effUINT uiVersion,effLPCSTR pszName,EFFClass * pBaseClass) : EFFClass(uiVersion,pszName,pBaseClass) {}
	
	//EFFRunTimeTypeInfoImplPureVirtual() : EFFClass(0,NULL,NULL) {}

	EFFRunTimeTypeInfoImplPureVirtual(const EFFRunTimeTypeInfoImplPureVirtual<T> & rhs) : EFFClass(rhs) {}

	EFFRunTimeTypeInfoImplPureVirtual<T> & operator = (const EFFRunTimeTypeInfoImplPureVirtual<T> & rhs)
	{
		EFFClass::operator = (rhs);
		return *this;
	}
};


EFFBASE_END


#endif