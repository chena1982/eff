// EFFAssetServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	EFFNetServer * server = new EFFNetServer();
	server->Init();
	server->Bind(_effT("tcp://*:5555"));

	while ( true )
	{
		server->ReceiveMsg();

		Sleep(1000);
	}

	return 0;
}

