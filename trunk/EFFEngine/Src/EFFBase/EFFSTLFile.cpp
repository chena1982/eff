/******************************************************************************
	created:	2008-12-1   23:20
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFSTLFile.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFSTLFile.h"

#define new EFFNEW

EFFBASE_BEGIN

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
		return fread_s(pBuffer,dwSize,1,dwSize,m_pFile);
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

void EFFSTLFile::Close()
{
	if ( m_pFile != NULL )
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

unsigned int EFFSTLFile::Length()
{
	if ( m_pFile != NULL )
	{
		int n = _fileno(m_pFile);  
		return _filelength(n);
	}
	return 0;
}

EFFBASE_END