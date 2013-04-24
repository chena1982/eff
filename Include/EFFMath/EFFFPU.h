/******************************************************************************
	created:	2013-02-04   21:35
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFMath\EFFFPU.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFFPU_2013_02_04__
#define __EFFFPU_2013_02_04__


EFFMATH_BEGIN

#define DW_AS_FLT(DW) ( *(effFLOAT *)&(DW) )
#define FLT_AS_DW(F) ( *(effUINT *)&(F) )
#define FLT_SIGN(F) ( (FLT_AS_DW(F) & 0x80000000L) )
#define ALMOST_ZERO(F) ( (FLT_AS_DW(F) & 0x7f800000L) == 0 )
#define IS_SPECIAL(F)  ( (FLT_AS_DW(F) & 0x7f800000L) == 0x7f800000L )

#define SIGN_BITMASK			0x80000000

//! Integer representation of a floating-point value.
#define IR(x)					((effUINT&)(x))

//! Signed integer representation of a floating-point value.
#define SIR(x)					((effINT&)(x))

//! Absolute integer representation of a floating-point value
#define AIR(x)					(IR(x)&0x7fffffff)

//! Floating-point representation of an integer value.
#define FR(x)					((effFLOAT&)(x))

//! Integer-based comparison of a floating point value.
//! Don't use it blindly, it can be faster or slower than the FPU comparison, depends on the context.
//#define IS_NEGATIVE_FLOAT(x)	( IR(x) & 0x80000000 )
#define IS_NEGATIVE_FLOAT(x)	( x < 0.0f )


#ifdef SYSTEM_WIN32

//! Fast fabs for floating-point values. It just clears the sign bit.
//! Don't use it blindy, it can be faster or slower than the FPU comparison, depends on the context.
inline effFLOAT FastFabs(effFLOAT x)
{
	effDWORD FloatBits = IR(x)&0x7fffffff;
	return FR(FloatBits);
}

//! Fast square root for floating-point values.
inline effFLOAT FastSqrt(effFLOAT square)
{
	effFLOAT retval;

	__asm
	{
		mov             eax, square
		sub             eax, 0x3F800000
		sar             eax, 1
		add             eax, 0x3F800000
		mov             [retval], eax
	}
	return retval;
}

//! Saturates positive to zero.
inline effFLOAT fsat(effFLOAT f)
{
	effUINT y = (effUINT&)f & ~((effINT&)f >>31);
	return (effFLOAT&)y;
}

//! Computes 1.0f / sqrtf(x).
inline effFLOAT frsqrt(effFLOAT f)
{
	effFLOAT x = f * 0.5f;
	effUINT y = 0x5f3759df - ((effUINT&)f >> 1);
	// Iteration...
	(effFLOAT&)y  = (effFLOAT&)y * ( 1.5f - ( x * (effFLOAT&)y * (effFLOAT&)y ) );
	// Result
	return (effFLOAT&)y;
}

//! Computes 1.0f / sqrtf(x). Comes from NVIDIA.
inline effFLOAT InvSqrt(const effFLOAT& x)
{
	effUINT tmp = (effUINT(IEEE_1_0 << 1) + IEEE_1_0 - *(effUINT*)&x) >> 1;   
	effFLOAT y = *(effFLOAT*)&tmp;                                             
	return y * (1.47f - 0.47f * x * y * y);
}

//! Computes 1.0f / sqrtf(x). Comes from Quake3. Looks like the first one I had above.
//! See http://www.magic-software.com/3DGEDInvSqrt.html
inline effFLOAT RSqrt(effFLOAT number)
{
	long i;
	effFLOAT x2, y;
	const effFLOAT threehalfs = 1.5f;

	x2 = number * 0.5f;
	y  = number;
	i  = * (long *) &y;
	i  = 0x5f3759df - (i >> 1);
	y  = * (effFLOAT *) &i;
	y  = y * (threehalfs - (x2 * y * y));

	return y;
}

inline effFLOAT fsqrt(effFLOAT f)
{
	effUINT y = ( ( (effINT&)f - 0x3f800000 ) >> 1 ) + 0x3f800000;
	// Iteration...?
	// (effFLOAT&)y = (3.0f - ((effFLOAT&)y * (effFLOAT&)y) / f) * (effFLOAT&)y * 0.5f;
	// Result
	return (effFLOAT&)y;
}

//! Returns the effFLOAT ranged espilon value.
inline effFLOAT fepsilon(effFLOAT f)
{
	effUINT b = (effUINT&)f & 0xff800000;
	effUINT a = b | 0x00000001;
	(effFLOAT&)a -= (effFLOAT&)b;
	// Result
	return (effFLOAT&)a;
}

//! Is the effFLOAT valid ?
inline effBOOL IsNAN(effFLOAT value)				{ return (IR(value)&0x7f800000) == 0x7f800000;	}
inline effBOOL IsIndeterminate(effFLOAT value)	{ return IR(value) == 0xffc00000;				}
inline effBOOL IsPlusInf(effFLOAT value)			{ return IR(value) == 0x7f800000;				}
inline effBOOL IsMinusInf(effFLOAT value)		{ return IR(value) == 0xff800000;				}

inline effBOOL IsValidFloat(effFLOAT value)
{
	if(IsNAN(value))			return effFALSE;
	if(IsIndeterminate(value))	return effFALSE;
	if(IsPlusInf(value))		return effFALSE;
	if(IsMinusInf(value))		return effFALSE;
	return true;
}

#define CHECK_VALID_FLOAT(x)	ASSERT(IsValidFloat(x));

/*
//! FPU precision setting function.
inline void SetFPU()
{
// This function evaluates whether the floating-point
// control word is set to single precision/round to nearest/
// exceptions disabled. If these conditions don't hold, the
// function changes the control word to set them and returns
// TRUE, putting the old control word value in the passback
// location pointed to by pwOldCW.
{
uword wTemp, wSave;

__asm fstcw wSave
if (wSave & 0x300 ||            // Not single mode
0x3f != (wSave & 0x3f) ||   // Exceptions enabled
wSave & 0xC00)              // Not round to nearest mode
{
__asm
{
mov ax, wSave
and ax, not 300h    ;; single mode
or  ax, 3fh         ;; disable all exceptions
and ax, not 0xC00   ;; round to nearest mode
mov wTemp, ax
fldcw   wTemp
}
}
}
}
*/
//! This function computes the slowest possible floating-point value (you can also directly use FLT_EPSILON)
inline effFLOAT ComputeFloatEpsilon()
{
	effFLOAT f = 1.0f;
	((effUINT&)f)^=1;
	return f - 1.0f;	// You can check it's the same as FLT_EPSILON
}

