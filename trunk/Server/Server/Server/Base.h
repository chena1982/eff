#pragma once
#include "AuServerSock.h"

class Base : public AuListener
{
public:
	Base(void);
	~Base(void);
	
	//
	bool listen( SOCKET client );

	static void onRecvData( char* buff, int size );
};
