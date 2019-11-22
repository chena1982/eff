/******************************************************************************
	created:	2012-03-15   23:40
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFBase\EFFThread.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFThread_2012_03_15__
#define __EFFThread_2012_03_15__


EFFBASE_BEGIN

#if (_MSC_VER >= 800)

class EFFBASE_API EFFCriticalSection
{
public:
	EFFCriticalSection();
	~EFFCriticalSection();
public:
	effVOID		Enter();
	effVOID		Leave();
protected:
	CRITICAL_SECTION	criticalSection;
};

class EFFBASE_API EFFThread
{
public:
	EFFThread();
	~EFFThread();


};

#else
#endif




EFFBASE_END


#endif