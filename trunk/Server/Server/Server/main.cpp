#include "AuServerSock.h"
#include "windows.h"


void main()
{
	AuServerSock server;

	if( !server.Init())
		return;

	printf("listening.......\n");
	while( server.doing() )
	{
	}

	server.close();
}