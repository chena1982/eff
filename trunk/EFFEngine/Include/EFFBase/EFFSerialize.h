#pragma once

#include "EFFFile.h"

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
inline void VisitMeta(T & data,ArgWriteBin & argWriteBin)
{
	argWriteBin.pFile->Write(&data,sizeof(T));
};
