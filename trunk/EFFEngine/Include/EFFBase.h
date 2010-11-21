/******************************************************************************
	created:	2008-12-12   23:01
	file path:	d:\EFF\EFFEngine\Include\EFFBase.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFBase_2008_12_12__
#define __EFFBase_2008_12_12__


#include ".\EFFBase\EFFBaseBaseDefines.h"
#include ".\EFFBase\EFFMemory.h"
#include ".\EFFBase\EFFReflection.h"
#include ".\EFFBase\EFFSerialize.h"
#include ".\EFFBase\EFFRtti.h"
#include ".\EFFBase\EFFFile.h"
#include ".\EFFBase\EFFSTLFile.h"
#include ".\EFFBase\EFFObject.h"
#include ".\EFFBase\EFFObjectManager.h"
#include ".\EFFBase\EFFGlobal.h"
#include ".\EFFBase\EFFIUnknown.h"
#include ".\EFFBase\EFFIProxy.h"
#include ".\EFFBase\EFFEvent.h"

#if defined UNICODE || defined _UNICODE
	#ifdef _DEBUG 
		#pragma comment(lib,"EFFBaseUD.lib")
	#else
		#pragma comment(lib,"EFFBaseU.lib")
	#endif
#else
	#if _DEBUG 
		#pragma comment(lib,"EFFBaseD.lib")
	#else
		#pragma comment(lib,"EFFBase.lib")
	#endif
#endif


#endif