/******************************************************************************
	created:	2012-9-21   22:54
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFStringHash.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFStringHash_2012_9_21__
#define __EFFStringHash_2012_9_21__


EFFBASE_BEGIN


effUINT CalculateFNV(const effTCHAR * str);

template <effUINT N, effUINT I>
struct FnvHash
{
	EFFINLINE static effUINT Hash(const effTCHAR (&str)[N])
	{
		effUINT ch = str[I-1];
		return (FnvHash<N, I-1>::Hash(str) ^ ch) * 16777619u;
	}
};
 
template <effUINT N>
struct FnvHash<N, 1>
{
	EFFINLINE static effUINT Hash(const effTCHAR (&str)[N])
	{
		effUINT ch = str[0];
		return (2166136261u ^ ch) * 16777619u;
	}
};
 
class EFFBASE_API EFFStringHash
{
public:

	class ConstCharWrapper
	{
	public:
		EFFINLINE ConstCharWrapper(const effTCHAR * str) : wrapperStr(str) {}
		const effTCHAR * wrapperStr;
	};
 
	EFFStringHash(ConstCharWrapper str)
	: hash(CalculateFNV(str.wrapperStr))
	{
	}

	template <effUINT N>
	EFFINLINE EFFStringHash(const effTCHAR (&str)[N])
	: hash(FnvHash<N, N>::Hash(str))
	{
	}

	EFFINLINE effUINT GetHash() { return hash; }

protected:
	effUINT hash;
};

EFFBASE_END


#endif