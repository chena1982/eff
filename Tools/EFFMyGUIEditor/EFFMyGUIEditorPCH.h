#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <Windowsx.h>
#include <Shellapi.h>


#include <d3d9.h>


#include <EFF3D.h>
using namespace EFF3D;

#include <EFFFramework.h>



#include <Base/Main.h>  
#include <BaseManager.h>
#include <MyGUI.h>

#include <SettingsManager.h>
using namespace tools;

#ifndef EFFMYGUIEDITOR_EXPORTS
	#define EFFMYGUIEDITOR_API		__declspec(dllimport)
#else
	#define EFFMYGUIEDITOR_API		__declspec(dllexport)
#endif