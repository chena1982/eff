/******************************************************************************
	created:	2011-11-30   22:48
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFProperty.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFProperty_2011_22_48__
#define __EFFProperty_2011_22_48__

#include <boost\type_traits.hpp>

#include "EFFStringHash.h"
#include "EFFRttiApi.h"
#include "EFFFile.h"


#include "metareflect.hxx"

/*namespace YAML
{
	template<>
	struct convert<effString>
	{
		static Node encode(const effString & rhs)
		{
			return Node(EFFBase::EFFSTRING2ANSI(rhs));
		}
		
		static bool decode(const Node & node, effString & rhs)
		{
			if ( !node.IsScalar() )
			{
				return false;
			}

			rhs = EFFBase::ANSI2EFFSTRING(node.Scalar().c_str());
			return true;
		}
	};
}*/

EFFBASE_BEGIN

class EFFClass;
class EFFFile;

#pragma warning(push)
#pragma warning(disable:4251)


//typedef effVOID (* SavePropertyFP)(EFFFile * file, effVOID * baseAddress, EFFProperty * property, effBOOL isBinary, YAML::Emitter * textOut);

class EFFBASE_API EFFProperty2
{
	friend class EFFClass;
public:
	enum STLContainerType
	{
		ContainerType_None,
		ContainerType_Vector,
		ContainerType_Map,
		ContainerType_List,
	};

public:
	EFFProperty2()
	{
		offset = 0;
		size = 0;
		defaultValue = NULL;
		minValue = NULL;
		maxValue = NULL;
		type = NULL;

		isPointer = effFALSE;
		isArray = effFALSE;
		stlContainerType = ContainerType_None;
		//savePropertyFP = NULL;
	}
public:
	virtual ~EFFProperty2() {}


public:


public:
	template<typename PropertyType>
	effBOOL GetValue(effVOID * baseAddress, PropertyType & result)
	{
		result = *((PropertyType *)((effBYTE *)baseAddress + offset));
		return effTRUE;
	}

	template<typename PropertyType>
	effBOOL GetElement(effVOID * baseAddress, effUINT index, PropertyType & result)
	{
		if (stlContainerType == EFFProperty::ContainerType_Vector)
		{
			VECTOR<PropertyType> & propertyVector = *((VECTOR<PropertyType> *)((effBYTE *)baseAddress + offset));
			result = propertyVector[index];
			return effTRUE;
		}


		return effFALSE;
	}

	effSIZE GetElementCount(effVOID * baseAddress);

	template<typename PropertyType, typename Visitor>
	effVOID ForEach(effVOID * baseAddress, Visitor visitor)
	{
		if (stlContainerType == EFFProperty::ContainerType_Vector)
		{
			//PropertyForEachString(baseAddress, visitor);
			VECTOR<PropertyType> & propertyVector = *((VECTOR<PropertyType> *)((effBYTE *)baseAddress + offset));
			FOR_EACH(propertyVector.begin(), propertyVector.end(), visitor);
		}
	}

public:
	effUINT					offset;
	effUINT					size;


	effString				name;
	EFFStringHash			nameHash;
	effVOID *				defaultValue;
	effVOID *				minValue;
	effVOID *				maxValue;
	EFFClass *				type;
	effBOOL					isPointer;
	effBOOL					isArray;
	STLContainerType		stlContainerType;
	//SavePropertyFP			savePropertyFP;

	metareflect::Qualifier		qualifier;
};


class EFFBASE_API EFFProperty
{
	friend class EFFClass;
public:
	enum STLContainerType
	{
		ContainerType_None,
		ContainerType_Vector,
		ContainerType_Map,
		ContainerType_List,
	};

public:
	EFFProperty()
	{
		offset = 0;
		size = 0;
		defaultValue = NULL;
		minValue = NULL;
		maxValue = NULL;
		Class = NULL;

		isPointer = effFALSE;
		isArray = effFALSE;
		stlContainerType = ContainerType_None;
		//savePropertyFP = NULL;
	}
public:
	virtual ~EFFProperty() {}

