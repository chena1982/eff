#include "stdio.h"
#include "AuServerSock.h"

//
//SOCKET AuListener::listener;
//char AuListener::addr[15];
//bool AuListener::exitThr_;
//SOCKBUFF AuListener::data_;
//AuServerSock* AuListener::server_;
//HANDLE AuListener::serverHandle_;

DWORD onRecvStatic( LPVOID lpParameter )
{
	AuListener* obj = (AuListener*)lpParameter;
	if( obj )
	{
		obj->onRecv( obj );
	}
	return 0;
}

AuListener::AuListener()
{
	listener = 0;
	exitThr_ = false;
	data_.size = 0;
	serverHandle_ = 0;
	outTime_ = 0;
	ZeroMemory( data_.data, AUBUFFSIZE );
}

AuListener::~AuListener()
{
	closesocket( listener );
}

bool AuListener::listen( SOCKET client, const char* addrIP, AuServerSock* server )
{
	listener = client;
	strcpy( addr, addrIP );
	server_ = server;
	exitThr_ = false;
	outTime_ = timeGetTime();
	CreateThread(0,0, (LPTHREAD_START_ROUTINE)onRecvStatic, this, 0, 0 );
	return true;
}

void AuListener::onRecvData( char* buff, int size )
{
	printf( "recv %s\n", buff );
}

bool AuListener::doing()
{
	// 如果超时就断线
	if( timeGetTime() - outTime_ > 10000 && outTime_ > 0)
	{
		closesocket( listener );
		if( server_ )
			server_->onDisconnect( listener );
		server_ = 0;
		printf( "onDisconnect %s \n", addr );
		return false;
	}
	return true;
}

DWORD AuListener::onRecv( LPVOID lpParameter )
{
	char buf[AUDATASIZE];

	// recv data
	ZeroMemory( buf, AUDATASIZE );
	buf[0] = '@';
	buf[1] = '!';	
	int br = recv( listener, buf, AUDATASIZE, 0 );
	
	outTime_ = timeGetTime();
	if( 0 >= br )
	{		
		printf( "onDisconnect %s \n", addr );
		if( server_ )
			server_->onDisconnect( listener );
		closesocket( listener );				
		return 0;	
	}		

	if(buf[0] != '@' && buf[1] != '!')
	{
		int s = data_.size;
		long header = 0;
		long dataSize = 0;
		ZeroMemory( &data_.data[s], AUBUFFSIZE - s );
		memcpy( &data_.data[s], &buf, AUDATASIZE );
		
		int calcuSize = AUDATASIZE + s; 
		for( int j = 0; j < AUBUFFSIZE; )
		{	
			header = *(DWORD*)&data_.data[j];
			if( j + 4 > AUBUFFSIZE && header != 0 )
			{
				int lsize = AUBUFFSIZE - j;
				memcpy( data_.data, &data_.data[j], lsize );
				data_.size = lsize;
				break;
			}
			if( j + 4 > calcuSize && header != 0 )
			{
				int lsize = calcuSize - j;
				memcpy( data_.data, &data_.data[j], lsize );
				data_.size = lsize;
				break;
			}
			
			if( header == 10031 )
			{
				if( j + 8 > AUBUFFSIZE )
				{
					int lsize = AUBUFFSIZE - j;
					memcpy( data_.data, &data_.data[j], lsize );
					data_.size = lsize;
					break;
				}
				if( j + 8 > calcuSize )
				{
					int lsize = calcuSize - j;
					memcpy( data_.data, &data_.data[j], lsize );
					data_.size = lsize;
					break;
				}

				dataSize = *(DWORD*)&data_.data[j+4];
			
				if( j + 8 + dataSize > AUBUFFSIZE )
				{
					int lsize = AUBUFFSIZE - j;
					memcpy( data_.data, &data_.data[j], lsize );
					data_.size = lsize;
					break;
				}

				if( j + 8 + dataSize > calcuSize )
				{
					int lsize = calcuSize - j;
					memcpy( data_.data, &data_.data[j], lsize );
					data_.size = lsize;
					break;
				}
				
				j += 8;
				char data[AUDATASIZE];
				memcpy( data, &data_.data[j], dataSize );
				data[dataSize] = '\0';
				onRecvData( data, dataSize );
				j += dataSize;
				data_.size = 0;
			
				continue;
			}else
			{					
				break;
			}
		}
	}

	if( exitThr_ )
		return 0;

	CreateThread(0,0, (LPTHREAD_START_ROUTINE)onRecvStatic, 0, 0, 0 );
	return 0;
}

