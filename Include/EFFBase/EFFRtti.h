/******************************************************************************
	created:	2008-12-1   22:32
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFRtti.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFRtti_2008_12_1__
#define __EFFRtti_2008_12_1__

#include <boost\type_traits.hpp>

#include "EFFReflection.h"
#include "EFFProperty.h"
#include "EFFSTLFile.h"
#include "EFFUtility.h"
#include "YAMLWrap.h"

#include <iostream>

EFFBASE_BEGIN






#define RTTI_CLASS(T)	((EFFClass *)(&T::runtimeInfoClass##T))
#define RTTI_CLASSID(T)	((EFFClass *)(&T::runtimeInfoClass##T))->GetID()

#define RTTI_DECLARE_BASE(CLASS)\
public:\
	typedef CLASS classThis;\
	friend EFFClassImpl<CLASS>;\
	static EFFClassImpl<CLASS> runtimeInfoClass##CLASS;\
	static EFFClass * GetThisClass();\
	virtual EFFClass * GetRuntimeClass() const;\

#define RTTI_DECLARE_BASE_CUSTOM_SAVE(CLASS)\
	RTTI_DECLARE_BASE(CLASS)\
	virtual effVOID SaveToFile(EFFFile * file, effBOOL isBinary);\
	virtual effVOID SaveToFile(const effString & filePath, effBOOL isBinary);\

#define	RTTI_DECLARE(CLASS, BASECLASS)\
	RTTI_DECLARE_BASE(CLASS)\
	typedef BASECLASS classBase;

#define RTTI_DECLARE_CUSTOM_SAVE(CLASS, BASECLASS)\
	RTTI_DECLARE_BASE(CLASS)\
	virtual effVOID SaveToFile(EFFFile * file, effBOOL isBinary);\
	virtual effVOID SaveToFile(const effString & filePath, effBOOL isBinary);\
	typedef BASECLASS classBase;

#define	RTTI_DECLARE_BASE_PURE(CLASS)\
public:\
	typedef CLASS classThis;\
	friend EFFPureVirtualClassImpl<CLASS>;\
	static EFFPureVirtualClassImpl<CLASS> runtimeInfoClass##CLASS;\
	static EFFClass * GetThisClass();\
	virtual EFFClass * GetRuntimeClass() const;

#define	RTTI_DECLARE_PURE(CLASS, BASECLASS)\
	RTTI_DECLARE_BASE_PURE(CLASS)\
	typedef BASECLASS classBase;\

#define RTTI_DECLARE_POD(CLASS)\
	class CLASS##POD\
	{\
	public:\
		typedef CLASS##POD classThis;\
		friend EFFClassImpl<CLASS>;\
		static EFFClassImpl<CLASS> runtimeInfoClass##CLASS##POD;\
		static EFFClass * GetThisClass();\
		virtual EFFClass * GetRuntimeClass() const;\
	};

#define RTTI_DECLARE_PURE_POD(CLASS)\
	class CLASS##POD\
	{\
	public:\
		typedef CLASS##POD classThis;\
		friend EFFPureVirtualClassImpl<CLASS>;\
		static EFFPureVirtualClassImpl<CLASS> runtimeInfoClass##CLASS##POD;\
		static EFFClass * GetThisClass();\
		virtual EFFClass * GetRuntimeClass() const;\
	};



#define RTTI_IMPLEMENT_INTERNAL(CLASS, VERSION, NAME, BASECLASS)\
	EFFClassImpl<CLASS> CLASS::runtimeInfoClass##CLASS(VERSION, effFALSE, NAME, BASECLASS);\
	EFFClass * CLASS##::GetThisClass()\
	{\
		return RTTI_CLASS(CLASS);\
	}\
	EFFClass * CLASS##::GetRuntimeClass() const\
	{\
		return GetThisClass();\
	}\


#define RTTI_IMPLEMENT_CUSTOM_SAVE_INTERNAL(CLASS, VERSION, NAME, BASECLASS)\
	EFFClassImpl<CLASS> CLASS::runtimeInfoClass##CLASS(VERSION, effFALSE, NAME, BASECLASS);\
	EFFClass * CLASS##::GetThisClass()\
	{\
		return RTTI_CLASS(CLASS);\
	}\
	EFFClass * CLASS##::GetRuntimeClass() const\
	{\
		return GetThisClass();\
	}\


#define RTTI_IMPLEMENT_NO_SAVE_INTERNAL(CLASS, VERSION, NAME, BASECLASS)\
	RTTI_IMPLEMENT_CUSTOM_SAVE_INTERNAL(CLASS, VERSION, NAME, BASECLASS)

#define RTTI_IMPLEMENT_PURE_INTERNAL(CLASS, VERSION, NAME, BASECLASS) \
	EFFPureVirtualClassImpl<CLASS> CLASS::runtimeInfoClass##CLASS(VERSION, effFALSE, NAME, BASECLASS);\
	EFFClass * CLASS::GetThisClass()\
	{\
		return RTTI_CLASS(CLASS);\
	}\
	EFFClass * CLASS::GetRuntimeClass() const\
	{\
		return GetThisClass();\
	}

#define RTTI_IMPLEMENT_POD_INTERNAL(CLASS, VERSION, NAME, BASECLASS)\
	EFFClassImpl<CLASS> CLASS##POD::runtimeInfoClass##CLASS##POD(VERSION, effTRUE, NAME, BASECLASS);\
	EFFClass * CLASS##POD##::GetThisClass()\
	{\
		return RTTI_CLASS(CLASS##POD);\
	}\
	EFFClass * CLASS##POD##::GetRuntimeClass() const\
	{\
		return GetThisClass();\
	}

#define RTTI_IMPLEMENT_PURE_POD_INTERNAL(CLASS, VERSION, NAME, BASECLASS)\
	EFFPureVirtualClassImpl<CLASS> CLASS##POD::runtimeInfoClass##CLASS##POD(VERSION, effTRUE, NAME, BASECLASS);\
	EFFClass * CLASS##POD##::GetThisClass()\
	{\
		return RTTI_CLASS(CLASS##POD);\
	}\
	EFFClass * CLASS##POD##::GetRuntimeClass() const\
	{\
		return GetThisClass();\
	}

#define RTTI_IMPLEMENT_BASE(CLASS, VERSION)				RTTI_IMPLEMENT_INTERNAL(CLASS, VERSION, _effT(#CLASS), NULL)
#define RTTI_IMPLEMENT_BASE_CUSTOM_SAVE(CLASS, VERSION)	RTTI_IMPLEMENT_CUSTOM_SAVE_INTERNAL(CLASS, VERSION, _effT(#CLASS), NULL)
#define RTTI_IMPLEMENT_BASE_NO_SAVE(CLASS, VERSION)		RTTI_IMPLEMENT_NO_SAVE_INTERNAL(CLASS, VERSION, _effT(#CLASS), NULL)
#define RTTI_IMPLEMENT_BASE_PURE(CLASS, VERSION)		RTTI_IMPLEMENT_PURE_INTERNAL(CLASS, VERSION, _effT(#CLASS), NULL)

#define RTTI_IMPLEMENT(CLASS, VERSION)					RTTI_IMPLEMENT_INTERNAL(CLASS, VERSION, _effT(#CLASS), CLASS::classBase::GetThisClass())
#define RTTI_IMPLEMENT_CUSTOM_SAVE(CLASS, VERSION)		RTTI_IMPLEMENT_CUSTOM_SAVE_INTERNAL(CLASS, VERSION, _effT(#CLASS), CLASS::classBase::GetThisClass())
#define RTTI_IMPLEMENT_NO_SAVE(CLASS, VERSION)			RTTI_IMPLEMENT_NO_SAVE_INTERNAL(CLASS, VERSION, _effT(#CLASS), CLASS::classBase::GetThisClass())

#define RTTI_IMPLEMENT_PURE(CLASS, VERSION)				RTTI_IMPLEMENT_PURE_INTERNAL(CLASS, VERSION, _effT(#CLASS), CLASS::classBase::GetThisClass())

#define RTTI_IMPLEMENT_POD(CLASS)						RTTI_IMPLEMENT_POD_INTERNAL(CLASS, 0, _effT(#CLASS), NULL)
#define RTTI_IMPLEMENT_PURE_POD(CLASS)					RTTI_IMPLEMENT_PURE_POD_INTERNAL(CLASS, 0, _effT(#CLASS), NULL)









class RegisterProperty
{

public:
	/*RegisterProperty(const effString & podPropertyTypeName, effULONG offset, effULONG size, const effString & name)
	{
		classType::GetThisClass()->AddProperty(member, name, podPropertyTypeName);
	}*/

	RegisterProperty(const effString & podPropertyTypeName, EFFClass * Class, effULONG offset, effULONG size, const effString & name)
	{
		EFFClass * propertyClass = EFFGetClass(podPropertyTypeName);
		//Class->addProperty(offset, size, name, propertyClass);
	}

};

