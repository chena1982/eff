#include "stdafx.h"

#include "EFFWinFile.h"


EFFWinFile::EFFWinFile()
{
	m_pFile = NULL;
}

EFFWinFile::~EFFWinFile()
{
	if ( m_pFile != NULL )
	{
		fclose(m_pFile);
	}
}

bool EFFWinFile::Open(const char *szFilePath,const char *szOpenMode)
{
	fopen_s(&m_pFile,szFilePath,szOpenMode);
	if ( m_pFile != NULL )
	{
		return true;
	}
	return false;
}

unsigned int EFFWinFile::Read(void * pBuffer,unsigned int dwSize)
{
	if ( m_pFile != NULL )
	{
		return fread(pBuffer,1,dwSize,m_pFile);
	}
	return 0;
}

unsigned int EFFWinFile::Write(void * pBuffer,unsigned int dwSize)
{
	if ( m_pFile != NULL )
	{
		return fwrite(pBuffer,1,dwSize,m_pFile);
	}
	return 0;
}