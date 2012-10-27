/******************************************************************************
	created:	2008-12-1   22:31
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFBaseDefines.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFBaseDefines_2008_12_1__
#define __EFFBaseDefines_2008_12_1__






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

#ifdef _MAC
	#define CALLBACK    PASCAL
	#define WINAPI      CDECL
	#define WINAPIV     CDECL
	#define APIENTRY    WINAPI
	#define APIPRIVATE  CDECL
	#ifdef _68K_
		#define PASCAL      __pascal
	#else
		#define PASCAL
	#endif
#elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
	#define CALLBACK    __stdcall
	#define WINAPI      __stdcall
	#define WINAPIV     __cdecl
	#define APIENTRY    WINAPI
	#define APIPRIVATE  __stdcall
	#define PASCAL      __stdcall
#else
	#define CALLBACK
	#define WINAPI
	#define WINAPIV
	#define APIENTRY    WINAPI
	#define APIPRIVATE
	#define PASCAL      pascal
#endif


//字符
typedef wchar_t effWCHAR;
typedef char effCHAR;

#if defined UNICODE || defined _UNICODE
	typedef effWCHAR effTCHAR;
	typedef std::wstring effString;
#else
	typedef effCHAR effTCHAR;
	typedef std::string effString;
#endif

typedef effCHAR *					effLPSTR;
typedef effWCHAR *					effLPWSTR;
typedef effTCHAR *					effLPTSTR;
typedef const effCHAR *				effLPCSTR;
typedef const effWCHAR *			effLPCWSTR;
typedef const effTCHAR *			effLPCTSTR;



// 有符号数据类型
typedef short						effSHORT;
typedef int							effINT;
typedef long						effLONG;
typedef __int64						effLONGLONG;
typedef char						effINT8;
typedef short						effINT16;
typedef int							effINT32;
typedef __int64						effINT64;


// 无符号数据类型
typedef unsigned char				effBYTE;
typedef unsigned short				effUSHORT;
typedef unsigned int				effUINT;
typedef unsigned long				effULONG;
typedef unsigned __int64			effULONGLONG;
typedef unsigned char				effUINT8;
typedef unsigned short				effUINT16;
typedef unsigned int				effUINT32;
typedef unsigned __int64			effUINT64;


typedef unsigned short				effWORD;
typedef unsigned long				effDWORD;



// 浮点数据类型
typedef float						effFLOAT;
typedef double						effDOUBLE;
typedef long double					effLDOUBLE;
typedef float						effFLOAT32;
typedef double						effFLOAT64;
typedef long double					effFLOAT128;



typedef void						effVOID;
typedef void *						effLPVOID;
typedef const void *				effLPCVOID;

typedef long						effHRESULT;
typedef void *						effHANDLE;


// boolean
typedef int							effBOOL;
const int effFALSE = 0;
const int effTRUE = 1;


#define SF_DELETE(p) { delete p; p = NULL; }
#define SFT_DELETE(p) { delete[] p; p = NULL; }

template <typename T>
effVOID SF_RELEASE(T * p)
{
	if ( p != NULL )
	{
		p->Release();
		p = NULL;
	}
};

template<class ClassType, class MemberType>
static inline effULONG mem_offset(MemberType ClassType::*member)
{
    return (effULONG)(effULONG *)&(((ClassType *)NULL)->*member); 
}

template<class ClassType, class MemberType>
static inline effULONG mem_size(MemberType ClassType::*member)
{
    return sizeof(((ClassType *)NULL)->*member); 
}

#define MEM_OFFSET(CLASS, MEMBER) (mem_offset(&CLASS::MEMBER))
#define MEM_SIZE(CLASS, MEMBER) (mem_size(&CLASS::MEMBER))




#define EFFBASE_BEGIN		namespace EFFBase {
#define EFFBASE_END			}
#define USE_EFFBASE			using namespace EFFBase;

#ifndef EFFBASE_EXPORTS
	#ifndef EFFLIB
		#define EFFBASE_API		__declspec(dllimport)
	#else
		#define EFFBASE_API
	#endif
#else
	#define EFFBASE_API		__declspec(dllexport)
#endif

#define EFFINLINE	__forceinline


	





#endif