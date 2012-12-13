/******************************************************************************
	created:	2012-03-19   23:20
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFBase\EFFMessageCenter.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFMessageCenter.h"

#include "EFFEvent.h"

EFFBASE_BEGIN


EFFThreadMessageQueue::EFFThreadMessageQueue()
{
}

EFFThreadMessageQueue::~EFFThreadMessageQueue()
{
}

effVOID	EFFThreadMessageQueue::PostMessage(EFFEventCall * message)
{
}


EFFMessageCenter::EFFMessageCenter()
{
}


EFFMessageCenter::~EFFMessageCenter()
{
}

EFFThreadMessageQueue * EFFMessageCenter::GetCurrentThreadMessageQueue()
{
	
	//threadCriticalSection.Enter();

	effUINT threadId = GetCurrentThreadId();
	std::map<effUINT, EFFThreadMessageQueue *>::iterator it = allThreadMessages.find(threadId);
	if ( it != allThreadMessages.end() )
	{
		return it->second;
	}

	EFFThreadMessageQueue * queue = new EFFThreadMessageQueue();
	allThreadMessages[threadId] = queue;

	//threadCriticalSection.Leave();

	return queue;

}

EFFBASE_END

