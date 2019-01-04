/******************************************************************************
	created:	2012-10-23   0:04
	file path:	d:\EFF\EFFEngine\Include\EFFFramework.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFFramework_2012_10_23__
#define __EFFFramework_2012_10_23__

#include "EFFFramework/EFFFrameworkBaseDefines.h"
#include "EFFFramework/EFFApplication.h"
#include "EFFFramework/EFFFrameworkMessageManager.h"

#if defined UNICODE || defined _UNICODE
#	ifdef _DEBUG 
#		pragma comment(lib,"EFFFramework_d.lib")
#	else
#		pragma comment(lib,"EFFFramework.lib")
#	endif
#else
#	if _DEBUG 
#		pragma comment(lib,"EFFFramework_d.lib")
#	else
#		pragma comment(lib,"EFFFramework.lib")
#	endif
#endif

#endif



