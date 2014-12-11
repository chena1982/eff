/******************************************************************************
	created:	2012-10-12   21:33
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFNetServer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFNetServer_2012_00_12__
#define __EFFNetServer_2012_00_12__


EFFBASE_BEGIN

class EFFBASE_API EFFNetServer
{
public:
	EFFNetServer();
	~EFFNetServer();
public:
	effVOID Init();
	effBOOL Bind(effString address);
	effBOOL ReceiveMsg();
	effBOOL ReceiveMsg(effVOID * buffer, effINT size);
	effBOOL SendCmd(effINT id);
	effBOOL SendMsg(effINT id, effVOID * buffer, effINT size);
	effVOID Shutdown();

protected:
	void * context;
	void * socket;

};

EFFBASE_END


#endif