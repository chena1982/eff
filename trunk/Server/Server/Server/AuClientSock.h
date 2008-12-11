#pragma once

#include <winsock2.h>

#define AUDATASIZE 1024
#define AUBUFFSIZE AUDATASIZE * 2 //4096

class AuClientSock
{
public:
	AuClientSock(void);
	~AuClientSock(void);

	// init
	bool Init();
	void close();

	static void onRecvData( char* buff, int size );
	static void onDisconnect();
	bool sendTo( char* buff, int size );
	void connectToServer( char* addr, u_short port = 20018 );

private:
	static DWORD onRecv( LPVOID lpParameter );

	static SOCKET ConnectSocket;
	static char data[2048];
	static int size;
	static bool exit_;
	HANDLE serverHandle_;
};
