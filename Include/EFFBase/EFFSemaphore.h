/******************************************************************************
created:	2017-5-11   20:33
file path:	d:\EFF\Include\EFFBase\EFFSemaphore.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFSemaphore_2017_5_11__
#define __EFFSemaphore_2017_5_11__

EFFBASE_BEGIN

class EFFBASE_API EFFSemaphore
{
public:
    EFFSemaphore();
    ~EFFSemaphore();

public:
    effBOOL Create(effLONG initCount, effLONG maxCount, const effTCHAR *  name = NULL);

    effBOOL Open(const effTCHAR * name);

	effVOID Wait();
	effVOID Release(effLONG releaseCount);
protected:
    HANDLE semaphore;
};

EFFBASE_END

#endif