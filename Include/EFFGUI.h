/******************************************************************************
created:	2017-07-07   21:31
file path:	d:\EFF\Include\EFFGUI\EFFGUI.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFGUI_2017_07_07__
#define __EFFGUI_2017_07_07__


#include "EFFGUI/EFFGUIWidget.h"


#if defined UNICODE || defined _UNICODE
	#ifdef _DEBUG 
		#pragma comment(lib, "EFFGUI_d.lib")
	#else
		#pragma comment(lib, "EFFGUI.lib")
	#endif
#else
	#if _DEBUG 
		#pragma comment(lib, "EFFGUI_d.lib")
	#else
		#pragma comment(lib, "EFFGUI.lib")
	#endif
#endif


#endif