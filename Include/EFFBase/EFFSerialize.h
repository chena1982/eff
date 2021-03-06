/******************************************************************************
	created:	2008-12-1   22:33
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFSerialize.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFSerialize_2008_12_1__
#define __EFFSerialize_2008_12_1__




#include <boost\type_traits.hpp>
#include <boost\static_assert.hpp>


//EFFBASE_BEGIN


//struct ArgWriteXml { TiXmlNode *node; String filename; Dword fileVersion; Dword classVersion; };
//struct ArgReadXml { TiXmlNode *node; String filename; Dword fileVersion; Dword classVersion; XmlFormat format; };

struct ArgWriteBin
{
	EFFFile *			pFile;
    effStringA		    strFileName;
	unsigned int		dwFileVersion;
	unsigned int		dwClassVersion;
};

struct ArgReadBin
{
	EFFFile *			pFile;
    effStringA			strFileName;
	unsigned int		dwFileVersion;
	unsigned int		dwClassVersion;
	//BinFormat format;
};

/*inline YAML::Node & operator << (YAML::Node & node, const effString & str)
{
    //out << EFFSTRING2ANSI(str);
    return node;
}*/

template<typename PropertyType>
inline effVOID SaveCustomSaveProperty(EFFFile * file, effVOID * baseAddress, EFFProperty * Property)
{
	PropertyType * data = NULL;
	if (!Property->GetIsPointer())
	{
		data = (PropertyType *)((effBYTE *)baseAddress + Property->GetOffset());
	}
	else
	{
		data = *(PropertyType **)((effBYTE *)baseAddress + Property->GetOffset());
	}


	file->Write(data, sizeof(PropertyType));

	//node[EFFSTRING2ANSI(Property->GetName())] = *data;
	
}

template<typename Type>
effVOID SaveProperty(EFFFile * file, const Type & data)
{

}

template<>
inline effVOID SaveProperty(EFFFile * file, const effString & data)
{
    effSIZE length = data.length();
	file->Write(&length, sizeof(effSIZE));
	file->Write((effVOID *)data.c_str(), length * sizeof(effTCHAR));
}

template<>
inline effVOID SaveProperty(EFFFile * file, const VECTOR<effString> & datas)
{
	effSIZE length = datas.size();
	file->Write(&length, sizeof(effSIZE));
	for (effSIZE i = 0; i < datas.size(); i++)
	{
		SaveProperty(file, datas[i]);
	}
}

template<>
inline effVOID SaveProperty(EFFFile * file, const effINT & data)
{
	file->Write((effVOID *)&data, 4);
	//SaveIntPropertyToYAMLFile(propertyName, data);
}

template<typename Type>
inline effVOID SaveIntVectorProperty(EFFFile * file, const VECTOR<Type> & datas)
{
	effSIZE length = datas.size();
	file->Write(&length, sizeof(effSIZE));
	if (length > 0)
	{
		file->Write((effVOID *)&datas[0], sizeof(Type) * length);
	}
}

template<>
inline effVOID SaveProperty(EFFFile * file, const effUINT & data)
{
	file->Write((effVOID *)&data, 4);
}

template<>
inline effVOID SaveProperty(EFFFile * file, const effFLOAT & data)
{
	file->Write((effVOID *)&data, 4);
}


template<typename PropertyType, typename IsPOD>
inline void SaveProperty(EFFFile * file, effVOID * baseAddress, EFFProperty * property, effBOOL isBinary)
{

}

template<typename PropertyType>
inline void SaveProperty(EFFFile * file, effVOID * baseAddress, EFFProperty * property, effBOOL isBinary)
{
}

