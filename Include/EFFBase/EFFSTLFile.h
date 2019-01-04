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

	virtual effBOOL Open(const effString & filePath, const effTCHAR * openMode);
    virtual effSIZE Read(effVOID * buffer, effSIZE size);
    virtual effSIZE Write(effVOID * buffer, effSIZE size);
	virtual effVOID Close();
	virtual effUINT Length();

	FILE * GetFile() { return file; }
protected:
private:
	FILE *		file;
};

EFFBASE_END

#endif