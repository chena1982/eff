#include "stdafx.h"



#include "EFFObject.h"
#include "EFFProperty.h"
#include "EFFNetClient.h"





EFFBASE_BEGIN

EFFNetClient::EFFNetClient()
{
	context = NULL;
	socket = NULL;
}

EFFNetClient::~EFFNetClient()
{
}

effVOID EFFNetClient::Init()
{
	context = zmq_ctx_new();
    socket = zmq_socket(context, ZMQ_REQ);

}

effBOOL EFFNetClient::Connect(effString address)
{
	if ( zmq_connect(socket, EFFSTRING2ANSI(address)) == -1 )
	{
		effINT error = zmq_errno();
		return effFALSE;
	}

	return effTRUE;
}

effBOOL EFFNetClient::Send(EFFObject * object, effString & propertyName)
{


	Json::Value root;
	root["encoding"] = "UTF-8";
	root["object_type_name"] = EFFSTRING2ANSI(object->GetRuntimeClass()->GetName());
	root["object_id"] = object->GetID();
	
	Json::Value objectProperty;
	AddPropertyToJson(object, propertyName, &objectProperty);
	root["object_property"] = objectProperty;

	std::string & buffer = root.toStyledString();


	zmq_msg_t msg;
	zmq_msg_init_data(&msg, (effVOID *)buffer.c_str(), buffer.size()+1, NULL, NULL);
	
	if ( zmq_msg_send(&msg, socket, 0) == -1 )
	{
		effINT error = zmq_errno();
		zmq_msg_close(&msg);
		return effFALSE;
	}

	zmq_msg_close(&msg);
	//zmq_send(socket, buffer.c_str(), buffer.size(), 0);


	return effTRUE;
}

effVOID EFFNetClient::AddPropertyToJson(EFFObject * object, effString & propertyName, Json::Value * jsonValue)
{
	EFFProperty * addedProperty = object->GetProperty(propertyName);

	if ( addedProperty == NULL )
	{
		return;
	}

	if ( addedProperty->GetClass()->IsPODType() )
	{
		if ( addedProperty->GetIsSTLContainer() )
		{

			effString propertyClassName = addedProperty->GetClass()->GetName();
			if ( propertyClassName == _effT("effString") )
			{
				//Json::Value elements;
				addedProperty->ForEach<effString>(object, [=, this](const effString & propertyValue)
													{
														(*jsonValue)[EFFSTRING2ANSI(propertyName)].append(EFFSTRING2ANSI(propertyValue));
													});

				//(*root)[EFFSTRING2ANSI(propertyName)] = elements;
			}

		}
		else
		{
			effString propertyClassName = addedProperty->GetClass()->GetName();
			if ( propertyClassName == _effT("effBOOL") )
			{
				effBOOL propertyValue;
				addedProperty->GetValue(object, propertyValue);
				(*jsonValue)[EFFSTRING2ANSI(propertyName)] = propertyValue;
			}
			else if ( propertyClassName == _effT("effString") )
			{
				effString propertyValue;
				addedProperty->GetValue(object, propertyValue);
				(*jsonValue)[EFFSTRING2ANSI(propertyName)] = EFFSTRING2ANSI(propertyValue);
			}

		}
	}
}

effVOID EFFNetClient::Shutdown()
{
	zmq_close(socket);
	zmq_ctx_destroy(context);
}

EFFBASE_END