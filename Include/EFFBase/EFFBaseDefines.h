/******************************************************************************
	created:	2008-12-1   22:31
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFBaseDefines.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFBaseDefines_2008_12_1__
#define __EFFBaseDefines_2008_12_1__


#define EFFBASE_BEGIN		namespace EFFBase {
#define EFFBASE_END			}
#define USE_EFFBASE			using namespace EFFBase;

EFFBASE_BEGIN

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
    #ifdef _WIN64 // VC++ defines both _WIN32 and _WIN64 when compiling for Win64.
        #define EFF_PLATFORM_WIN64 1
    #else
        #define EFF_PLATFORM_WIN32 1
    #endif
#endif

#define USE_EASTL 1


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


#if defined(USE_EASTL)
    #if defined UNICODE || defined _UNICODE
	    typedef effWCHAR effTCHAR;
	    typedef eastl::wstring effString;
        typedef eastl::string effStringA;
    #else
	    typedef effCHAR effTCHAR;
	    typedef eastl::string effString;
		typedef eastl::string effStringA;
    #endif

	#define VECTOR eastl::vector
	#define DEQUE eastl::deque
	#define MAP eastl::map
	#define MAKE_PAIR eastl::make_pair
	#define FOR_EACH eastl::for_each
	#define MIN eastl::min
#else
    #if defined UNICODE || defined _UNICODE
        typedef effWCHAR effTCHAR;
        typedef std::wstring effString;
		typedef std::string effStringA;
    #else
        typedef effCHAR effTCHAR;
        typedef std::string effString;
    #endif

	#define VECTOR std::vector
	#define DEQUE std::deque
	#define MAP std::map
	#define MAKE_PAIR std::make_pair
	#define FOR_EACH std::for_each
	#define MIN std::min
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


#if defined(EFF_PLATFORM_WIN64)
    typedef unsigned __int64		effSIZE;
#elif defined(EFF_PLATFORM_WIN32)
    typedef unsigned int		    effSIZE;
#endif

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
    return (effULONG)(effSIZE)(effULONG *)&(((ClassType *)NULL)->*member); 
}

template<class ClassType, class MemberType>
static inline effULONG mem_size(MemberType ClassType::*member)
{
    return sizeof(((ClassType *)NULL)->*member); 
}

#define MEM_OFFSET(CLASS, MEMBER) (mem_offset(&CLASS::MEMBER))
#define MEM_SIZE(CLASS, MEMBER) (mem_size(&CLASS::MEMBER))

enum class SliderType
{
    Horizontal,
    Vertical
};

struct Slider
{
    SliderType type;

    Slider(SliderType type)
        : type(type) { }
};

struct Range
{
    float min, max;

    Range(float min, float max)
        : min(min)
        , max(max) { }
};


#define EFF_INVALID_HANDLE 0xFFFFFFFF

#define EFF_HANDLE(name)                                                           \
	struct name { effUINT idx; };                                                  \
	inline effBOOL isValid(name handle) { return EFF_INVALID_HANDLE != handle.idx; }



#ifndef EFFBASE_EXPORTS
	#ifndef EFFBASE_EXPORT_STATIC
		#define EFFBASE_API		__declspec(dllimport)
	#else
		#define EFFBASE_API
	#endif
#else
	#define EFFBASE_API		__declspec(dllexport)
#endif

#define EFFINLINE	__forceinline



EFFBASE_END


#endif