#define RTTI_PROPERTY(CLASS, MEMBER, NAME)\
	static RegisterProperty registerProperty##CLASS##MEMBER(EFFClass::GetClassName(&CLASS::MEMBER), CLASS::GetThisClass(), MEM_OFFSET(CLASS, MEMBER), MEM_SIZE(CLASS, MEMBER), NAME);

RTTI_DECLARE_POD(effString)
RTTI_DECLARE_POD(effINT)
RTTI_DECLARE_POD(effUINT)
RTTI_DECLARE_POD(effFLOAT)
RTTI_DECLARE_POD(effBOOL)
RTTI_DECLARE_PURE_POD(effVOID)



template<typename PropertyType>
class GetParameterProperty
{
public:
	GetParameterProperty(const effString & name)
	{
		parameterProperty = EFFNEW EFFPropertyImpl<PropertyType, EFF_IS_POD<PropertyType, boost::is_pod<boost::remove_pointer<PropertyType>>::type>::type>();
		SetProperty(parameterProperty, 0, sizeof(PropertyType), name);
	}
	EFFProperty * parameterProperty;
};

template<typename PropertyType>
class GetParameterProperty<typename PropertyType *>
{
public:
	GetParameterProperty(const effString & name)
	{
		parameterProperty = EFFNEW EFFPropertyImpl<PropertyType, EFF_IS_POD<PropertyType, boost::is_pod<PropertyType>::type>::type>();
		SetProperty(parameterProperty, 0, sizeof(PropertyType), name);
	}
	EFFProperty * parameterProperty;
};

