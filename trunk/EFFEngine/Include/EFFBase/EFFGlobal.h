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

class EFFBASE_API EFFGlobal
{
	friend class EFFGlobalUnExport;
protected:
	EFFGlobal();
	~EFFGlobal();
public:
	EFFObjectManager *				GetObjectManager() { return m_pEFFObjectManager; }
private:
	EFFObjectManager *				m_pEFFObjectManager;
};

class EFFGlobalUnExport
{
public:
	static EFFGlobal effGlobal;
};

EFFBASE_API EFFGlobal & GetEFFGlobal();


EFFBASE_END

#endif