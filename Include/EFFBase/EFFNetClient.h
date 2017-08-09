/******************************************************************************
	created:	2012-08-27   22:10
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFNetClient.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFNetClient_2012_08_27__
#define __EFFNetClient_2012_08_27__


//class Json::Value;

EFFBASE_BEGIN

class EFFObject;
class EFFProperty;


class EFFBASE_API EFFNetClient
{
public:
	EFFNetClient();
	~EFFNetClient();

public:
	effVOID Init();
	effBOOL Connect(effString address);
	effBOOL Send(EFFObject * object, const effString & propertyName);
	effBOOL ReceiveMsg(effVOID * buffer, effINT size);
	effBOOL SendCmd(effINT id);
	effBOOL SendMsg(effINT id, effVOID * buffer, effINT size);
	effVOID Shutdown();
protected:


protected:
	void * context;
	void * socket;
};

EFFBASE_END

#endif