void AuListener::close()
{
	exitThr_ = true;
}

bool AuListener::sendTo( char* buff, int size )
{
	if( size > 2046 )
		return false;
	
	char data[2048];

	DWORD header = 10031;
	memcpy( data, &header, sizeof(DWORD) );

	memcpy( &data[4], &size, sizeof(int) );

	memcpy( &data[8], buff, size );

	send( listener, data, size + 8, 0 );
	return true;
}

AuListener* AuServerSock::clients_[AULINKCOUNT];
SOCKET AuServerSock::server_;
HANDLE AuServerSock::serverHandle_;
bool AuServerSock::exitThr_;
int AuServerSock::curr = 0;
stdext::hash_map<SOCKET, int> AuServerSock::map_;

AuServerSock::AuServerSock(void)
{
	exitThr_ = false;
	for( int i = 0; i < AULINKCOUNT;++i )
	{		
		clients_[i] = 0;
	}
	curr = 0;
}

AuServerSock::~AuServerSock(void)
{
}

// init
bool AuServerSock::Init( u_short port )
{
	WSADATA wsaData;

	sockaddr_in local;

	int wsaret=WSAStartup(0x101,&wsaData);
	if(wsaret!=0)
    {
        return false;
    }

	local.sin_family=AF_INET; // 地址族
    local.sin_addr.s_addr=INADDR_ANY; // 网际IP地址
    local.sin_port=htons((u_short)port); // 使用的端口
	server_ = socket( AF_INET,SOCK_STREAM, 0 );

	if( server_ == INVALID_SOCKET )
    {
        return false;
    }

	if( bind( server_,(sockaddr*)&local,sizeof(local)) != 0 )
    {
        return false;
    }

	if( listen( server_, 10 ) != 0 )
    {
        return false;
    }

	exitThr_ = false;	
	return true;
}

DWORD AuServerSock::onListen_( LPVOID lpParameter )
{
	while( exitThr_ ){Sleep(10);}
	exitThr_ = true;
	SOCKET client;
	sockaddr_in from;
    int fromlen = sizeof(from);
	
	// accept()将会接收即将到来的客户端连接。
	client = accept( server_,(sockaddr*)&from, &fromlen);

	std::string addr = inet_ntoa(from.sin_addr);
	AuListener* pClient = new AuListener;
	
	pClient->listen( client, addr.c_str(), (AuServerSock*)lpParameter );
	clients_[curr] = pClient;
	map_[client] = curr;
	curr++;
	if( curr == AULINKCOUNT )
		curr = 0;

	char temp[512];
	sprintf(temp,"link IP is %s\r\n", addr.c_str() );
	printf( "%s \n", temp );

	AuServerSock* obj = (AuServerSock*)lpParameter;
	obj->onConnect( client );
	serverHandle_ = 0;
	exitThr_ = false;
	return 0;
}

bool AuServerSock::doing()
{	
	if( !serverHandle_ && !exitThr_ )
	{
		serverHandle_ = CreateThread(0,0, (LPTHREAD_START_ROUTINE)onListen_, this, 0, 0 );
	}
	for( int i = 0; i < AULINKCOUNT; ++i )
	{
		if(clients_[i])
		{
			clients_[i]->doing();
		}
	}
	return true;
}

void AuServerSock::close()
{
	CloseHandle( serverHandle_ );

	for( int i = 0; i < AULINKCOUNT;++i )
	{
		if(clients_[i])
			delete clients_[i];
	}
	
	// closesocket()关闭套接字，并释放套接字描述符。
    closesocket(server_);

    // 最初这个函数也许有些用处，现在保留它只是为了向后兼容。
    // 但是调用它可能会更安全，因为我相信某些实现会使用它来结束WS2_32.DLL的使用。
    WSACleanup();
}


void AuServerSock::onConnect( SOCKET client )
{
}

void AuServerSock::onDisconnect( SOCKET client )
{
	stdext::hash_map<SOCKET, int>::iterator iter = map_.find( client );
	if( iter != map_.end() )
	{
		int index = (*iter).second;
		clients_[index] = 0;
		map_.erase( iter );
	}
}