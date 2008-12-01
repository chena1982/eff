/******************************************************************************
	created:	2008-12-1   22:31
	file path:	d:\EFF\EFFEngine\Include\EFFBase.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFBaseBaseDefines_2008_12_1__
#define __EFFBaseBaseDefines_2008_12_1__






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




#if defined UNICODE || defined _UNICODE
#define _effT(s)		L##s
#else
#define _effT(s)		s
#endif




//×Ö·û
typedef wchar_t effWCHAR;
typedef char effCHAR;

#if defined UNICODE || defined _UNICODE
	typedef effWCHAR effTCHAR;
#else
	typedef effCHAR effTCHAR;
#endif

typedef effCHAR * effLPSTR;
typedef effWCHAR * effLPWSTR;
typedef effTCHAR * effLPTSTR;
typedef const effCHAR * effLPCSTR;
typedef const effWCHAR * effLPCWSTR;
typedef const effTCHAR * effLPCTSTR;




typedef float Real;


#define SF_DELETE(p) { delete p; p = NULL; }
#define SFT_DELETE(p) { delete[] p; p = NULL; }

template <typename T>
void SF_RELEASE(T * p)
{
	if( p != NULL )
	{
		p->Release();
		p = NULL;
	}
};


#ifndef EFFBASE_EXPORTS
	#ifndef _EFFLIB
		#define EFFBASE_API		__declspec(dllimport)
	#else
		#define EFFBASE_API
	#endif
#else
	#define EFFBASE_API		__declspec(dllexport)
#endif




#endif