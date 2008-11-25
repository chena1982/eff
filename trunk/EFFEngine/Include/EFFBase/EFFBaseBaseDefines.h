#ifndef __EFFBaseBaseDefines_H_2008_11_2_23_54__
#define __EFFBaseBaseDefines_H_2008_11_2_23_54__




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