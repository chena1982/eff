#pragma once
#include "AuServerSock.h"
#include "cstdmf/pch.hpp"

class LoginServer: public AuServerSock, public Singleness<LoginServer>
{
public:
	LoginServer(void);
	~LoginServer(void);

	// init loginServer
	bool Init( u_short port = 20018 );

	virtual void onConnect( SOCKET client );
	virtual void onDisconnect( SOCKET client );
};
