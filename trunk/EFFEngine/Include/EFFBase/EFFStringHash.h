/******************************************************************************
	created:	2012-9-21   22:54
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFStringHash.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFStringHash_2012_9_21__
#define __EFFStringHash_2012_9_21__

//http://www.altdevblogaday.com/2011/10/27/quasi-compile-time-string-hashing/

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

//±‡“Î ±hash
class EFFBASE_API EFFStringHash
{
public:

	/*class ConstCharWrapper
	{
	public:
		EFFINLINE ConstCharWrapper(const effTCHAR * str) : wrapperStr(str) {}
		const effTCHAR * wrapperStr;
	};*/
 
	EFFStringHash(const effString & str)
	: hash(CalculateFNV(str.c_str()))
	{
	}

	template <effUINT N>
	EFFINLINE EFFStringHash(const effTCHAR (&str)[N])
	: hash(FnvHash<N, N>::Hash(str))
	{
	}

	EFFStringHash() { hash = 0; }

	EFFStringHash(const EFFStringHash & stringHash) { hash = stringHash.hash; }

public:

	effVOID CalculateHash(const effString & str) { hash = CalculateFNV(str.c_str()); }

	EFFINLINE effUINT GetHash() const { return hash; }

	effBOOL operator == (const EFFStringHash & stringHash) { return hash == stringHash.GetHash(); }
protected:
	effUINT hash;
};

EFFBASE_END


#endif