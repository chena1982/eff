/******************************************************************************
	created:	2017-07-07   21:31
	file path:	d:\EFF\Include\EFFGUI\EFFGUIDefines.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFGUIDefines_2017_07_07__
#define __EFFGUIDefines_2017_07_07__

#define EFFGUI_BEGIN		namespace EFFGUI {
#define EFFGUI_END			}
#define USE_EFFGUI			using namespace EFFGUI;

EFFGUI_BEGIN

enum PositionType
{
	Absolute = 0,
	Relative,
};

#define Align_HCenter	1
#define Align_VCenter	2
#define Align_Center	(Align_HCenter | Align_VCenter),

#define Align_Left      4
#define Align_Right     8

#define Align_Top       16
#define Align_Bottom    32

#define Align_Default  (Align_Left | Align_Top)


#define SizeMode_HFix       1
#define SizeMode_VFix       2
#define SizeMode_HStretch   4
#define SizeMode_VStretch   8


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







#ifndef EFFGUI_EXPORTS
	#define EFFGUI_API		__declspec(dllimport)
#else
    #define EFFGUI_API		__declspec(dllexport)
#endif




	
EFFGUI_END




#endif