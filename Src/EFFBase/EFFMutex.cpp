

#include "EFFBasePCH.h"
#include "EFFMutex.h"



EFFBASE_BEGIN


EFFMutex::EFFMutex()
{
	mutex = NULL;
}


EFFMutex::~EFFMutex()
{
	CloseHandle(mutex);
}

effBOOL EFFMutex::Create(const effTCHAR *  name)
{

	mutex = CreateMutex(NULL, effTRUE, name);

	return mutex != NULL;
}

effBOOL EFFMutex::Open(const effTCHAR * name)
{
	mutex = OpenMutex(SEMAPHORE_ALL_ACCESS, effFALSE, name);

	return mutex != NULL;
}

effVOID EFFMutex::Wait()
{
	if (mutex == NULL)
	{
		return;
	}

	WaitForSingleObject(mutex, INFINITE);
}

effVOID EFFMutex::Release()
{
	if (mutex == NULL)
	{
		return;
	}

	ReleaseMutex(mutex);
}

effVOID EFFMutex::Lock()
{

}

effVOID EFFMutex::Unlock()
{

}

EFFBASE_END