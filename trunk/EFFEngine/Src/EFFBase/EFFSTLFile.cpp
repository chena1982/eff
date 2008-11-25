#include "stdafx.h"

#include "EFFSTLFile.h"


EFFSTLFile::EFFSTLFile()
{
	m_pFile = NULL;
}

EFFSTLFile::~EFFSTLFile()
{
	if ( m_pFile != NULL )
	{
		fclose(m_pFile);
	}
}

bool EFFSTLFile::Open(const char *szFilePath,const char *szOpenMode)
{
	fopen_s(&m_pFile,szFilePath,szOpenMode);
	if ( m_pFile != NULL )
	{
		return true;
	}
	return false;
}

unsigned int EFFSTLFile::Read(void * pBuffer,unsigned int dwSize)
{
	if ( m_pFile != NULL )
	{
		return fread(pBuffer,1,dwSize,m_pFile);
	}
	return 0;
}

unsigned int EFFSTLFile::Write(void * pBuffer,unsigned int dwSize)
{
	if ( m_pFile != NULL )
	{
		return fwrite(pBuffer,1,dwSize,m_pFile);
	}
	return 0;
}