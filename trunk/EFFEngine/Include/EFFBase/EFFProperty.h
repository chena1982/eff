/******************************************************************************
	created:	2011-11-30   22:48
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFProperty.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFProperty_2011_22_48__
#define __EFFProperty_2011_22_48__

#include <boost\type_traits.hpp>
#include "EFFUtility.h"

EFFBASE_BEGIN

class EFFClass;
class EFFProperty;
class EFFFile;

#pragma warning(push)
#pragma warning(disable:4251)


typedef effVOID (* SavePropertyFP)(EFFFile * file, effVOID * baseAddress, EFFProperty * property);


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
		savePropertyFP = NULL;
	}
public:
	virtual ~EFFProperty() {}

	virtual effVOID				SetOffset(effUINT offset) { this->offset = offset; }
	virtual effVOID				SetSize(effUINT size) { this->size = size; }
	virtual effVOID				SetName(const effString & name) { this->name = name; }
	virtual effVOID				SetClass(EFFClass * propertyClass) { Class = propertyClass; }
	virtual effVOID				SetIsPointer(effBOOL isPointer) { this->isPointer = isPointer; }
	virtual effVOID				SetSavePropertyFP(SavePropertyFP savePropertyFP) { this->savePropertyFP = savePropertyFP; }

	virtual effUINT				GetOffset() { return offset; }
	virtual effUINT				GetSize() { return size; }
	virtual effString			GetName() { return name; }
	virtual EFFClass *			GetClass() { return Class; }
	virtual effBOOL				GetIsPointer() { return isPointer; }
	virtual effBOOL				GetIsSTLContainer() { return stlContainerType != ContainerType_None; }
	virtual STLContainerType	GetSTLContainerType() { return stlContainerType; }
	virtual SavePropertyFP		GetSavePropertyFP() { return savePropertyFP; }


	virtual effVOID				SaveToFile(EFFFile * file, effVOID * baseAddress) = 0;
protected:
	effUINT					offset;
	effUINT					size;


	effString				name;
	effVOID *				defaultValue;
	effVOID *				minValue;
	effVOID *				maxValue;
	EFFClass *				Class;
	effBOOL					isPointer;
	effBOOL					isArray;
	STLContainerType		stlContainerType;
	SavePropertyFP			savePropertyFP;
};

template<typename PropertyeType, typename IsPODType>
class EFFPropertyImpl : public EFFProperty
{
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
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress) {}
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
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress)
	{
		if ( !isPointer )
		{
			PropertyType & data = *(PropertyType *)baseAddress;
			data.SaveToFile(file);
		}
		else
		{
			PropertyType & data = **(PropertyType **)baseAddress;
			data.SaveToFile(file);
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
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress)
	{
		if ( !isPointer )
		{
			PropertyType & data = *(PropertyType *)baseAddress;
			data.SaveToFile(file);
		}
		else
		{
			PropertyType & data = **(PropertyType **)baseAddress;
			data.SaveToFile(file);
		}
	}

};

template<typename PropertyType>
class EFFPropertyImpl<std::vector<PropertyType>, boost::false_type> : public EFFProperty
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
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress)
	{
		std::vector<PropertyType> & datas = *(std::vector<PropertyType> *)baseAddress;

		for ( effUINT i = 0; i < datas.size(); i++ )
		{
			datas[i].SaveToFile(file);
		}
	}

};

template<typename PropertyType>
class EFFPropertyImpl<std::vector<PropertyType>, boost::true_type> : public EFFProperty
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
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress)
	{
	}

};

template<typename PropertyType>
class EFFPropertyImpl<std::vector<PropertyType *>, boost::false_type> : public EFFProperty
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
	virtual effVOID	SaveToFile(EFFFile * file, effVOID * baseAddress)
	{
		std::vector<PropertyType *> & datas = *(std::vector<PropertyType *> *)baseAddress;

		for ( effUINT i = 0; i < datas.size(); i++ )
		{
			datas[i]->SaveToFile(file);
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
		std::vector<propertyType> & data = *((std::vector<propertyType> *)((effBYTE *)baseAddress + offset));
		std::vector<propertyType>::iterator it = data.begin();
		
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