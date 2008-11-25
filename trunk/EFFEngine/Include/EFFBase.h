
#pragma once


#include ".\EFFBase\EFFBaseBaseDefines.h"
#include ".\EFFBase\EFFMemory.h"
#include ".\EFFBase\EFFReflection.h"
#include ".\EFFBase\EFFSerialize.h"
#include ".\EFFBase\EFFRtti.h"
#include ".\EFFBase\EFFFile.h"
#include ".\EFFBase\EFFSTLFile.h"

#if defined UNICODE || defined _UNICODE
#	if _DEBUG 
#		pragma comment(lib,"EFFBaseUD.lib")
#	else
#		pragma comment(lib,"EFFBaseU.lib")
#	endif
#else
#	if _DEBUG 
#		pragma comment(lib,"EFFBaseD.lib")
#	else
#		pragma comment(lib,"EFFBase.lib")
#	endif
#endif