inline effBOOL IsFloatZero(effFLOAT x, effFLOAT epsilon=1e-6f)
{
	return x*x < epsilon;
}

#define FCOMI_ST0	_asm	_emit	0xdb	_asm	_emit	0xf0
#define FCOMIP_ST0	_asm	_emit	0xdf	_asm	_emit	0xf0
#define FCMOVB_ST0	_asm	_emit	0xda	_asm	_emit	0xc0
#define FCMOVNB_ST0	_asm	_emit	0xdb	_asm	_emit	0xc0

#define FCOMI_ST1	_asm	_emit	0xdb	_asm	_emit	0xf1
#define FCOMIP_ST1	_asm	_emit	0xdf	_asm	_emit	0xf1
#define FCMOVB_ST1	_asm	_emit	0xda	_asm	_emit	0xc1
#define FCMOVNB_ST1	_asm	_emit	0xdb	_asm	_emit	0xc1

#define FCOMI_ST2	_asm	_emit	0xdb	_asm	_emit	0xf2
#define FCOMIP_ST2	_asm	_emit	0xdf	_asm	_emit	0xf2
#define FCMOVB_ST2	_asm	_emit	0xda	_asm	_emit	0xc2
#define FCMOVNB_ST2	_asm	_emit	0xdb	_asm	_emit	0xc2

