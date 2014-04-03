/******************************************************************************
	created:	2008-12-10   23:29
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFGlobal.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFGlobal_2008_12_10__
#define __EFFGlobal_2008_12_10__


EFFBASE_BEGIN

class EFFObjectManager;

class EFFBASE_API EFFContext
{
	friend effVOID InitEFFContext();
	friend effVOID ReleaseEFFContext();
protected:
	EFFContext();
	~EFFContext();


public:
	inline EFFObjectManager *	GetDefaultObjectManager() { return effObjectManager; }

protected:
	effVOID AddProperty();

private:
	EFFObjectManager *	effObjectManager;
};



effVOID InitEFFContext();
effVOID ReleaseEFFContext();


EFFBASE_API EFFContext * GetEFFContext();



EFFBASE_END

#endif