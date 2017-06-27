/******************************************************************************
created:	2017-5-11   20:33
file path:	d:\EFF\Include\EFFBase\EFFSemaphore.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFSemaphore.h"



EFFBASE_BEGIN


EFFSemaphore::EFFSemaphore()
{
    semaphore = NULL;
}


EFFSemaphore::~EFFSemaphore()
{
    CloseHandle(semaphore);
}

effBOOL EFFSemaphore::Create(effLONG initCount, effLONG maxCount, const effTCHAR *  name)
{
    semaphore = CreateSemaphore(NULL, initCount, maxCount, name);

    return semaphore != NULL;
}

effBOOL EFFSemaphore::Open(const effTCHAR * name)
{
    semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, effFALSE, name);

    return semaphore != NULL;
}

effVOID EFFSemaphore::Wait()
{
	if (semaphore == NULL)
	{
		return;
	}

	WaitForSingleObject(semaphore, INFINITE);
}

effVOID EFFSemaphore::Release(effLONG releaseCount)
{
	if (semaphore == NULL)
	{
		return;
	}

	ReleaseSemaphore(semaphore, releaseCount, NULL);
}

EFFBASE_END