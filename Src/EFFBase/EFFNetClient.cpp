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

effBOOL EFFNetClient::Send(EFFObject * object, const effString & propertyName)
{
	if ( object == NULL )
	{
		return effFALSE;
	}

	effString buffer;
	object->GetPropertyJason(propertyName, buffer);


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

effVOID EFFNetClient::Shutdown()
{
	zmq_close(socket);
	zmq_ctx_destroy(context);
}

EFFBASE_END