#define FCOMI_ST3	_asm	_emit	0xdb	_asm	_emit	0xf3
#define FCOMIP_ST3	_asm	_emit	0xdf	_asm	_emit	0xf3
#define FCMOVB_ST3	_asm	_emit	0xda	_asm	_emit	0xc3
#define FCMOVNB_ST3	_asm	_emit	0xdb	_asm	_emit	0xc3

#define FCOMI_ST4	_asm	_emit	0xdb	_asm	_emit	0xf4
#define FCOMIP_ST4	_asm	_emit	0xdf	_asm	_emit	0xf4
#define FCMOVB_ST4	_asm	_emit	0xda	_asm	_emit	0xc4
#define FCMOVNB_ST4	_asm	_emit	0xdb	_asm	_emit	0xc4

#define FCOMI_ST5	_asm	_emit	0xdb	_asm	_emit	0xf5
#define FCOMIP_ST5	_asm	_emit	0xdf	_asm	_emit	0xf5
#define FCMOVB_ST5	_asm	_emit	0xda	_asm	_emit	0xc5
#define FCMOVNB_ST5	_asm	_emit	0xdb	_asm	_emit	0xc5

#define FCOMI_ST6	_asm	_emit	0xdb	_asm	_emit	0xf6
#define FCOMIP_ST6	_asm	_emit	0xdf	_asm	_emit	0xf6
#define FCMOVB_ST6	_asm	_emit	0xda	_asm	_emit	0xc6
#define FCMOVNB_ST6	_asm	_emit	0xdb	_asm	_emit	0xc6

#define FCOMI_ST7	_asm	_emit	0xdb	_asm	_emit	0xf7
#define FCOMIP_ST7	_asm	_emit	0xdf	_asm	_emit	0xf7
#define FCMOVB_ST7	_asm	_emit	0xda	_asm	_emit	0xc7
#define FCMOVNB_ST7	_asm	_emit	0xdb	_asm	_emit	0xc7

//! A global function to find MAX(a,b) using FCOMI/FCMOV
inline effFLOAT FCMax2(effFLOAT a, effFLOAT b)
{
	effFLOAT Res;
	_asm	fld		[a]
	_asm	fld		[b]
	FCOMI_ST1
	FCMOVB_ST1
	_asm	fstp	[Res]
	_asm	fcomp
	return Res;
}

//! A global function to find MIN(a,b) using FCOMI/FCMOV
inline effFLOAT FCMin2(effFLOAT a, effFLOAT b)
{
	effFLOAT Res;
	_asm	fld		[a]
	_asm	fld		[b]
	FCOMI_ST1
		FCMOVNB_ST1
		_asm	fstp	[Res]
		_asm	fcomp
			return Res;
}

//! A global function to find MAX(a,b,c) using FCOMI/FCMOV
inline effFLOAT FCMax3(effFLOAT a, effFLOAT b, effFLOAT c)
{
	effFLOAT Res;
	_asm	fld		[a]
	_asm	fld		[b]
	_asm	fld		[c]
	FCOMI_ST1
		FCMOVB_ST1
		FCOMI_ST2
		FCMOVB_ST2
		_asm	fstp	[Res]
		_asm	fcompp
			return Res;
}

//! A global function to find MIN(a,b,c) using FCOMI/FCMOV
inline effFLOAT FCMin3(effFLOAT a, effFLOAT b, effFLOAT c)
{
	effFLOAT Res;
	_asm	fld		[a]
	_asm	fld		[b]
	_asm	fld		[c]
	FCOMI_ST1
		FCMOVNB_ST1
		FCOMI_ST2
		FCMOVNB_ST2
		_asm	fstp	[Res]
		_asm	fcompp
			return Res;
}

inline int ConvertToSortable(effFLOAT f)
{
	int& Fi = (int&)f;
	int Fmask = (Fi>>31);
	Fi ^= Fmask;
	Fmask &= ~(1<<31);
	Fi -= Fmask;
	return Fi;
}

#endif

#endif


EFFMATH_END