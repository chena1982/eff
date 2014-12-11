#include "stdafx.h"
#include "EFFNetServer.h"

#include "EFFScopeGuard.h"

EFFBASE_BEGIN


EFFNetServer::EFFNetServer()
{
	context = NULL;
	socket = NULL;
}

EFFNetServer::~EFFNetServer()
{
}

effVOID EFFNetServer::Init()
{
	context = zmq_ctx_new();

	//Socket to talk to clients
	socket = zmq_socket(context, ZMQ_PAIR);


}

effBOOL EFFNetServer::Bind(effString address)
{
	if ( zmq_bind(socket, EFFSTRING2ANSI(address)) == -1 )
	{
		std::string error = zmq_strerror(zmq_errno());
		return effFALSE;
	}

	return effTRUE;
}

effBOOL EFFNetServer::ReceiveMsg()
{
	effBYTE buffer[256];
    effINT receiveSize = zmq_recv(socket, buffer, 256, ZMQ_DONTWAIT);
	if ( receiveSize > 0 )
	{
		printf("%s", buffer);
	}

	return effTRUE;
}

effBOOL EFFNetServer::ReceiveMsg(effVOID * buffer, effINT size)
{
    effINT receiveSize = zmq_recv(socket, buffer, size, ZMQ_DONTWAIT);
	if ( receiveSize > 0 && receiveSize <= size )
	{
		return effTRUE;
	}

	return effFALSE;
}

effBOOL EFFNetServer::SendCmd(effINT id)
{
	if ( zmq_send(socket, &id, sizeof(effINT), 0) == -1 )
	{
		std::string error = zmq_strerror(zmq_errno());
		return effFALSE;
	}

	return effTRUE;
}

effBOOL EFFNetServer::SendMsg(effINT id, effVOID * buffer, effINT size)
{
	if ( zmq_send(socket, buffer, size, 0) == -1 )
	{
		std::string error = zmq_strerror(zmq_errno());
		return effFALSE;
	}

	return effTRUE;
}

effVOID EFFNetServer::Shutdown()
{
	zmq_close(socket);
	zmq_ctx_destroy(context);
}

EFFBASE_END