/******************************************************************************
	created:	2017-07-07   21:31
	file path:	d:\EFF\Include\EFFGUI\EFFGUIDefines.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFGUIDefines_2017_07_07__
#define __EFFGUIDefines_2017_07_07__






#ifdef UNICODE
	#ifndef _UNICODE
		#define _UNICODE
	#endif
#endif

#ifdef _UNICODE
	#ifndef UNICODE
		#define UNICODE
	#endif
#endif





#define EFFGUI_BEGIN		namespace EFFGUI {
#define EFFGUI_END			}
#define USE_EFFGUI			using namespace EFFGUI;

#ifndef EFFGUI_EXPORTS
	#define EFFGUI_API		__declspec(dllimport)
#else
    #define EFFGUI_API		__declspec(dllexport)
#endif




	





#endif