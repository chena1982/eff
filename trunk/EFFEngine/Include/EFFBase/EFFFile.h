/******************************************************************************
	created:	2008-12-1   22:30
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFFile.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFFile_2008_12_1__
#define __EFFFile_2008_12_1__

EFFBASE_BEGIN

class EFFBASE_API EFFFile
{
public:
	virtual bool Open(const char *szFilePath,const char *szOpenMode) = 0;
	virtual unsigned int Read(void * pBuffer,unsigned int dwSize) = 0;
	virtual unsigned int Write(void * pBuffer,unsigned int dwSize) = 0;
protected:
private:
};

EFFBASE_END

#endif