/******************************************************************************
	created:	2012-12-26   23:36
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFRttiApi.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFMethod_2012_12_26__
#define __EFFMethod_2012_12_26__

EFFBASE_BEGIN

class EFFClass;
class EFFProperty;

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

ClassID	ClassIDFromString(const effString & className);

EFFBASE_API effVOID		EFFRegisterClass(EFFClass * pClass);
EFFBASE_API effVOID		EFFUnRegisterClass(EFFClass * pClass);

EFFBASE_API effVOID *	EFFCreateObject(const effString & className);
EFFBASE_API effVOID *	EFFCreateObject(const ClassID & classId);

EFFBASE_API EFFClass *	EFFGetClass(const effString & className);
EFFBASE_API EFFClass *	EFFGetClass(const ClassID & classId);

EFFBASE_API effVOID SetProperty(EFFProperty * addedProperty, effLONG offset, effLONG size, const effString & name);

class EFFBASE_API EFFClass
{
	friend class RegisterProperty;
public:
	EFFClass(effUINT version, effBOOL isPOD, const effString & name, EFFClass * baseClass)
		:version(version), isPOD(isPOD), className(name), baseClass(baseClass)
	{
		id = ClassIDFromString(name);
		classNameHash.CalculateHash(name);
		EFFRegisterClass(this);
	}



	EFFClass & operator = (const EFFClass & rhs)
	{
		id = rhs.id;
		version = rhs.version;
		className = rhs.className;
		classNameHash = rhs.classNameHash;
		baseClass = rhs.baseClass;
		return *this;
	}

	virtual ~EFFClass();


	//friend class __ucCompareRTTI;
	//friend class __ucCompareRTTIAndClassID;

	virtual effVOID * CreateObject() = 0;

private:
	EFFClass(const EFFClass & rhs) {}
public:
	inline EFFClass * GetBaseClass() const { return baseClass; }

	effString GetName() const { return className; }

	EFFStringHash GetNameHash() const { return classNameHash; }

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
	EFFStringHash								classNameHash;
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


EFFBASE_END

#endif