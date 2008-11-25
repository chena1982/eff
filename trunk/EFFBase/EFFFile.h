#pragma once;


class EFFBASE_API EFFFile
{
public:
	virtual bool Open(const char *szFilePath,const char *szOpenMode) = 0;
	virtual unsigned int Read(void * pBuffer,unsigned int dwSize) = 0;
	virtual unsigned int Write(void * pBuffer,unsigned int dwSize) = 0;
protected:
private:
};