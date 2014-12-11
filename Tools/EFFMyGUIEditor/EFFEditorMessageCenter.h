#pragma once


namespace EFFBase
{
	class EFFNetServer;
}

effVOID InitServer();
EFFNetServer * GetServer();
effVOID ReceiveMsg(effFLOAT elapsedTime);
effVOID ShutdownServer();



effVOID SendWindowPosAndSize();