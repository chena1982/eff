/******************************************************************************
	created:	2008-12-1   23:20
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFSTLFile.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFSTLFile.h"

//#define new EFFNEW

EFFBASE_BEGIN

EFFSTLFile::EFFSTLFile()
{
	file = NULL;
}

EFFSTLFile::~EFFSTLFile()
{
	if ( file != NULL )
	{
		fclose(file);
	}
}

effBOOL EFFSTLFile::Open(const effString & filePath, const effTCHAR * openMode)
{

	_wfopen_s(&file, filePath.c_str(), openMode);

	if ( file != NULL )
	{
		return effTRUE;
	}
	return effFALSE;
}

effSIZE EFFSTLFile::Read(effVOID * buffer, effSIZE size)
{

	return fread(buffer, size, 1, file);
}

effSIZE EFFSTLFile::Write(effVOID * buffer, effSIZE size)
{
	return fwrite(buffer, size, 1, file);
}

effVOID EFFSTLFile::Close()
{
	if ( file != NULL )
	{
		fclose(file);
		file = NULL;
	}
}

effUINT EFFSTLFile::Length()
{
	int n = _fileno(file);  
	//return _filelength(n);
	return 0;
}

EFFBASE_END