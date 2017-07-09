#include "EFFBasePCH.h"



#include "EFFObject.h"
#include "EFFProperty.h"
#include "EFFNetClient.h"
#include "EFFScopeGuard.h"




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
    socket = zmq_socket(context, ZMQ_PAIR);

}

effBOOL EFFNetClient::Connect(effString address)
{
	if ( zmq_connect(socket, EFFSTRING2ANSI(address)) == -1 )
	{
		//std::string error = zmq_strerror(zmq_errno());
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
	ON_SCOPE_EXIT([&] { zmq_msg_close(&msg); })
	zmq_msg_init_data(&msg, (effVOID *)buffer.c_str(), buffer.size()+1, NULL, NULL);
	
	if ( zmq_msg_send(&msg, socket, 0) == -1 )
	{
		//std::string error = zmq_strerror(zmq_errno());
		return effFALSE;
	}

	return effTRUE;
}

effBOOL EFFNetClient::ReceiveMsg(effVOID * buffer, effINT size)
{
	effINT receiveSize = zmq_recv(socket, buffer, size, ZMQ_DONTWAIT);
	if ( receiveSize > 0 && receiveSize <= size )
	{
		return effTRUE;
	}

	return effFALSE;
}

effBOOL EFFNetClient::SendCmd(effINT id)
{
	if (zmq_send(socket, &id, sizeof(effINT), ZMQ_DONTWAIT) == -1)
	{
		int error = zmq_errno();
		int count = 0;
		while (error == EAGAIN && count < 10)
		{
			if (zmq_send(socket, &id, sizeof(effINT), ZMQ_DONTWAIT) == -1)
			{
				error = zmq_errno();
			}
			else
			{
				return effTRUE;
			}

			count++;
		}

		//std::string errorText = zmq_strerror(error);
		return effFALSE;
	}

	return effTRUE;
}

effBOOL EFFNetClient::SendMsg(effINT id, effVOID * buffer, effINT size)
{
	if (zmq_send(socket, buffer, size, ZMQ_DONTWAIT) == -1)
	{
		int error = zmq_errno();
		int count = 0;
		while (error == EAGAIN && count < 10)
		{
			if (zmq_send(socket, buffer, size, ZMQ_DONTWAIT) == -1)
			{
				error = zmq_errno();
			}
			else
			{
				return effTRUE;
			}

			count++;
		}

		//std::string errorText = zmq_strerror(error);
		return effFALSE;
	}

	return effTRUE;
}

effVOID EFFNetClient::Shutdown()
{
	zmq_close(socket);
	zmq_ctx_destroy(context);
}

EFFBASE_END