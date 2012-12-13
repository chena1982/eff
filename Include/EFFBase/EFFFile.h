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
	virtual effBOOL Open(const effString & filePath, const effTCHAR * openMode) = 0;
	virtual effUINT Read(effVOID * buffer, effUINT size) = 0;
	virtual effUINT Write(effVOID * buffer, effUINT size) = 0;
	virtual effVOID Close() = 0;
	virtual effUINT Length() = 0;
protected:
private:
};

EFFBASE_END

#endif