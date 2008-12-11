#include "stdio.h"
#include "AuClientSock.h"
#include <winsock2.h>

SOCKET AuClientSock::ConnectSocket;
char AuClientSock::data[2048];
int AuClientSock::size;
bool AuClientSock::exit_;

DWORD AuClientSock::onRecv( LPVOID lpParameter )
{
	char buf[AUDATASIZE];

	// recv data
	ZeroMemory( buf, AUDATASIZE );
	buf[0] = '@';
	buf[1] = '!';	
	int br = recv( ConnectSocket, buf, AUDATASIZE,0 );
	
	if( 0 >= br )
	{		
		printf( "onDisconnect\n");
		onDisconnect();
		closesocket( ConnectSocket );				
		return 0;	
	}

	if(buf[0] != '@' && buf[1] != '!')
	{
		int s = size;
		long header = 0;
		long dataSize = 0;
		ZeroMemory( &data[s], AUBUFFSIZE - s );
		memcpy( &data[s], &buf, AUDATASIZE );
		
		int calcuSize = AUDATASIZE + s; 
		for( int j = 0; j < AUBUFFSIZE; )
		{	
			header = *(DWORD*)&data[j];
			if( j + 4 > AUBUFFSIZE && header != 0 )
			{
				int lsize = AUBUFFSIZE - j;
				memcpy( data, &data[j], lsize );
				size = lsize;
				break;
			}
			if( j + 4 > calcuSize && header != 0 )
			{
				int lsize = calcuSize - j;
				memcpy( data, &data[j], lsize );
				size = lsize;
				break;
			}
			
			if( header == 10031 )
			{
				if( j + 8 > AUBUFFSIZE )
				{
					int lsize = AUBUFFSIZE - j;
					memcpy( data, &data[j], lsize );
					size = lsize;
					break;
				}
				if( j + 8 > calcuSize )
				{
					int lsize = calcuSize - j;
					memcpy( data, &data[j], lsize );
					size = lsize;
					break;
				}

				dataSize = *(DWORD*)&data[j+4];
			
				if( j + 8 + dataSize > AUBUFFSIZE )
				{
					int lsize = AUBUFFSIZE - j;
					memcpy( data, &data[j], lsize );
					size = lsize;
					break;
				}

				if( j + 8 + dataSize > calcuSize )
				{
					int lsize = calcuSize - j;
					memcpy( data, &data[j], lsize );
					size = lsize;
					break;
				}
				
				j += 8;
				char dat[AUDATASIZE];
				memcpy( dat, &data[j], dataSize );
				dat[dataSize] = '\0';
				onRecvData( dat, dataSize );
				j += dataSize;
				size = 0;
			
				continue;
			}else
			{					
				break;
			}
		}
	}

	if( exit_ )
		return 0;

	CreateThread(0,0, (LPTHREAD_START_ROUTINE)onRecv, 0, 0, 0 );
	return 0;
}

AuClientSock::AuClientSock(void)
{
	exit_ = false;
}

AuClientSock::~AuClientSock(void)
{
	exit_ = true;
}


bool AuClientSock::Init()
{
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n");

	//----------------------
	// Create a SOCKET for connecting to server  
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) 
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		//WSACleanup();
		return false;
	}
	
	exit_ = false;
	serverHandle_ = CreateThread(NULL,0, (LPTHREAD_START_ROUTINE)onRecv,NULL,0, 0 );		
	return true;
}


void AuClientSock::close()
{
	exit_ = true;
	WSACleanup();
}

void AuClientSock::onRecvData( char* buff, int size )
{
}

void AuClientSock::onDisconnect()
{
}

bool AuClientSock::sendTo( char* buff, int size )
{
	if( size > 2046 )
		return false;
	
	char data[2048];

	DWORD header = 10031;
	memcpy( data, &header, sizeof(DWORD) );

	memcpy( &data[4], &size, sizeof(int) );

	memcpy( &data[8], buff, size );

	send( ConnectSocket, data, size + 8, 0 );
	return true;
}

void AuClientSock::connectToServer( char* addr, u_short port )
{
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	sockaddr_in clientService; 
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr( addr );
	clientService.sin_port = htons( port );

	//----------------------
	// Connect to server.
	if ( connect( ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService) ) == SOCKET_ERROR) 
	{
		printf( "Failed to connect.\n" );
		WSACleanup();
		return;
	}
	printf("Connected to server.\n");
}