#pragma once


#include "EFFReflection.h"

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

ClassID EFFBASE_API ClassIDFromString(char *string);


#define RTTI_CLASS(T)	((EFFClass *)(&T::runtimeInfoClass##T))
#define RTTI_CLASSID(T)	((EFFClass *)(&T::runtimeInfoClass##T))->GetID()

#define	RTTI_DECLARE(Class,BaseClass)\
public:\
	typedef BaseClass	classBase;\
	friend EFFClassImpl<Class>;\
	static EFFClassImpl<Class>	runtimeInfoClass##Class;\
	static EFFClass * GetThisClass();\
	virtual EFFClass * GetRuntimeClass() const;\
	virtual void SaveToFile(EFFFile * pFile)

#define RTTI_IMPLEMENT_NAME(Class,version,name)\
	EFFClassImpl<Class> Class::runtimeInfoClass##Class = EFFClassImpl<Class>(version,name,Class::classBase::GetThisClass());\
	EFFClass* Class::GetThisClass()\
	{\
		return RTTI_CLASS(Class);\
	}\
	EFFClass* Class::GetRuntimeClass() const\
	{\
		return GetThisClass();\
	}

#define RTTI_IMPLEMENT(Class,version)		RTTI_IMPLEMENT_NAME(Class,version,#Class)

class EFFClass;

void EFFRegisterClass(EFFClass * pClass);

void EFFUnRegisterClass(EFFClass * pClass);


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

	unsigned int GetVersion() const
	{
		return m_dwVersion;
	}

	ClassID	GetID() const
	{
		return m_Id; 
	}
	
	friend bool operator == (const EFFClass & crt1,const EFFClass & crt2)
	{
		return crt1.m_Id == crt2.m_Id && crt1.m_dwVersion == crt2.m_dwVersion;
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

	bool IsKindOf(ClassID m_Id)
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

	void AddMemberMethod(__callable__ * pMethod)
	{
		m_vMemberMethod.push_back(pMethod);
	}

	void AddProperty(__property__ & property)
	{
		m_vProperty.push_back(property);
	}

public:
	EFFClass(unsigned int dwVersion,effLPCSTR pName,EFFClass * pBaseClass)
		:m_dwVersion(dwVersion),m_strClassName(pName),m_pBaseClass(pBaseClass)
	{
		m_Id = ClassIDFromString(const_cast<char *>(pName));
		EFFRegisterClass(this);
	}
	EFFClass(const EFFClass & rhs)
		:m_Id(rhs.m_Id),m_dwVersion(rhs.m_dwVersion),m_strClassName(rhs.m_strClassName),m_pBaseClass(rhs.m_pBaseClass)
	{
		EFFRegisterClass(this);
	}

	EFFClass & operator = (const EFFClass & rhs)
	{
		m_Id = rhs.m_Id;
		m_dwVersion = rhs.m_dwVersion;
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

protected:
	ClassID										m_Id;
	unsigned int								m_dwVersion;
	std::string									m_strClassName;
	EFFClass *									m_pBaseClass;

	std::vector<__callable__ *>			m_vMemberMethod;
	std::vector<__property__ >		m_vProperty;
};


template<class T>
class EFFClassImpl : public EFFClass
{
public:
	virtual void * CreateObject() const
	{
#if _DEBUG
		return new(__FILE__,__LINE__)T;
#else
		return new T;
#endif
	}

	EFFClassImpl(unsigned int dwVersion,effLPCSTR pName,EFFClass * pBaseClass)
		: EFFClass(dwVersion,pName,pBaseClass) { }
	
	//EFFClassImpl() : EFFClass(0,0,0,NULL) { }

	EFFClassImpl(const EFFClassImpl<T> & rhs)
		:EFFClass(rhs) { }
	
	EFFClassImpl<T> & operator = (const EFFClassImpl<T> & rhs)
	{
		EFFClass::operator = (rhs);
		return *this;
	}
};

