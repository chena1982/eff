/******************************************************************************
	created:	2012-03-15   23:40
	file path:	d:\EFF\trunk\EFFEngine\Src\EFFBase\EFFThread.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFThread.h"

EFFBASE_BEGIN

#if (_MSC_VER >= 800)

EFFCriticalSection::EFFCriticalSection()
{
	InitializeCriticalSection(&criticalSection);
}

EFFCriticalSection::~EFFCriticalSection()
{
	DeleteCriticalSection(&criticalSection);
}

effVOID	EFFCriticalSection::Enter()
{
	EnterCriticalSection(&criticalSection);
}

effVOID	EFFCriticalSection::Leave()
{
	LeaveCriticalSection(&criticalSection);
}

#else
#endif

EFFBASE_END