	virtual effVOID				SetOffset(effUINT offset) { this->offset = offset; }
	virtual effVOID				SetSize(effUINT size) { this->size = size; }
	virtual effVOID				SetName(const effString & name);
	virtual effVOID				SetClass(EFFClass * propertyClass) { Class = propertyClass; }
	virtual effVOID				SetIsPointer(effBOOL isPointer) { this->isPointer = isPointer; }
	//virtual effVOID				SetSavePropertyFP(SavePropertyFP savePropertyFP) { this->savePropertyFP = savePropertyFP; }

	virtual effUINT				GetOffset() { return offset; }
	virtual effUINT				GetSize() { return size; }
	virtual effString &			GetName() { return name; }
	virtual EFFStringHash		GetNameHash() { return nameHash; }
	virtual EFFClass *			GetClass() { return Class; }
	virtual effBOOL				GetIsPointer() { return isPointer; }
	virtual effBOOL				GetIsSTLContainer() { return stlContainerType != ContainerType_None; }
	virtual STLContainerType	GetSTLContainerType() { return stlContainerType; }
	//virtual SavePropertyFP		GetSavePropertyFP() { return savePropertyFP; }


	virtual effVOID				SaveToFile(EFFFile * file, effVOID * baseAddress, effBOOL isBinary) = 0;

public:
	template<typename PropertyType>
	effBOOL GetValue(effVOID * baseAddress, PropertyType & result)
	{
		result = *((PropertyType *)((effBYTE *)baseAddress + offset));
		return effTRUE;
	}

	template<typename PropertyType>
	effBOOL GetElement(effVOID * baseAddress, effUINT index, PropertyType & result)
	{
		if ( stlContainerType == EFFProperty::ContainerType_Vector )
		{
			VECTOR<PropertyType> & propertyVector = *((VECTOR<PropertyType> *)((effBYTE *)baseAddress + offset));
			result = propertyVector[index];
			return effTRUE;
		}


		return effFALSE;
	}

	effSIZE GetElementCount(effVOID * baseAddress);

	template<typename PropertyType, typename Visitor>
	effVOID ForEach(effVOID * baseAddress, Visitor visitor)
	{
		if ( stlContainerType == EFFProperty::ContainerType_Vector )
		{
			//PropertyForEachString(baseAddress, visitor);
			VECTOR<PropertyType> & propertyVector = *((VECTOR<PropertyType> *)((effBYTE *)baseAddress + offset));
			FOR_EACH(propertyVector.begin(), propertyVector.end(), visitor);
		}
	}

protected:
	effUINT					offset;
	effUINT					size;


	effString				name;
	EFFStringHash			nameHash;
	effVOID *				defaultValue;
	effVOID *				minValue;
	effVOID *				maxValue;
	EFFClass *				Class;
	effBOOL					isPointer;
	effBOOL					isArray;
	STLContainerType		stlContainerType;
	//SavePropertyFP			savePropertyFP;
};


#include "EFFSerialize.h"

template<typename PropertyeType, typename IsPODType>
class EFFPropertyImpl : public EFFProperty
{
	friend class EFFClass;
public:
	EFFPropertyImpl()
	{
	}
public:
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress, effBOOL isBinary)
	{
	}
};

template<typename PropertyType>
class EFFPropertyImpl<PropertyType, boost::true_type> : public EFFProperty
{
	friend class EFFClass;
public:
	EFFPropertyImpl()
	{
		Class = EFFGetClass(ClassNameTrait<PropertyType, EFF_IS_POD<PropertyType, boost::is_pod<PropertyType>::type>::type>()());
		isPointer = boost::is_pointer<PropertyType>();
	}
public:
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress, effBOOL isBinary)
	{

		if ( Class->GetNameHash() == EFFStringHash(_effT("effString")) )
		{
			effString & data = *((effString *)((effBYTE *)baseAddress + offset));
			SaveProperty(file, data);
		}
		else if ( Class->GetNameHash() == EFFStringHash(_effT("effINT")) )
		{
			effINT & data = *((effINT *)((effBYTE *)baseAddress + offset));
			SaveProperty(file, data);
		}
		else if ( Class->GetNameHash() == EFFStringHash(_effT("effUINT")) )
		{
			effUINT & data = *((effUINT *)((effBYTE *)baseAddress + offset));
			SaveProperty(file, data);
		}
		else if ( Class->GetNameHash() == EFFStringHash(_effT("effFloat")) )
		{
			effFLOAT & data = *((effFLOAT *)((effBYTE *)baseAddress + offset));
			SaveProperty(file, data);
		}
		else
		{
			SaveCustomSaveProperty<PropertyType>(file, baseAddress, this);
		}
	}
};

