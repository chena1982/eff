/******************************************************************************
	created:	2008-12-13   0:32
	file path:	d:\EFF\EFFEngine\Include\EFFMath\EFFMathBaseDefines.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFMathBaseDefines_2008_12_13__
#define __EFFMathBaseDefines_2008_12_13__





#define EFFMATH_BEGIN	namespace EFFMATH {
#define EFFMATH_END		}
#define USE_EFFMATH		using namespace EFFMATH;

#ifndef EFFMATH_EXPORTS
	#ifndef _EFFMATHLIB
		#define EFFMATH_API		__declspec(dllimport)
	#else
		#define EFFMATH_API
	#endif
#else
	#define EFFMATH_API		__declspec(dllexport)
#endif


EFFMATH_BEGIN

enum VectorComponent
{
	X,
	Y,
	Z,
	W
};


//定义这些结构体，主要为了解决互相引用的问题，这样EFFMathFunction.h不需要include EFFVector3.h EFFMatrix4.h这些头文件
struct EFFVECTOR2
{
	effFLOAT	x;
	effFLOAT	y;
};

struct EFFVECTOR3
{
	union
	{
		struct 
		{
			effFLOAT	x;
			effFLOAT	y;
			effFLOAT	z;
		};

		effFLOAT		value[3];
	};
};

struct EFFVECTOR4
{
	union
	{
		struct 
		{
			effFLOAT	x;
			effFLOAT	y;
			effFLOAT	z;
			effFLOAT	w;
		};

		effFLOAT		value[4];
	};
};

struct EFFQUATERNION
{
	union
	{
		struct 
		{
			effFLOAT	x;
			effFLOAT	y;
			effFLOAT	z;
			effFLOAT	w;
		};

		effFLOAT		value[4];
	};
};


struct EFFMATRIX4
{
	union 
	{
		struct
		{
			effFLOAT	m11, m12, m13, m14;			
			effFLOAT	m21, m22, m23, m24;
			effFLOAT	m31, m32, m33, m34;
			effFLOAT	m41, m42, m43, m44;
		};

		effFLOAT		m[4][4];
	};
};

struct EFFPLANE
{
	union
	{
		struct 
		{
			effFLOAT	a;
			effFLOAT	b;
			effFLOAT	c;
			effFLOAT	d;
		};
		effFLOAT		value[4];
	};
};

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define EFFMATH_EPSILON		(1.0e-5f)

//#ifdef PI 
//	#undef PI
//	#define PI					3.1415926535897932384626433832795028841971693993751f
//#else
//	#define PI					3.1415926535897932384626433832795028841971693993751f
//#endif

#ifdef PI 
	#undef PI
	#define PI					3.14159265f
#else
	#define PI					3.14159265f
#endif

#ifdef HALFPI
	#undef HALFPI
	#define HALFPI				1.57079632679489661923f
#else
	#define HALFPI				1.57079632679489661923f
#endif

#ifdef TWOPI
	#undef TWOPI
	#define TWOPI				6.28318530717958647692f
#else
	#define TWOPI				6.28318530717958647692f
#endif

#define	INVPI				0.31830988618379067154f									//!< 1.0 / PI

#define	RADTODEG			57.2957795130823208768f									//!< 180.0 / PI, convert radians to degrees
#define	DEGTORAD			0.01745329251994329577f									//!< PI / 180.0, convert degrees to radians

#define	EXP					2.71828182845904523536f									//!< e
#define	INVLOG2				3.32192809488736234787f									//!< 1.0 / log10(2)
#define	LN2					0.693147180559945f										//!< ln(2)
#define	INVLN2				1.44269504089f											//!< 1.0f / ln(2)

#define	INV3				0.33333333333333333333f									//!< 1/3
#define	INV6				0.16666666666666666666f									//!< 1/6
#define	INV7				0.14285714285714285714f									//!< 1/7
#define	INV9				0.11111111111111111111f									//!< 1/9
#define	INV255				0.00392156862745098039f									//!< 1/255

#define	SQRT2				1.41421356237f											//!< sqrt(2)
#define	INVSQRT2			0.707106781188f											//!< 1 / sqrt(2)

#define	SQRT3				1.73205080757f											//!< sqrt(3)
#define	INVSQRT3			0.577350269189f											//!< 1 / sqrt(3)



#define INVALID_NUMBER		0xDEADBEEF	//!< Standard junk value



#define	MAX_SBYTE				0x7f						//!< max possible sbyte value
#define	MIN_SBYTE				0x80						//!< min possible sbyte value
#define	MAX_UBYTE				0xff						//!< max possible ubyte value
#define	MIN_UBYTE				0x00						//!< min possible ubyte value
#define	MAX_SWORD				0x7fff						//!< max possible sword value
#define	MIN_SWORD				0x8000						//!< min possible sword value
#define	MAX_UWORD				0xffff						//!< max possible uword value
#define	MIN_UWORD				0x0000						//!< min possible uword value
#define	MAX_SDWORD				0x7fffffff					//!< max possible sdword value
#define	MIN_SDWORD				0x80000000					//!< min possible sdword value
#define	MAX_UDWORD				0xffffffff					//!< max possible udword value
#define	MIN_UDWORD				0x00000000					//!< min possible udword value
#define	MAX_FLOAT				FLT_MAX						//!< max possible effFLOAT value
#define	MIN_FLOAT				(-FLT_MAX)					//!< min possible loat value
#define IEEE_1_0				0x3f800000					//!< integer representation of 1.0
#define IEEE_255_0				0x437f0000					//!< integer representation of 255.0
#define IEEE_MAX_FLOAT			0x7f7fffff					//!< integer representation of MAX_FLOAT
#define IEEE_MIN_FLOAT			0xff7fffff					//!< integer representation of MIN_FLOAT
#define IEEE_UNDERFLOW_LIMIT	0x1a000000

#define ONE_OVER_RAND_MAX		(1.0f / effFLOAT(RAND_MAX))	//!< Inverse of the max possible value returned by rand()

#ifndef MIN
	#define		MIN(a, b)       ((a) < (b) ? (a) : (b))			//!< Returns the min value between a and b
#endif


#ifndef MAX
	#define		MAX(a, b)       ((a) > (b) ? (a) : (b))			//!< Returns the max value between a and b
#endif

#ifndef MAXMAX
	#define		MAXMAX(a,b,c)   ((a) > (b) ? MAX(a, c) : MAX(b, c))	//!<	Returns the max value between a, b and c
#endif

template<class T>
const T & TMin(const T & a, const T & b)
{
	return b < a ? b : a;
};


template<class T>
const T & TMax(const T & a, const T & b)
{
	return a < b ? b : a;
};

template<typename T>
effVOID TSetMin(T & a, const T & b)
{
	if ( a > b )
	{
		a = b;
	}
};

template<class T>
effVOID TSetMax(T & a, const T & b)
{
	if ( a < b )
	{
		a = b;
	}
};

#define		SQR(x)			((x)*(x))						//!< Returns x square
#define		CUBE(x)			((x)*(x)*(x))					//!< Returns x cube

#define		AND		&										//!< ...
#define		OR		|										//!< ...
#define		XOR		^										//!< ...

#define		QUADRAT(x)		((x)*(x))						//!< Returns x square

#ifdef _WIN32
#   define srand48(x) srand((effUINT) (x))
#	define srandom(x) srand((effUINT) (x))
#	define random()   ((effDOUBLE) rand())
#   define drand48()  ((effDOUBLE) (((effDOUBLE) rand()) / ((effDOUBLE) RAND_MAX)))
#endif

EFFMATH_END

#endif