/*template<typename PropertyType>
inline void SavePODVectorProperty(EFFFile * file, effVOID * baseAddress, EFFProperty * property)
{
	VECTOR<PropertyType> & datas = *((VECTOR<PropertyType> *)((effBYTE *)baseAddress + property->GetOffset()));
	VECTOR<PropertyType>::iterator it = datas.begin();

	effUINT size = datas.size();
	file->Write(&size, 4);

	if ( !property->GetClass()->IsPODType() )
	{
		BOOST_ASSERT_MSG(effFALSE, "code should not go here.");
		return;
	}

	if ( property->GetClass()->GetClassName() == _effT("effString") )
	{
		for ( ; it != datas.end(); it++ )
		{
			effString & element = *((effString *)(effVOID *)(&(*it)));
			effUINT length = element.length();
			file->Write(&length, 4);
			file->Write((effVOID *)element.c_str(), length * sizeof(effTCHAR));
		}
	}
	else
	{
		effULONG vectorOffset = reinterpret_cast<effULONG>((effVOID *)&datas[0]) - reinterpret_cast<effULONG>((effVOID *)&datas);
		effVOID * source = (effVOID *)((effBYTE *)baseAddress + property->GetOffset() + vectorOffset);
		file->Write(source, property->GetSize() * datas.size());
	}

}

template<typename PropertyType>
inline void SaveVectorProperty(EFFFile * file, effVOID * baseAddress, EFFProperty * property)
{
	VECTOR<PropertyType> & datas = *((VECTOR<PropertyType> *)((effBYTE *)baseAddress + property->GetOffset()));
	VECTOR<PropertyType>::iterator it = datas.begin();

	effUINT size = datas.size();
	file->Write(&size, 4);

	for ( effUINT i = 0; i < datas.size(); i++ )
	{
		PropertyType & element = datas[i];
		element.SaveToFile(file);
	}
}

template<typename PropertyType>
inline void SavePointerVectorProperty(EFFFile * file, effVOID * baseAddress, EFFProperty * property)
{
	VECTOR<PropertyType *> & datas = *((VECTOR<PropertyType *> *)((effBYTE *)baseAddress + property->GetOffset()));
	VECTOR<PropertyType *>::iterator it = datas.begin();

	effUINT size = datas.size();
	file->Write(&size, 4);


	for ( effUINT i = 0; i < datas.size(); i++ )
	{
		PropertyType * element = datas[i];
		element->SaveToFile(file);
	}		

}*/



/*template<typename T>
inline void SaveProperty(T & data,ArgWriteBin * pArgWriteBin,boost::true_type)
{
	pArgWriteBin->pFile->Write(&data,sizeof(T));
};



template<typename T>
inline void SaveProperty(T & data,ArgWriteBin * pArgWriteBin,boost::false_type)
{
	data.SaveToFile(pArgWriteBin->pFile);
};


inline void SaveProperty(std::string & data,ArgWriteBin * pArgWriteBin,boost::false_type)
{
	unsigned int dwLength = data.length();
	pArgWriteBin->pFile->Write(&dwLength,4);
	pArgWriteBin->pFile->Write((void *)data.c_str(),dwLength);
};





template<typename T>
inline void SaveProperty(VECTOR<T> & data,ArgWriteBin * pArgWriteBin,boost::false_type)
{
	pArgWriteBin->pFile->Write((void *)&data.at(0),sizeof(T) * data.size());
};





template<typename T>
inline void SaveProperty(VECTOR<T> & data,ArgWriteBin * pArgWriteBin,boost::true_type)
{
	unsigned int dwSize = data.size();
	pArgWriteBin->pFile->Write(&dwSize,4);
	pArgWriteBin->pFile->Write((void *)&data.at(0),sizeof(T) * data.size());
};

inline void SaveProperty(VECTOR<std::string> & data,ArgWriteBin * pArgWriteBin,boost::false_type)
{
	unsigned int dwSize = data.size();
	pArgWriteBin->pFile->Write(&dwSize,4);
	for ( unsigned int i = 0; i < data.size(); i++ )
	{
		SaveProperty(data[i],pArgWriteBin,boost::false_type());
	}
};


//指针类型

template<typename T,typename TN>
inline void SaveProperty(T * data,TN elementNum,ArgWriteBin * pArgWriteBin,boost::true_type)
{
	//保证元素个数的类型是整型
	BOOST_STATIC_ASSERT(boost::is_integral<TN>::value);

	//保证数组里的元素类型不是指针
	BOOST_STATIC_ASSERT(!boost::is_pointer<T>::value);

	pArgWriteBin->pFile->Write(data,elementNum * sizeof(T));
}

template<typename T,typename TN>
inline void SaveProperty(T * data,TN elementNum,ArgWriteBin * pArgWriteBin,boost::false_type)
{
	//保证数组元素个数的类型是整型
	BOOST_STATIC_ASSERT(boost::is_integral<TN>::value);

	//保证数组里的元素类型不是指针
	BOOST_STATIC_ASSERT(!boost::is_pointer<T>::value);

	for ( TN i = 0; i < elementNum; i++ )
	{
		data[i].SaveToFile(pArgWriteBin->pFile);
	}
}*/


//EFFBASE_END

#endif