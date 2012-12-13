#include "stdafx.h"
#include "EFFNetServer.h"



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
	socket = zmq_socket(context, ZMQ_REP);


}

effBOOL EFFNetServer::Bind(effString address)
{
	if ( zmq_bind(socket, EFFSTRING2ANSI(address)) == -1 )
	{
		effINT error = zmq_errno();
		return effFALSE;
	}

	return effTRUE;
}

effBOOL EFFNetServer::ReceiveMsg()
{
    zmq_msg_t msg;
    zmq_msg_init(&msg);
    effINT size = zmq_msg_recv(&msg, socket, ZMQ_DONTWAIT);
	if ( size > 0 )
	{
		effCHAR * buffer = (effCHAR *)zmq_msg_data(&msg);
		effINT z = 0;
	}
    zmq_msg_close(&msg);

	return effTRUE;
}

effVOID EFFNetServer::Shutdown()
{
	zmq_close(socket);
	zmq_ctx_destroy(context);
}

EFFBASE_END