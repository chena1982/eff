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

struct EFFBASE_API ClassID
{
	effUINT64 classId[2];

	bool operator == (const ClassID & c) const
	{
		return classId[0] == c.classId[0] && classId[1] == c.classId[1];
	}

	bool operator < (const ClassID & c) const
	{
		return classId[0] < c.classId[0];
	}

public:
	effUINT64 High() { return classId[0]; }
	effUINT64 Low() { return classId[1]; }
};

class EFFClass;

effVOID EFFRegisterClass(EFFClass * pClass);
effVOID EFFUnRegisterClass(EFFClass * pClass);

EFFBASE_API effVOID * EFFCreateObject(const effString & className);
EFFBASE_API effVOID * EFFCreateObject(const ClassID & classId);

EFFBASE_API EFFClass *	EFFGetClass(const effString & className);
EFFBASE_API EFFClass *	EFFGetClass(const ClassID & classId);
EFFBASE_API ClassID		ClassIDFromString(const effString & className);
EFFBASE_API effVOID		SetProperty(EFFProperty * addedProperty, effLONG offset, effLONG size, const effString & name);

#define RTTI_CLASS(T)	((EFFClass *)(&T::runtimeInfoClass##T))
#define RTTI_CLASSID(T)	((EFFClass *)(&T::runtimeInfoClass##T))->GetID()

