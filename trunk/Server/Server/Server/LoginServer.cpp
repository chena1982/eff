#include "LoginServer.h"
#include "RoomManager.h"


DEF_SINGLENESS(LoginServer)

LoginServer::LoginServer(void)
{
}

LoginServer::~LoginServer(void)
{
}

// init loginServer
bool LoginServer::Init( u_short port )
{
	return AuServerSock::Init( port );
}


void LoginServer::onConnect( SOCKET client )
{
}

void LoginServer::onDisconnect( SOCKET client )
{
}