template<>
class GetParameterProperty<effVOID>
{
public:
	GetParameterProperty(const effString & name)
	{
		parameterProperty = EFFNEW EFFPropertyImpl<effVOID, EFF_IS_POD<effVOID, boost::true_type>::type>();
		SetProperty(parameterProperty, 0, 0, name);
	}
	EFFProperty * parameterProperty;
};




template<typename PropertyType>
class GetParameterProperty<VECTOR<PropertyType>>
{
public:
	GetParameterProperty(const effString & name)
	{
		parameterProperty = EFFNEW EFFPropertyImpl<VECTOR<PropertyType>, boost::is_pod<boost::remove_pointer<PropertyType>>::type>::type>();
		SetProperty(parameterProperty, 0, sizeof(PropertyType), name);
	}

	EFFProperty * parameterProperty;
};

template<typename PropertyType>
class GetParameterProperty<VECTOR<PropertyType *>>
{
public:
	GetParameterProperty(const effString & name)
	{
		parameterProperty = EFFNEW EFFPropertyImpl<VECTOR<PropertyType *>, EFF_IS_POD<PropertyType, boost::is_pod<boost::remove_pointer<PropertyType>>::type>::type>();
		SetProperty(parameterProperty, 0, sizeof(PropertyType), name);
	}

	EFFProperty * parameterProperty;
};

EFFBASE_END


#endif