// dllmain.cpp : Defines the entry point for the DLL application.
#include "EFFBasePCH.h"

#include "EFFGlobal.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				EFFBase::InitEFFContext();
			}
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			{
				EFFBase::ReleaseEFFContext();
			}
			break;
	}
	return TRUE;
}

