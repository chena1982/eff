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


#if defined(__has_feature)
#	define EFF_CLANG_HAS_FEATURE(_x) __has_feature(_x)
#else
#	define EFF_CLANG_HAS_FEATURE(_x) 0
#endif // defined(__has_feature)

// Compiler
#define EFF_COMPILER_CLANG           0
#define EFF_COMPILER_CLANG_ANALYZER  0
#define EFF_COMPILER_GCC             0
#define EFF_COMPILER_MSVC            0


// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Compilers
#if defined(__clang__)
// clang defines __GNUC__ or _MSC_VER
#	undef  EFF_COMPILER_CLANG
#	define EFF_COMPILER_CLANG (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#	if defined(__clang_analyzer__)
#		undef  EFF_COMPILER_CLANG_ANALYZER
#		define EFF_COMPILER_CLANG_ANALYZER 1
#	endif // defined(__clang_analyzer__)
#elif defined(_MSC_VER)
#	undef  EFF_COMPILER_MSVC
#	define EFF_COMPILER_MSVC _MSC_VER
#elif defined(__GNUC__)
#	undef  EFF_COMPILER_GCC
#	define EFF_COMPILER_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#	error "EFF_COMPILER_* is not defined!"
#endif //

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Architectures
#if defined(__arm__)     || \
	defined(__aarch64__) || \
	defined(_M_ARM)
#	undef  EFF_CPU_ARM
#	define EFF_CPU_ARM 1
#	define EFF_CACHE_LINE_SIZE 64
#elif defined(__MIPSEL__)     || \
	  defined(__mips_isa_rev) || \
	  defined(__mips64)
#	undef  EFF_CPU_MIPS
#	define EFF_CPU_MIPS 1
#	define EFF_CACHE_LINE_SIZE 64
#elif defined(_M_PPC)        || \
	  defined(__powerpc__)   || \
	  defined(__powerpc64__)
#	undef  EFF_CPU_PPC
#	define EFF_CPU_PPC 1
#	define EFF_CACHE_LINE_SIZE 128
#elif defined(__riscv)   || \
	  defined(__riscv__) || \
	  defined(RISCVEL)
#	undef  EFF_CPU_RISCV
#	define EFF_CPU_RISCV 1
#	define EFF_CACHE_LINE_SIZE 64
#elif defined(_M_IX86)    || \
	  defined(_M_X64)     || \
	  defined(__i386__)   || \
	  defined(__x86_64__)
#	undef  EFF_CPU_X86
#	define EFF_CPU_X86 1
#	define EFF_CACHE_LINE_SIZE 64
#else // PNaCl doesn't have CPU defined.
#	undef  EFF_CPU_JIT
#	define EFF_CPU_JIT 1
#	define EFF_CACHE_LINE_SIZE 64
#endif //

#if defined(__x86_64__)    || \
	defined(_M_X64)        || \
	defined(__aarch64__)   || \
	defined(__64BIT__)     || \
	defined(__mips64)      || \
	defined(__powerpc64__) || \
	defined(__ppc64__)     || \
	defined(__LP64__)
#	undef  EFF_ARCH_64BIT
#	define EFF_ARCH_64BIT 64
#else
#	undef  EFF_ARCH_32BIT
#	define EFF_ARCH_32BIT 32
#endif //

#if EFF_CPU_PPC
#	undef  EFF_CPU_ENDIAN_BIG
#	define EFF_CPU_ENDIAN_BIG 1
#else
#	undef  EFF_CPU_ENDIAN_LITTLE
#	define EFF_CPU_ENDIAN_LITTLE 1
#endif // EFF_PLATFORM_

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Operating_Systems
#if defined(_DURANGO) || defined(_XBOX_ONE)
#	undef  EFF_PLATFORM_XBOXONE
#	define EFF_PLATFORM_XBOXONE 1
#elif defined(_WIN32) || defined(_WIN64)
// http://msdn.microsoft.com/en-us/library/6sehtctf.aspx
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif // NOMINMAX
//  If _USING_V110_SDK71_ is defined it means we are using the v110_xp or v120_xp toolset.
#	if defined(_MSC_VER) && (_MSC_VER >= 1700) && (!_USING_V110_SDK71_)
#		include <winapifamily.h>
#	endif // defined(_MSC_VER) && (_MSC_VER >= 1700) && (!_USING_V110_SDK71_)
#	if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
#		undef  EFF_PLATFORM_WINDOWS
#		if !defined(WINVER) && !defined(_WIN32_WINNT)
#			if EFF_ARCH_64BIT
//				When building 64-bit target Win7 and above.
#				define WINVER 0x0601
#				define _WIN32_WINNT 0x0601
#			else
//				Windows Server 2003 with SP1, Windows XP with SP2 and above
#				define WINVER 0x0502
#				define _WIN32_WINNT 0x0502
#			endif // EFF_ARCH_64BIT
#		endif // !defined(WINVER) && !defined(_WIN32_WINNT)
#		define EFF_PLATFORM_WINDOWS _WIN32_WINNT
#	else
#		undef  EFF_PLATFORM_WINRT
#		define EFF_PLATFORM_WINRT 1
#	endif
#elif defined(__ANDROID__)
// Android compiler defines __linux__
#	include <sys/cdefs.h> // Defines __BIONIC__ and includes android/api-level.h
#	undef  EFF_PLATFORM_ANDROID
#	define EFF_PLATFORM_ANDROID __ANDROID_API__
#elif defined(__STEAMLINK__)
// SteamLink compiler defines __linux__
#	undef  EFF_PLATFORM_STEAMLINK
#	define EFF_PLATFORM_STEAMLINK 1
#elif defined(__VCCOREVER__)
// RaspberryPi compiler defines __linux__
#	undef  EFF_PLATFORM_RPI
#	define EFF_PLATFORM_RPI 1
#elif  defined(__linux__)
#	undef  EFF_PLATFORM_LINUX
#	define EFF_PLATFORM_LINUX 1
#elif  defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) \
	|| defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#	undef  EFF_PLATFORM_IOS