template<>
class EFFPropertyImpl<effVOID, boost::true_type> : public EFFProperty
{
	friend class EFFClass;
public:
	EFFPropertyImpl()
	{
		Class = EFFGetClass(_effT("void"));
		isPointer = effFALSE;
	}
public:
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress, effBOOL isBinary) {}

};

template<>
class EFFPropertyImpl<effVOID *, boost::true_type> : public EFFProperty
{
	friend class EFFClass;
public:
	EFFPropertyImpl()
	{
		Class = EFFGetClass(_effT("void"));
		isPointer = effTRUE;
	}
public:
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress, effBOOL isBinary) {}

};

template<typename PropertyType>
class EFFPropertyImpl<PropertyType, boost::false_type> : public EFFProperty
{
	friend class EFFClass;
public:
	EFFPropertyImpl()
	{
		Class = EFFGetClass(ClassNameTrait<PropertyType, EFF_IS_POD<PropertyType, boost::is_pod<PropertyType>::type>::type>()());
		isPointer = boost::is_pointer<PropertyType>();
	}
public:
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress, effBOOL isBinary)
	{
		PropertyType * data = (PropertyType *)((effBYTE *)baseAddress + offset);

		if ( isBinary )
		{
			data->SaveToFile(file, isBinary);
		}
		else
		{
			SaveNoPODPropertyToYAMLFile(name, data);
			//node[EFFSTRING2ANSI(GetName())] = data->GetID();
		}
	}

};

template<typename PropertyType>
class EFFPropertyImpl<PropertyType *, boost::false_type> : public EFFProperty
{
	friend class EFFClass;
public:
	EFFPropertyImpl()
	{
		Class = EFFGetClass(ClassNameTrait<PropertyType, EFF_IS_POD<PropertyType, boost::is_pod<PropertyType>::type>::type>()());
		isPointer = boost::is_pointer<PropertyType>();
	}
public:
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress, effBOOL isBinary)
	{
		PropertyType * data = *((PropertyType **)((effBYTE *)baseAddress + offset));

		if ( isBinary )
		{
			data->SaveToFile(file, isBinary);
		}
		else
		{
			//node[EFFSTRING2ANSI(GetName())] = data->GetID();
		}
	}

};

template<typename PropertyType>
class EFFPropertyImpl<PropertyType &, boost::false_type> : public EFFProperty
{
	friend class EFFClass;
public:
	EFFPropertyImpl()
	{
		Class = EFFGetClass(ClassNameTrait<PropertyType, EFF_IS_POD<PropertyType, boost::is_pod<PropertyType>::type>::type>()());
		isPointer = boost::is_pointer<PropertyType>();
	}
public:
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress, effBOOL isBinary)
	{
		if ( !isPointer )
		{
			PropertyType & data = *(PropertyType *)baseAddress;
			data.SaveToFile(file, isBinary);
		}
		else
		{
			PropertyType & data = **(PropertyType **)baseAddress;
			data.SaveToFile(file, isBinary);
		}
	}

};

template<typename PropertyType>
class EFFPropertyImpl<VECTOR<PropertyType>, boost::false_type> : public EFFProperty
{
	friend class EFFClass;
public:
	EFFPropertyImpl()
	{
		Class = EFFGetClass(ClassNameTrait<PropertyType, EFF_IS_POD<PropertyType, boost::is_pod<PropertyType>::type>::type>()());
		stlContainerType = EFFProperty::ContainerType_Vector;
		//BOOST_ASSERT(!boost::is_pointer<PropertyType>());
		isPointer = effFALSE;
	}
public:
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress, effBOOL isBinary)
	{
		VECTOR<PropertyType> & datas = *(VECTOR<PropertyType> *)baseAddress;

		for ( effUINT i = 0; i < datas.size(); i++ )
		{
			datas[i].SaveToFile(file, isBinary);
		}
	}

};

