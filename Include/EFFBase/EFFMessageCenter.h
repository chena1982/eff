/******************************************************************************
	created:	2012-03-19   23:20
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFBase\EFFMessageCenter.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFEvent_2012_03_19__
#define __EFFEvent_2012_03_19__

#include "EFFThread.h"
#include "EFFFastIdMap.h"

EFFBASE_BEGIN

class EFFEventCall;

class EFFThreadMessageQueue
{
public:
	EFFThreadMessageQueue();
	~EFFThreadMessageQueue();

public:
	effVOID	PostMessage(EFFEventCall * message);
	effVOID	ProcessMessage(const effCHAR * message);
protected:
	EFFFastIdMap<EFFEventCall *> messages;
};

class EFFMessageCenter
{

public:
	EFFMessageCenter();
	virtual ~EFFMessageCenter();
public:
	EFFThreadMessageQueue * GetCurrentThreadMessageQueue();
protected:
	MAP<effUINT, EFFThreadMessageQueue *>		allThreadMessages;
	//EFFCriticalSection								threadCriticalSection;


};

EFFBASE_END


#endif