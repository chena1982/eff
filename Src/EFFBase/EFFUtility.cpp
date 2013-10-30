#include "stdafx.h"
#include "EFFUtility.h"



EFFBASE_BEGIN

effCHAR AnsiUnicodeStringConvert::charBuffer[1024];
effWCHAR AnsiUnicodeStringConvert::wcharBuffer[1024];

const effCHAR * AnsiUnicodeStringConvert::W2A(const effWCHAR * str)
{
	effULONG length = WideCharToMultiByte(CP_OEMCP, NULL, str, -1, NULL, 0, NULL, FALSE);

	if ( length > 1023 )
	{
		return NULL;
	}

	WideCharToMultiByte(CP_OEMCP, NULL, str, -1, &AnsiUnicodeStringConvert::charBuffer[0], length, NULL, FALSE);
	return charBuffer;
}




const effWCHAR * AnsiUnicodeStringConvert::A2W(const effCHAR * str)
{
	effULONG length = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
 
	if ( length > 1023 )
	{
		return NULL;
	}

	MultiByteToWideChar(CP_ACP, 0, str, -1, &AnsiUnicodeStringConvert::wcharBuffer[0], length);
	return wcharBuffer;
}

const effWCHAR * AnsiUnicodeStringConvert::UTF82W(const effCHAR * str)
{
	effULONG length = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
 
	if ( length > 1023 )
	{
		return NULL;
	}

	MultiByteToWideChar(CP_UTF8, 0, str, -1, &AnsiUnicodeStringConvert::wcharBuffer[0], length);
	return wcharBuffer;
}

effString GetPODTypeClassName(const effCHAR * propertyTypeName)
{
	if ( strcmp(propertyTypeName, "int") == 0 )
	{
		return effString(_effT("effINT"));
	}
	else if ( strcmp(propertyTypeName, "float") == 0 )
	{
		return effString(_effT("effFLOAT"));
	}
	else if ( strcmp(propertyTypeName, "void") == 0 )
	{
		return effString(_effT("effVOID"));
	}
	else if ( strcmp(propertyTypeName, "bool") == 0 )
	{
		return effString(_effT("effBOOL"));
	}

	return effString(_effT("unkown pod type"));
}

EFFBASE_END