#pragma once

#include "EFFFile.h"

class EFFBASE_API EFFSTLFile : public EFFFile
{
public:
	EFFSTLFile();
	~EFFSTLFile();
public:

	virtual bool Open(const char *szFilePath,const char *szOpenMode);
	virtual unsigned int Read(void * pBuffer,unsigned int dwSize);
	virtual unsigned int Write(void * pBuffer,unsigned int dwSize);
protected:
private:
	FILE *		m_pFile;
};