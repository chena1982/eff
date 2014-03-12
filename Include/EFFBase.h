/******************************************************************************
	created:	2008-12-12   23:01
	file path:	d:\EFF\EFFEngine\Include\EFFBase.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFBase_2008_12_12__
#define __EFFBase_2008_12_12__

#include <string>
#include <hash_map>
#include <map>
#include <vector>
#include <iostream>

#include <yaml-cpp\yaml.h>

#include "EFFBase\EFFBaseDefines.h"
#include "EFFBase\EFFMemory.h"
#include "EFFBase\EFFUtility.h"
#include "EFFBase\EFFRtti.h"
#include "EFFBase\EFFRttiApi.h"
#include "EFFBase\EFFFile.h"
#include "EFFBase\EFFSTLFile.h"
#include "EFFBase\EFFObject.h"
#include "EFFBase\EFFObjectManager.h"
#include "EFFBase\EFFGlobal.h"
#include "EFFBase\EFFComponent.h"
#include "EFFBase\EFFIProxy.h"
#include "EFFBase\EFFEvent.h"
#include "EFFBase\Reflection4CppEnum.h"
//#include "EFFBase\EFFThread.h"
#include "EFFBase\EFFStringHash.h"
#include "EFFBase\EFFNetClient.h"
#include "EFFBase\EFFNetServer.h"
//#include "EFFBase\EFFGameObject.h"
#include "EFFBase\EFFScopeGuard.h"
#include "EFFBase\EFFOSVersion.h"
#include "EFFBase\EFFInputManager.h"


#if defined UNICODE || defined _UNICODE
	#ifdef _DEBUG 
		#pragma comment(lib, "EFFBaseUD.lib")
		#pragma comment(lib, "libyaml-cppmdd.lib")
	#else
		#pragma comment(lib, "EFFBaseU.lib")
	#endif
#else
	#if _DEBUG 
		#pragma comment(lib, "EFFBaseD.lib")
	#else
		#pragma comment(lib, "EFFBase.lib")
	#endif
#endif


#endif