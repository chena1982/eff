#include "AuClientSock.h"
#include "windows.h"

void main()
{
	AuClientSock clientSock;

	if(!clientSock.Init())
		return;

	clientSock.connectToServer( "127.0.0.1" );

	for( int i = 0; i < 5; ++i)
	{
		clientSock.sendTo( "test", 4 );
	};
	while( true )
	{
		Sleep(100);
	}
	clientSock.close();

}