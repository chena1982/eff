/******************************************************************************
	created:	2008-12-1   23:20
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFMemory.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFMemory.h"

//#define new EFFNEW


#ifdef _DEBUG

	static unsigned int g_dwAllocTimes = 0;
	
	/*MemoryInfo g_MemoryInfo;

	void __cdecl _effTrace(LPCSTR lpszFormat, ...)
	{
		va_list pArgs;
		va_start(pArgs,lpszFormat);

		char ttmp[1024];

		int nBuf = ::_vsnprintf_s(ttmp,1024,lpszFormat,pArgs);


		OutputDebugStringA(ttmp);
		va_end(pArgs);
	}


	MemoryInfo::MemoryInfo()
	{
		header = NULL;
	}

	MemoryInfo::~MemoryInfo()
	{
		while ( header != NULL )
		{
			if( header->file != NULL )
			{
				_effTrace("%s(%d) : Memory leak! There have bytes memory had not be delete. Alloc ID : %d\n",header->file,header->line,header->id);
			}
			header = header->next;
		}
	}


	void * EFFMemoryNew(size_t size,const char * pszFileName,unsigned int dwLine)
	{

		AllocInfo * pMemoryInfo = (AllocInfo *)dlmalloc(sizeof(AllocInfo)+size);
		pMemoryInfo->file = pszFileName;
		pMemoryInfo->line = dwLine;
		pMemoryInfo->id = g_dwAllocTimes;
		g_dwAllocTimes++;

		pMemoryInfo->next = g_MemoryInfo.header;
		pMemoryInfo->previous = NULL;
		g_MemoryInfo.header = pMemoryInfo;

		if ( pMemoryInfo->next != NULL )
		{
			pMemoryInfo->next->previous = pMemoryInfo;
		}

		return (void *)(((char *)(pMemoryInfo)) + sizeof(AllocInfo));
	}

	void EFFMemoryDelete(void * memory,const char * pszFileName,unsigned int dwLine)
	{
		AllocInfo * pAllocInfo = (AllocInfo *)(((char *)memory) - sizeof(AllocInfo));
		if ( pAllocInfo->previous != NULL )
		{
			pAllocInfo->previous->next =  pAllocInfo->next;
		}
		if ( pAllocInfo->next != NULL )
		{
			pAllocInfo->next->previous = pAllocInfo->previous;
		}

		if ( g_MemoryInfo.header == pAllocInfo )
		{
			g_MemoryInfo.header = pAllocInfo->next;
		}

		dlfree((void *)(((char *)memory) - sizeof(AllocInfo)));
	}*/









#else

#endif


