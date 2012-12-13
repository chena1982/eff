


/******************************************************************************
	created:	2008-12-2   0:27
	file path:	d:\EFF\EFFEngine\Include.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9_2008_12_2__
#define __EFFD3D9_2008_12_2__


#if defined UNICODE || defined _UNICODE
#	if _DEBUG 
#		pragma comment(lib,"EFFD3D9UD.lib")
#	else
#		pragma comment(lib,"EFFD3D9U.lib")
#	endif
#else
#	if _DEBUG 
#		pragma comment(lib,"EFFD3D9D.lib")
#	else
#		pragma comment(lib,"EFFD3D9.lib")
#	endif
#endif

#endif