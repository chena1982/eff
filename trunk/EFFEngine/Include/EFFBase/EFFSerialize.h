#pragma once

#include "EFFFile.h"

#include "boost\type_traits.hpp"

#define DECLARE_CLASS(c)\
private:\
	friend class boost::serialization::access;\
	template<class Archive>\
	void serialize(Archive & ar, const unsigned int version)\

#define MEMBER(m)\
ar & m;


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



template<typename T>
inline void VisitMeta(T & data,ArgWriteBin & argWriteBin,boost::true_type)
{
	argWriteBin.pFile->Write(&data,sizeof(T));
};


template<typename T>
inline void VisitMeta(T & data,ArgWriteBin & argWriteBin,boost::false_type)
{
	data.SaveToFile(argWriteBin.pFile);
};

template<typename T>
inline void VisitMeta(std::string & data,ArgWriteBin & argWriteBin,boost::false_type)
{
	unsigned int dwLength = data.length();
	argWriteBin.pFile->Write(&dwLength,4);
	argWriteBin.pFile->Write((void *)data.c_str(),dwLength);
};

template<typename T>
inline void VisitMeta(std::vector<T> & data,ArgWriteBin & argWriteBin,boost::false_type)
{
	argWriteBin.pFile->Write((void *)&data.at(0),sizeof(T) * data.size());
};



template<typename T>
inline void VisitMeta(std::vector<std::string> & data,ArgWriteBin & argWriteBin,boost::false_type)
{
	unsigned int dwSize = data.size();
	argWriteBin.pFile->Write(&dwSize,4);
	for ( unsigned int i = 0; i < data.size(); i++ )
	{
		VisitMeta<std::string>(data[i],argWriteBin,boost::false_type());
	}
};

template<typename T>
inline void VisitMeta(std::vector<T> & data,ArgWriteBin & argWriteBin,boost::true_type)
{
	unsigned int dwSize = data.size();
	argWriteBin.pFile->Write(&dwSize,4);
	argWriteBin.pFile->Write((void *)&data.at(0),sizeof(T) * data.size());
};
