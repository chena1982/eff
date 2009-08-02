/******************************************************************************
	created:	2008-12-1   22:33
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFSTLFile.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFSTLFile_2008_12_1__
#define __EFFSTLFile_2008_12_1__



#include "EFFFile.h"


EFFBASE_BEGIN

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

EFFBASE_END

#endif