template<typename PropertyType>
class EFFPropertyImpl<VECTOR<PropertyType>, boost::true_type> : public EFFProperty
{
	friend class EFFClass;
public:
	EFFPropertyImpl()
	{
		Class = EFFGetClass(ClassNameTrait<PropertyType, EFF_IS_POD<PropertyType, boost::is_pod<PropertyType>::type>::type>()());
		stlContainerType = EFFProperty::ContainerType_Vector;
		//BOOST_ASSERT(!boost::is_pointer<PropertyType>());
		isPointer = effFALSE;
	}
public:
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress, effBOOL isBinary)
	{



		if ( Class->GetNameHash() == EFFStringHash(_effT("effString")) )
		{
			VECTOR<effString> & datas = *((VECTOR<effString> *)((effBYTE *)baseAddress + offset));
			SaveStringVectorProperty(file, name, datas, isBinary);
		}
		else if ( Class->GetNameHash() == EFFStringHash(_effT("effINT")) )
		{
			VECTOR<effINT> & datas = *((VECTOR<effINT> *)((effBYTE *)baseAddress + offset));
			SaveIntVectorProperty(file, name, datas, isBinary);
		}
		/*else if ( Class->GetNameHash() == EFFStringHash(_effT("effUINT")) )
		{
			effUINT & data = *((effUINT *)((effBYTE *)baseAddress + offset));
			SaveUintProperty(file, name, data, isBinary);
		}
		else if ( Class->GetNameHash() == EFFStringHash(_effT("effFloat")) )
		{
			effFLOAT & data = *((effFLOAT *)((effBYTE *)baseAddress + offset));
			SaveFloatProperty(file, name, data, isBinary);
		}*/
	}

};

template<typename PropertyType>
class EFFPropertyImpl<VECTOR<PropertyType *>, boost::false_type> : public EFFProperty
{
	friend class EFFClass;
public:
	EFFPropertyImpl()
	{
		Class = EFFGetClass(ClassNameTrait<PropertyType, EFF_IS_POD<PropertyType, boost::is_pod<PropertyType>::type>::type>()());
		stlContainerType = EFFProperty::ContainerType_Vector;
		//BOOST_ASSERT(boost::is_pointer<PropertyType>());
		isPointer = effTRUE;
	}
public:
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress, effBOOL isBinary)
	{
		VECTOR<PropertyType *> & datas = *(VECTOR<PropertyType *> *)baseAddress;

		for ( effUINT i = 0; i < datas.size(); i++ )
		{
			datas[i]->SaveToFile(file, isBinary);
		}
	}

};



/*class EFFStringProperty : public EFFProperty
{
public:
	virtual effVOID SaveToFile(EFFFile * file, effVOID * baseAddress);
};

class EFFPODProperty : public EFFProperty
{
public:
	virtual effVOID SaveToFile(EFFFile * file, effVOID * baseAddress);
};



template<typename propertyType>
class EFFVectorProperty : public EFFProperty
{
public:
	virtual effVOID SaveToFile(EFFFile * file, effVOID * baseAddress)
	{
		VECTOR<propertyType> & data = *((VECTOR<propertyType> *)((effBYTE *)baseAddress + offset));
		VECTOR<propertyType>::iterator it = data.begin();
		
		if ( Class->GetName() == _effT("effString") )
		{
			static EFFStringProperty stringProperty;
			for ( ; it != data.end(); it++ )
			{
				stringProperty.SaveToFile(file, (effVOID *)&(*it));
			}
		}
		else if ( Class->isPOD )
		{
			effULONG vectorOffset = reinterpret_cast<effULONG>((effVOID *)&data[0]) - reinterpret_cast<effULONG>((effVOID *)&data);
			effVOID * source = (effVOID *)((effBYTE *)baseAddress + offset + vectorOffset);
			file->Write(source, size * data.size());
		}
	}

private:
};*/



#pragma warning(pop)

EFFBASE_END


#endif