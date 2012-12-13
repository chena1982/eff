/********************************************************************
	created:	2009-11-23   23:30
	filename: 	d:\EFF\EFFEngine\Include\EFFBase\GetDllExportFunctionsName.h
	author:		ChenA
	
	purpose:	
*********************************************************************/

#ifndef __GetDllExportFunctionsName_2009_11_23__
#define __GetDllExportFunctionsName_2009_11_23__


extern "C" {


bool EFFBASE_API GetDllExportFunctionsName(TCHAR *szFileName, int & nNumOfExports, char ** & pszFunctions);

void EFFBASE_API FreeFunctionsNameArray(int nNumOfExports, char ** lppBuffer);

}

#endif