#define RTTI_DECLARE_BASE(CLASS)\
public:\
	typedef CLASS classThis;\
	friend EFFClassImpl<CLASS>;\
	static EFFClassImpl<CLASS> runtimeInfoClass##CLASS;\
	static EFFClass * GetThisClass();\
	virtual EFFClass * GetRuntimeClass() const;\
	virtual void SaveToFile(EFFFile * pFile);\
	virtual void SaveToFile(const effString & filePath);

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
	void CLASS##::SaveToFile(EFFFile * file)\
	{\
		EFFClass * Class = GetRuntimeClass();\
		std::vector<EFFProperty *> & properties = Class->GetProperties();\
		for ( effUINT i = 0; i < properties.size(); i++ )\
		{\
			EFFProperty * curProperty = properties[i];\
			if ( curProperty->GetClass()->IsPODType() && !curProperty->GetIsSTLContainer() )\
			{\
				curProperty->GetSavePropertyFP()(file, this, curProperty);\
			}\
			else\
			{\
				curProperty->SaveToFile(file, ((effBYTE *)this) +  curProperty->GetOffset());\
			}\
		}\
	}\
	void CLASS##::SaveToFile(const effString & filePath)\
	{\
		EFFSTLFile file;\
		if ( !file.Open(filePath, _effT("wb")) )\
		{\
			return;\
		}\
		SaveToFile(&file);\
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
#define RTTI_IMPLEMENT_BASE_PURE(CLASS, VERSION)		RTTI_IMPLEMENT_PURE_INTERNAL(CLASS, VERSION, _effT(#CLASS), NULL)

#define RTTI_IMPLEMENT(CLASS, VERSION)					RTTI_IMPLEMENT_INTERNAL(CLASS, VERSION, _effT(#CLASS), CLASS::classBase::GetThisClass())
#define RTTI_IMPLEMENT_PURE(CLASS, VERSION)				RTTI_IMPLEMENT_PURE_INTERNAL(CLASS, VERSION, _effT(#CLASS), CLASS::classBase::GetThisClass())

#define RTTI_IMPLEMENT_POD(CLASS)						RTTI_IMPLEMENT_POD_INTERNAL(CLASS, 0, _effT(#CLASS), NULL)
#define RTTI_IMPLEMENT_PURE_POD(CLASS)					RTTI_IMPLEMENT_PURE_POD_INTERNAL(CLASS, 0, _effT(#CLASS), NULL)






class EFFBASE_API EFFClass
{
	friend class RegisterProperty;
public:
	EFFClass(effUINT version, effBOOL isPOD, const effString & name, EFFClass * baseClass)
		:version(version), isPOD(isPOD), className(name), baseClass(baseClass)
	{
		id = ClassIDFromString(name);
		EFFRegisterClass(this);
	}



	EFFClass & operator = (const EFFClass & rhs)
	{
		id = rhs.id;
		version = rhs.version;
		className = rhs.className;
		baseClass = rhs.baseClass;
		return *this;
	}

	virtual ~EFFClass()
	{
		for ( effUINT i = 0; i < properties.size(); i++ )
		{
			SF_DELETE(properties[i]);
		}
		properties.clear();
		EFFUnRegisterClass(this);
	}

	//friend class __ucCompareRTTI;
	//friend class __ucCompareRTTIAndClassID;

	virtual effVOID * CreateObject() = 0;

private:
	EFFClass(const EFFClass & rhs) {}
public:
	inline EFFClass * GetBaseClass() const { return baseClass; }

	effString GetName() const { return className; }

	inline effUINT GetVersion() const { return version; }

	inline const ClassID & GetID() const { return id; }

	inline effBOOL IsPODType() const { return isPOD; }

	std::vector<EFFProperty *> & GetProperties()
	{
		return properties;
	}
	
	friend effBOOL operator == (const EFFClass & class1, const EFFClass & class2)
	{
		return class1.id == class2.id && class1.version == class2.version;
	}

	effBOOL IsKindOf(const EFFClass * effClass)
	{
		EFFClass * Class = this;
		while( Class != NULL )
		{
			if( Class == effClass )
			{
				return effTRUE;
			}
			Class = Class->GetBaseClass();
		}
		return effFALSE;
	}

	effBOOL IsKindOf(const ClassID & id)
	{
		EFFClass * Class = this;
		while( Class != NULL )
		{
			if( Class->id == id )
			{
				return effTRUE;
			}
			Class = Class->GetBaseClass();
		}
		return effFALSE;
	}







	//GetClassName-------------------------------------------------------------------------------------------------
	/*template<typename PropertyType, typename IsPODType>
	class ClassNameTrait
	{
	public:
		effString operator ()()
		{
			return effString("class name trait error");

		}
	};

	template<typename PropertyType>
	class ClassNameTrait<PropertyType, boost::true_type>
	{
	public:
		effString operator ()()
		{
			return getPODTypeClassName(typeid(PropertyType).name());
		}
	};

	template<typename PropertyType>
	class ClassNameTrait<PropertyType, boost::false_type>
	{
	public:
		effString operator ()()
		{
			effString className = PropertyType::GetThisClass()->className;
			return className;
		}
	};


	template<>
	class ClassNameTrait<effString, boost::true_type>
	{
	public:
		effString operator ()()
		{
			return _effT("effString");
		}
	};*/

	/*template<typename ClassType, typename PropertyType>
	inline static effString GetClassName(PropertyType ClassType::*member, boost::true_type)
	{
		const effCHAR * propertyTypeName = typeid(PropertyType).name();
		return getPODTypeClassName(propertyTypeName);
	}

	template<typename ClassType, typename PropertyType>
	inline static effString GetClassName(PropertyType ClassType::*member, boost::false_type)
	{
		effString result(ANSI2EFFSTRING(typeid(PropertyType).name()));
		//remove the prefix "class "
		return result.substr(6, result.length() - 6);
	}

#if defined UNICODE || defined _UNICODE
	template<typename ClassType>
	inline static effString GetClassName(std::wstring ClassType::*member, boost::false_type)
	{
		return effString(_effT("effString"));
	};
#else
	template<typename ClassType>
	inline static effString GetClassName(std::string ClassType::*member, boost::false_type)
	{
		return effString(_effT("effString"));
	};
#endif*/




	//AddProperty-------------------------------------------------------------------------------------------------
	/*template<typename ClassType, typename PropertyType>
	effVOID AddProperty(PropertyType ClassType::*member, const effString & name)
	{
		EFFClass * propertyClass = EFFGetClass(ClassNameTrait<PropertyType>()());
		EFFProperty * addedProperty = addProperty(mem_offset(member), sizeof(PropertyType), name, propertyClass);

		if ( propertyClass->isPOD )
		{
			if ( propertyClass->className == _effT("effString") )
			{
				addedProperty->savePropertyFP = &SaveStringProperty;
			}
			else
			{
				addedProperty->savePropertyFP = &SavePODProperty;
			}
		}
	}


	//vector property
	template<typename ClassType, typename PropertyType>
	effVOID AddProperty(std::vector<PropertyType> ClassType::*member, const effString & name)
	{

		EFFClass * propertyClass = EFFGetClass(ClassNameTrait<PropertyType>()());
		EFFProperty * addedProperty = addProperty(mem_offset(member), sizeof(PropertyType), name, propertyClass);
		addedProperty->isSTLContainer = effTRUE;
		addedProperty->isPointer = effFALSE;
		addedProperty->savePropertyFP = SaveVectorProperty<PropertyType>;

	}

	template<typename ClassType, typename PropertyType>
	effVOID AddPODProperty(std::vector<PropertyType> ClassType::*member, const effString & name)
	{
		EFFClass * propertyClass = EFFGetClass(ClassNameTrait<PropertyType>()());
		EFFProperty * addedProperty = addProperty(mem_offset(member), sizeof(PropertyType), name, propertyClass);
		addedProperty->isSTLContainer = effTRUE;
		addedProperty->isPointer = effFALSE;
		addedProperty->savePropertyFP = SavePODVectorProperty<PropertyType>;
	}

	template<typename ClassType, typename PropertyType>
	effVOID AddProperty(std::vector<PropertyType *> ClassType::*member, const effString & name)
	{
		EFFClass * propertyClass = EFFGetClass(ClassNameTrait<PropertyType>()());
		EFFProperty * addedProperty = addProperty(mem_offset(member), sizeof(PropertyType), name, propertyClass);
		addedProperty->isSTLContainer = effTRUE;
		addedProperty->isPointer = effTRUE;

		if ( propertyClass->isPOD )
		{
			BOOST_ASSERT_MSG(effFALSE, "dont't support pod pointer vector property.");
		}
		else
		{
			addedProperty->savePropertyFP = SavePointerVectorProperty<PropertyType>;
		}
	}*/

	
	template<typename ClassType, typename PropertyType>
	effVOID AddProperty(PropertyType ClassType::*member, const effString & name)
	{
		EFFProperty * addedProperty = EFFNEW EFFPropertyImpl<PropertyType, EFF_IS_POD<PropertyType, boost::is_pod<boost::remove_pointer<PropertyType>>::type>::type>();
		SetProperty(addedProperty, mem_offset(member), sizeof(PropertyType), name);
		properties.push_back(addedProperty);
	}


	template<typename ClassType, typename PropertyType>
	effVOID AddProperty(std::vector<PropertyType> ClassType::*member, const effString & name)
	{
		EFFProperty * addedProperty = EFFNEW EFFPropertyImpl<std::vector<PropertyType>, EFF_IS_POD<PropertyType, boost::is_pod<boost::remove_pointer<PropertyType>>::type>::type>();
		SetProperty(addedProperty, mem_offset(member), sizeof(PropertyType), name);
		properties.push_back(addedProperty);
	}

	template<typename ClassType, typename PropertyType>
	effVOID AddProperty(std::vector<PropertyType *> ClassType::*member, const effString & name)
	{
		EFFProperty * addedProperty = EFFNEW EFFPropertyImpl<std::vector<PropertyType *>, EFF_IS_POD<PropertyType, boost::is_pod<PropertyType>::type>::type>();
		SetProperty(addedProperty, mem_offset(member), sizeof(PropertyType), name);
		properties.push_back(addedProperty);
	}
protected:
	
	//static effString		getPODTypeClassName(const effCHAR * propertyTypeName);





protected:
	ClassID										id;
	effUINT										version;
	effBOOL										isPOD;
	effString									className;
	EFFClass *									baseClass;
	std::vector<EFFProperty *>					properties;
};


template<class T>
class EFFClassImpl : public EFFClass
{
	typedef T classType;
public:
	virtual effVOID * CreateObject() { return new T; }

	EFFClassImpl(effUINT version, effBOOL isPOD, const effString & name, EFFClass * pBaseClas) : EFFClass(version, isPOD, name, pBaseClas) { }
	
	//EFFClassImpl() : EFFClass(0,0,0,NULL) { }

	EFFClassImpl(const EFFClassImpl<T> & rhs) : EFFClass(rhs) { }
};


template<class T>
class EFFPureVirtualClassImpl : public EFFClass
{
public:
	virtual effVOID * CreateObject() { return NULL; }

	EFFPureVirtualClassImpl(effUINT version, effBOOL isPOD, const effString & name, EFFClass * pBaseClas) : EFFClass(version, isPOD, name, pBaseClas) {}
	
	//EFFRunTimeTypeInfoImplPureVirtual() : EFFClass(0,NULL,NULL) {}

	EFFPureVirtualClassImpl(const EFFPureVirtualClassImpl<T> & rhs) : EFFClass(rhs) {}
};


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