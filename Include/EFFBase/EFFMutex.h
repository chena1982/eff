#pragma once


EFFBASE_BEGIN

class EFFBASE_API EFFMutex
{
public:
	EFFMutex();
	~EFFMutex();

public:
	effBOOL Create(const effTCHAR *  name);

	effBOOL Open(const effTCHAR * name);

	effVOID Wait();
	effVOID Release();

	effVOID Lock();
	effVOID Unlock();
protected:
	HANDLE mutex;
};

EFFBASE_END