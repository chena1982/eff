#pragma once

#include <winsock2.h>
#include <vector>
#include <string>
#include <hash_map>

#define AULINKCOUNT 2048

#define AUDATASIZE 1024
#define AUBUFFSIZE AUDATASIZE * 2 //4096
//#define AUDATASIZEEX AUDATASIZE+4

struct SOCKBUFF
{
	SOCKBUFF()
	{
		size = 0;
	}
	char data[AUBUFFSIZE];
	int size;
};
class AuServerSock;

class AuListener
{
public:
	AuListener();
	~AuListener();

	bool listen( SOCKET client, const char* addrIP, AuServerSock* server );

	void onRecvData( char* buff, int size );
		
	bool doing();

	bool sendTo( char* buff, int size );	

	void close();

	DWORD onRecv( LPVOID lpParameter );
	
	SOCKET listener;
private:
	HANDLE serverHandle_;
	
	char addr[15];
	bool exitThr_;
	DWORD outTime_;
	SOCKBUFF data_;
	AuServerSock* server_;
};

class AuServerSock
{
public:
	AuServerSock(void);
	~AuServerSock(void);

	// init
	bool Init( u_short port = 20018 );

	bool doing();

	void close();

	//virtual void onRecvData( SOCKET client, char* buff, int size );
	virtual void onConnect( SOCKET client );
	virtual void onDisconnect( SOCKET client );

	static DWORD onListen_( LPVOID lpParameter );

private:
	static stdext::hash_map<SOCKET, int> map_;
	static AuListener* clients_[AULINKCOUNT];
	static int curr;
	
	static SOCKET server_;
	static HANDLE serverHandle_;
	static bool exitThr_;
};
