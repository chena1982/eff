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

class EFFBASE_API AnsiUnicodeStringConvert
{
public:
	static const effCHAR * W2A(const effWCHAR * str);
	static const effWCHAR * A2W(const effCHAR * str);

private:
	static effCHAR charBuffer[1024];
	static effWCHAR wcharBuffer[1024];
};

#if defined UNICODE || defined _UNICODE
	#define EFFSTRING2ANSI(str) AnsiUnicodeStringConvert::W2A((str.c_str()))
	#define ANSI2EFFSTRING(str) AnsiUnicodeStringConvert::A2W(str)
#else
	#define EFFSTRING2ANSI(str) (str).c_str()
	#define ANSI2EFFSTRING(str) str
#endif


class EFFClass;

void EFFRegisterClass(EFFClass * pClass);
void EFFUnRegisterClass(EFFClass * pClass);

EFFBASE_API void * EFFCreateObject(const effString & className);
EFFBASE_API void * EFFCreateObject(const ClassID & classId);

EFFBASE_API EFFClass * EFFGetClass(const effString & className);
EFFBASE_API EFFClass * EFFGetClass(const ClassID & classId);
ClassID EFFBASE_API ClassIDFromString(const effString & className);


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
			curProperty->GetSavePropertyFP()(file, this, curProperty);\
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

#define RTTI_IMPLEMENT_BASE(CLASS, VERSION)				RTTI_IMPLEMENT_INTERNAL(CLASS, VERSION, _effT(#CLASS), NULL)
#define RTTI_IMPLEMENT_BASE_PURE(CLASS, VERSION)		RTTI_IMPLEMENT_PURE_INTERNAL(CLASS, VERSION, _effT(#CLASS), NULL)

#define RTTI_IMPLEMENT(CLASS, VERSION)					RTTI_IMPLEMENT_INTERNAL(CLASS, VERSION, _effT(#CLASS), CLASS::classBase::GetThisClass())
#define RTTI_IMPLEMENT_PURE(CLASS, VERSION)				RTTI_IMPLEMENT_PURE_INTERNAL(CLASS, VERSION, _effT(#CLASS), CLASS::classBase::GetThisClass())

#define RTTI_IMPLEMENT_POD(CLASS)						RTTI_IMPLEMENT_POD_INTERNAL(CLASS, 0, _effT(#CLASS), NULL)




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
	EFFClass * GetBaseClass() const 
	{
		return baseClass;
	}

	const effString & GetClassName() const
	{
		return className;
	}

	effUINT GetVersion() const
	{
		return version;
	}

	const ClassID & GetID() const
	{
		return id; 
	}

	std::vector<EFFProperty *> & GetProperties()
	{
		return properties;
	}
	
	friend bool operator == (const EFFClass & class1,const EFFClass & class2)
	{
		return class1.id == class2.id && class1.version == class2.version;
	}

	bool IsKindOf(const EFFClass * effClass)
	{
		EFFClass * Class = this;
		while( Class != NULL )
		{
			if( Class == effClass )
			{
				return true;
			}
			Class = Class->GetBaseClass();
		}
		return false;
	}

	bool IsKindOf(ClassID & id)
	{
		EFFClass * Class = this;
		while( Class != NULL )
		{
			if( Class->id == id )
			{
				return true;
			}
			Class = Class->GetBaseClass();
		}
		return false;
	}

	template<typename ClassType, typename PropertyType>
	inline static effString GetClassName(PropertyType ClassType::*member)
	{
		return GetClassName(member, boost::is_pod<PropertyType>());
	}

	template<typename ClassType, typename PropertyType>
	inline static effString GetClassName(PropertyType ClassType::*member, boost::true_type)
	{
		const char * propertyTypeName = typeid(PropertyType).name();
		if ( strcmp(propertyTypeName, "int") == 0 )
		{
			return effString(_effT("effINT"));
		}
		else if ( strcmp(propertyTypeName, "float") == 0 )
		{
			return effString(_effT("effFLOAT"));
		}
		return effString(_effT("unkown pod type"));
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
#endif




	template<class ClassType, class PropertyType>
	void AddProperty(PropertyType ClassType::*member, const effString & name)
	{
		EFFClass * propertyClass = EFFGetClass(GetClassName(member, boost::is_pod<PropertyType>()));
		_addProperty(mem_offset(member), sizeof(PropertyType), name, propertyClass);
	}


protected:
	inline void _addProperty(effLONG offset, effLONG size, const effString & name, EFFClass * Class)
	{
		EFFProperty * addedProperty = EFFNEW EFFProperty();
		addedProperty->offset = offset;
		addedProperty->size = size;
		addedProperty->name = name;
		addedProperty->Class = Class;

		if ( Class->isPOD )
		{
			if ( Class->className == _effT("effString") )
			{
				addedProperty->savePropertyFP = &SaveStringProperty;
			}
			else
			{
				addedProperty->savePropertyFP = &SavePODProperty;
			}
		}
		else
		{

		}

		properties.push_back(addedProperty);
	}

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
		Class->_addProperty(offset, size, name, propertyClass);
	}

};

#define RTTI_PROPERTY(CLASS, MEMBER, NAME)\
	static RegisterProperty registerProperty##CLASS##MEMBER(EFFClass::GetClassName(&CLASS::MEMBER), CLASS::GetThisClass(), MEM_OFFSET(CLASS, MEMBER), MEM_SIZE(CLASS, MEMBER), NAME);

RTTI_DECLARE_POD(effString)
RTTI_DECLARE_POD(effINT)
RTTI_DECLARE_POD(effFLOAT)




EFFBASE_END


#endif