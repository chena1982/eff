/******************************************************************************
	created:	2013-7-12   20:37
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFOSVersion.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFOSVersion.h"


EFFBASE_BEGIN

#ifdef _WIN32

EFFOSVERSION GetOSVersion()
{
	OSVERSIONINFO version;
    ZeroMemory(&version, sizeof(version));
    version.dwOSVersionInfoSize = sizeof(version);
    GetVersionEx(&version);
    

    if ( version.dwMajorVersion > 6 || ((version.dwMajorVersion == 6) && (version.dwMinorVersion >= 1)) )
	{
		return Win7;
	}

	return OS_Unknown;
}

#endif

EFFBASE_END

