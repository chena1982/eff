#include "Base.h"
#include "RoomManager.h"

Base::Base(void)
{
}

Base::~Base(void)
{
}

bool Base::listen( SOCKET client )
{
	AuListener::listen( client, "", 0 );
	return true;
}


void Base::onRecvData( char* buff, int size )
{
	
}