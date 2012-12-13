// EFFAssetServer.cpp : 定义控制台应用程序的入口点。
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

