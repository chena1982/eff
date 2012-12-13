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
	virtual effUINT Read(effVOID * buffer, effUINT size);
	virtual effUINT Write(effVOID * buffer, effUINT size);
	virtual effVOID Close();
	virtual effUINT Length();
protected:
private:
	FILE *		file;
};

EFFBASE_END

#endif