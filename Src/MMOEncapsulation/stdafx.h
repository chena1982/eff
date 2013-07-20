// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <Windowsx.h>

#include "resource.h"

// TODO: reference additional headers your program requires here
#include <yaml-cpp\yaml.h>

#include "EFFBase.h"
USE_EFFBASE

#include "EFFMath.h"
USE_EFFMATH

#include "EFF3D.h"
USE_EFF3D

#include <dwmapi.h>

#include "MMOEncapsulationBaseDefines.h"