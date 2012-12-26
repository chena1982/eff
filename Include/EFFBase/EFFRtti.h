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
	virtual effVOID SaveToFile(EFFFile * file, effBOOL isBinary, YAML::Emitter * textOut);\
	virtual effVOID SaveToFile(const effString & filePath, effBOOL isBinary);\
	virtual effVOID SaveComponents(EFFFile * file, effBOOL isBinary, YAML::Emitter * textOut) {};

#define	RTTI_DECLARE(CLASS, BASECLASS)\
	RTTI_DECLARE_BASE(CLASS)\
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
	effVOID CLASS##::SaveToFile(EFFFile * file, effBOOL isBinary, YAML::Emitter * textOut)\
	{\
		EFFClass * Class = GetRuntimeClass();\
		std::vector<EFFProperty *> & properties = Class->GetProperties();\
		for ( effUINT i = 0; i < properties.size(); i++ )\
		{\
			EFFProperty * curProperty = properties[i];\
			curProperty->SaveToFile(file, this, isBinary, textOut);\
		}\
		SaveComponents(file, isBinary, textOut);\
	}\
	effVOID CLASS##::SaveToFile(const effString & filePath, effBOOL isBinary)\
	{\
		EFFSTLFile file;\
		if ( isBinary )\
		{\
			if ( !file.Open(filePath, _effT("wb")) )\
			{\
				return;\
			}\
		}\
		else\
		{\
			if ( !file.Open(filePath, _effT("wt")) )\
			{\
				return;\
			}\
		}\
		if ( isBinary ) \
		{\
			SaveToFile(&file, isBinary, NULL);\
		}\
		else\
		{\
			YAML::Emitter textOut;\
			textOut << YAML::BeginMap;\
			SaveToFile(NULL, isBinary, &textOut);\
			textOut << YAML::EndMap;\
			file.Write((effVOID *)textOut.c_str(), strlen(textOut.c_str()));\
		}\
		file.Close();\
	}

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
	effVOID CLASS##::SaveToFile(const effString & filePath, effBOOL isBinary)\
	{\
		EFFSTLFile file;\
		if ( isBinary )\
		{\
			if ( !file.Open(filePath, _effT("wb")) )\
			{\
				return;\
			}\
		}\
		else\
		{\
			if ( !file.Open(filePath, _effT("wt")) )\
			{\
				return;\
			}\
		}\
		if ( isBinary ) \
		{\
			SaveToFile(&file, isBinary, NULL);\
		}\
		else\
		{\
			YAML::Emitter textOut;\
			textOut << YAML::BeginMap;\
			SaveToFile(NULL, isBinary, &textOut);\
			textOut << YAML::EndMap;\
			file.Write((effVOID *)textOut.c_str(), strlen(textOut.c_str()));\
		}\
		file.Close();\
	}


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
#define RTTI_IMPLEMENT_BASE_PURE(CLASS, VERSION)		RTTI_IMPLEMENT_PURE_INTERNAL(CLASS, VERSION, _effT(#CLASS), NULL)

#define RTTI_IMPLEMENT(CLASS, VERSION)					RTTI_IMPLEMENT_INTERNAL(CLASS, VERSION, _effT(#CLASS), CLASS::classBase::GetThisClass())
#define RTTI_IMPLEMENT_CUSTOM_SAVE(CLASS, VERSION)		RTTI_IMPLEMENT_CUSTOM_SAVE_INTERNAL(CLASS, VERSION, _effT(#CLASS), CLASS::classBase::GetThisClass())
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
class GetParameterProperty<std::vector<PropertyType>>
{
public:
	GetParameterProperty(const effString & name)
	{
		parameterProperty = EFFNEW EFFPropertyImpl<std::vector<PropertyType>, boost::is_pod<boost::remove_pointer<PropertyType>>::type>::type>();
		SetProperty(parameterProperty, 0, sizeof(PropertyType), name);
	}

	EFFProperty * parameterProperty;
};

template<typename PropertyType>
class GetParameterProperty<std::vector<PropertyType *>>
{
public:
	GetParameterProperty(const effString & name)
	{
		parameterProperty = EFFNEW EFFPropertyImpl<std::vector<PropertyType *>, EFF_IS_POD<PropertyType, boost::is_pod<boost::remove_pointer<PropertyType>>::type>::type>();
		SetProperty(parameterProperty, 0, sizeof(PropertyType), name);
	}

	EFFProperty * parameterProperty;
};

EFFBASE_END


#endif