#	define EFF_PLATFORM_IOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#	undef  EFF_PLATFORM_OSX
#	define EFF_PLATFORM_OSX __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#elif defined(__EMSCRIPTEN__)
#	undef  EFF_PLATFORM_EMSCRIPTEN
#	define EFF_PLATFORM_EMSCRIPTEN 1
#elif defined(__ORBIS__)
#	undef  EFF_PLATFORM_PS4
#	define EFF_PLATFORM_PS4 1
#elif  defined(__FreeBSD__)        \
	|| defined(__FreeBSD_kernel__) \
	|| defined(__NetBSD__)         \
	|| defined(__OpenBSD__)        \
	|| defined(__DragonFly__)
#	undef  EFF_PLATFORM_BSD
#	define EFF_PLATFORM_BSD 1
#elif defined(__GNU__)
#	undef  EFF_PLATFORM_HURD
#	define EFF_PLATFORM_HURD 1
#elif defined(__NX__)
# undef EFF_PLATFORM_NX
# define EFF_PLATFORM_NX 1
#endif //


#if EFF_COMPILER_GCC || EFF_COMPILER_CLANG
#	define EFF_ALIGN_DECL(_align, _decl) _decl __attribute__( (aligned(_align) ) )
#	define EFF_ALLOW_UNUSED __attribute__( (unused) )
#	define EFF_FORCE_INLINE inline __attribute__( (__always_inline__) )
#	define EFF_FUNCTION __PRETTY_FUNCTION__
#	define EFF_LIKELY(_x)   __builtin_expect(!!(_x), 1)
#	define EFF_UNLIKELY(_x) __builtin_expect(!!(_x), 0)
#	define EFF_NO_INLINE   __attribute__( (noinline) )
#	define EFF_NO_RETURN   __attribute__( (noreturn) )
#	define EFF_CONST_FUNC  __attribute__( (const) )

#	if EFF_COMPILER_GCC >= 70000
#		define EFF_FALLTHROUGH __attribute__( (fallthrough) )
#	else
#		define EFF_FALLTHROUGH EFF_NOOP()
#	endif // EFF_COMPILER_GCC >= 70000

#	define EFF_NO_VTABLE
#	define EFF_PRINTF_ARGS(_format, _args) __attribute__( (format(__printf__, _format, _args) ) )

#	if EFF_CLANG_HAS_FEATURE(cxx_thread_local)
#		define EFF_THREAD_LOCAL __thread
#	endif // EFF_COMPILER_CLANG

#	if (!EFF_PLATFORM_OSX && (EFF_COMPILER_GCC >= 40200)) || (EFF_COMPILER_GCC >= 40500)
#		define EFF_THREAD_LOCAL __thread
#	endif // EFF_COMPILER_GCC

#	define EFF_ATTRIBUTE(_x) __attribute__( (_x) )

#	if EFF_CRT_MSVC
#		define __stdcall
#	endif // EFF_CRT_MSVC
#elif EFF_COMPILER_MSVC
#	define EFF_ALIGN_DECL(_align, _decl) __declspec(align(_align) ) _decl
#	define EFF_ALLOW_UNUSED
#	define EFF_FORCE_INLINE __forceinline
#	define EFF_FUNCTION __FUNCTION__
#	define EFF_LIKELY(_x)   (_x)
#	define EFF_UNLIKELY(_x) (_x)
#	define EFF_NO_INLINE __declspec(noinline)
#	define EFF_NO_RETURN
#	define EFF_CONST_FUNC  __declspec(noalias)
#	define EFF_FALLTHROUGH EFF_NOOP()
#	define EFF_NO_VTABLE __declspec(novtable)
#	define EFF_PRINTF_ARGS(_format, _args)
#	define EFF_THREAD_LOCAL __declspec(thread)
#	define EFF_ATTRIBUTE(_x)
#else
#	error "Unknown EFF_COMPILER_?"
#endif

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


static const effUINT16 InvalidIndex16 = UINT16_MAX;

#define EFF_ALIGN_MASK(_value, _mask) ( ( (_value)+(_mask) ) & ( (~0)&(~(_mask) ) ) )
#define EFF_ALIGN_16(_value) EFF_ALIGN_MASK(_value, 0xf)
#define EFF_ALIGN_256(_value) EFF_ALIGN_MASK(_value, 0xff)
#define EFF_ALIGN_4096(_value) EFF_ALIGN_MASK(_value, 0xfff)

#define EFF_ALIGNOF(_type) __alignof(_type)

#define EFFINLINE	__forceinline

#ifndef EFFBASE_EXPORTS
	#ifndef EFFBASE_EXPORT_STATIC
		#define EFFBASE_API		__declspec(dllimport)
	#else
		#define EFFBASE_API
	#endif
#else
	#define EFFBASE_API		__declspec(dllexport)
#endif


EFFBASE_END


#endif