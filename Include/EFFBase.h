/******************************************************************************
	created:	2008-12-12   23:01
	file path:	d:\EFF\EFFEngine\Include\EFFBase.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFBase_2008_12_12__
#define __EFFBase_2008_12_12__

//#include <string>
//#include <hash_map>
//#include <map>
//#include <VECTOR>
//#include <iostream>

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/map.h>
#include <EASTL/deque.h>
#include <EASTL/queue.h>
#include <EASTL/algorithm.h>
#include <EASTL/fixed_allocator.h>
#include <EASTL/unordered_map.h>
#include <EASTL/set.h>

#include <jsoncpp-1.8.1/include/json/json.h>
#include <yaml/yaml.h>
#include <yaml-cpp/yaml.h>


#include "EFFBase/EFFBaseDefines.h"
#include "EFFBase/EFFMemory.h"
#include "EFFBase/EFFUtility.h"
#include "EFFBase/EFFRtti.h"
#include "EFFBase/EFFRttiApi.h"
#include "EFFBase/EFFFile.h"
#include "EFFBase/EFFSTLFile.h"
#include "EFFBase/EFFObject.h"
#include "EFFBase/EFFObjectManager.h"
#include "EFFBase/EFFGlobal.h"
#include "EFFBase/EFFComponent.h"
#include "EFFBase/EFFIProxy.h"
#include "EFFBase/EFFEvent.h"
#include "EFFBase/Reflection4CppEnum.h"
//#include "EFFBase/EFFThread.h"
#include "EFFBase/EFFSemaphore.h"
#include "EFFBase/EFFMutex.h"
#include "EFFBase/EFFThread.h"
#include "EFFBase/EFFStringHash.h"
#include "EFFBase/EFFNetClient.h"
#include "EFFBase/EFFNetServer.h"
//#include "EFFBase/EFFGameObject.h"
#include "EFFBase/EFFScopeGuard.h"
#include "EFFBase/EFFOSVersion.h"
#include "EFFBase/EFFInputManager.h"

#include "EFFBase/EFFMessageDefines.h"
#include "EFFBase/EFFEntity.h"
#include "EFFBase/EFFString.h"

#include "EFFBase/EFFRingBuffer.h"

#if defined UNICODE || defined _UNICODE
	#ifdef _DEBUG 
		#pragma comment(lib, "EFFBase_d.lib")
	#else
		#pragma comment(lib, "EFFBase.lib")
	#endif
#else
	#if _DEBUG 
		#pragma comment(lib, "EFFBase_d.lib")
	#else
		#pragma comment(lib, "EFFBase.lib")
	#endif
#endif


#endif