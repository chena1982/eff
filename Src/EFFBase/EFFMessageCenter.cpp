/******************************************************************************
	created:	2012-03-19   23:20
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFBase\EFFMessageCenter.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFMessageCenter.h"
#include "EFFEvent.h"
#include "EFFObjectManager.h"

EFFBASE_BEGIN


EFFThreadMessageQueue::EFFThreadMessageQueue() : messages(100)
{
}

EFFThreadMessageQueue::~EFFThreadMessageQueue()
{
}

effVOID	EFFThreadMessageQueue::PostMessage(EFFEventCall * message)
{
}

effVOID	EFFThreadMessageQueue::ProcessMessage(const effCHAR * message)
{
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(message, root);

	if ( !parsingSuccessful )
	{
		return;
	}

	if ( root["message_type"] == "Event" )
	{
	}
	else
	{
		EFFClass * objectClass = EFFGetClass(ANSI2EFFSTRING(root["object_type"].asCString()));
		EFFObjectManager * objectManager = EFFGetObjectManager(objectClass);

		if ( objectManager != NULL )
		{
			effINT id = root["object_id"].asInt();
			EFFObject * object = objectManager->GetObject(id);

			if ( object != NULL )
			{
				if ( root["message_type"] == "ObjectFunction" )
				{
				}
				else if ( root["message_type"] == "ObjectPropertices" )
				{
					for ( effUINT i = 0; i < root["object_propertices"].size(); i++ )
					{
						const Json::Value & propertyValue = root["object_propertices"][i];
						EFFProperty * objectProperty = objectClass->GetProperty(ANSI2EFFSTRING(propertyValue["property_name"].asCString()));
						if ( objectProperty != NULL )
						{
						}
					}
				}

			}
		}
	}
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

