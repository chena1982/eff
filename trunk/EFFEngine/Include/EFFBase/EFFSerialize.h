/******************************************************************************
	created:	2008-12-1   22:33
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFSerialize.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFSerialize_2008_12_1__
#define __EFFSerialize_2008_12_1__



#include "EFFFile.h"
#include "EFFProperty.h"

#include <boost\type_traits.hpp>
#include <boost\static_assert.hpp>

EFFBASE_BEGIN



//struct ArgWriteXml { TiXmlNode *node; String filename; Dword fileVersion; Dword classVersion; };
//struct ArgReadXml { TiXmlNode *node; String filename; Dword fileVersion; Dword classVersion; XmlFormat format; };

struct ArgWriteBin
{
	EFFFile *			pFile;
	std::string			strFileName;
	unsigned int		dwFileVersion;
	unsigned int		dwClassVersion;
};

struct ArgReadBin
{
	EFFFile *			pFile;
	std::string			strFileName;
	unsigned int		dwFileVersion;
	unsigned int		dwClassVersion;
	//BinFormat format;
};


inline void SaveStringProperty(EFFFile * file, effVOID * baseAddress, EFFProperty * property)
{
	effString & data = *((effString *)((effBYTE *)baseAddress + property->GetOffset()));
	effUINT length = data.length();
	file->Write(&length, 4);
	file->Write((effVOID *)data.c_str(), length * sizeof(effTCHAR));
}

inline void SavePODProperty(EFFFile * file, effVOID * baseAddress, EFFProperty * property)
{
	effVOID * source = (effVOID *)((effBYTE *)baseAddress + property->GetOffset());
	file->Write(source, property->GetSize());
}

template<typename T>
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
inline void SaveProperty(std::vector<T> & data,ArgWriteBin * pArgWriteBin,boost::false_type)
{
	pArgWriteBin->pFile->Write((void *)&data.at(0),sizeof(T) * data.size());
};





template<typename T>
inline void SaveProperty(std::vector<T> & data,ArgWriteBin * pArgWriteBin,boost::true_type)
{
	unsigned int dwSize = data.size();
	pArgWriteBin->pFile->Write(&dwSize,4);
	pArgWriteBin->pFile->Write((void *)&data.at(0),sizeof(T) * data.size());
};

inline void SaveProperty(std::vector<std::string> & data,ArgWriteBin * pArgWriteBin,boost::false_type)
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
}


EFFBASE_END

#endif