/******************************************************************************
	created:	2012-02-25   02:06
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFUtilty.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFUtility_2012_02_25__
#define __EFFUtility_2012_02_25__

#include <boost\type_traits.hpp>

EFFBASE_BEGIN


class EFFBASE_API AnsiUnicodeStringConvert
{
public:
	static const effCHAR * W2A(const effWCHAR * str);
	static const effWCHAR * A2W(const effCHAR * str);
	static const effWCHAR * UTF82W(const effCHAR * str);
	static const effCHAR * W2UTF8(const effWCHAR * str);

private:
	static effCHAR charBuffer[1024];
	static effWCHAR wcharBuffer[1024];
};

#if defined UNICODE || defined _UNICODE
	#define EFFSTRING2ANSI(str) AnsiUnicodeStringConvert::W2A(str.c_str())
	#define ANSI2EFFSTRING(str) AnsiUnicodeStringConvert::A2W(str)
	#define UTF82EFFSTRING(str) AnsiUnicodeStringConvert::UTF82W(str)
	#define EFFSTRING2UTF8(str) AnsiUnicodeStringConvert::W2UTF8(str.c_str())
#else
	#define EFFSTRING2ANSI(str) (str).c_str()
	#define ANSI2EFFSTRING(str) str
#endif



#define EFFLOG(x)	x



template<typename T1, typename T2>
class EFF_IS_POD
{
};


template<typename T1>
class EFF_IS_POD<T1, boost::true_type>
{
public:
	typedef boost::true_type type;
};

template<typename T1>
class EFF_IS_POD<const T1 *, boost::true_type>
{
public:
	typedef boost::true_type type;
};

template<typename T1>
class EFF_IS_POD<const T1 &, boost::true_type>
{
public:
	typedef boost::true_type type;
};

template<typename T1>
class EFF_IS_POD<T1, boost::false_type>
{
public:
	typedef boost::false_type type;
};

template<typename T1>
class EFF_IS_POD<const T1 *, boost::false_type>
{
public:
	typedef boost::false_type type;
};


template<typename T1>
class EFF_IS_POD<const T1 &, boost::false_type>
{
public:
	typedef boost::false_type type;
};


template<>
class EFF_IS_POD<effString, boost::false_type>
{
public:
	typedef boost::true_type type;
};

template<>
class EFF_IS_POD<effString *, boost::false_type>
{
public:
	typedef boost::true_type type;
};

template<>
class EFF_IS_POD<effString &, boost::false_type>
{
public:
	typedef boost::true_type type;
};

template<>
class EFF_IS_POD<const effString *, boost::false_type>
{
public:
	typedef boost::true_type type;
};

template<>
class EFF_IS_POD<const effString &, boost::false_type>
{
public:
	typedef boost::true_type type;
};

effString EFFBASE_API GetPODTypeClassName(const effCHAR * propertyTypeName);


template<typename PropertyType, typename IsPODType>
class ClassNameTrait
{
public:
	effString operator ()()
	{
		return effString("class name trait error");

	}
};

template<typename PropertyType>
class ClassNameTrait<PropertyType, boost::true_type>
{
public:
	effString operator ()()
	{
		return GetPODTypeClassName(typeid(PropertyType).name());
	}
};

template<typename PropertyType>
class ClassNameTrait<PropertyType *, boost::true_type>
{
public:
	effString operator ()()
	{
		return GetPODTypeClassName(typeid(PropertyType).name());
	}
};

template<typename PropertyType>
class ClassNameTrait<PropertyType &, boost::true_type>
{
public:
	effString operator ()()
	{
		return GetPODTypeClassName(typeid(PropertyType).name());
	}
};

template<typename PropertyType>
class ClassNameTrait<const PropertyType *, boost::true_type>
{
public:
	effString operator ()()
	{
		return GetPODTypeClassName(typeid(PropertyType).name());
	}
};

template<typename PropertyType>
class ClassNameTrait<const PropertyType &, boost::true_type>
{
public:
	effString operator ()()
	{
		return GetPODTypeClassName(typeid(PropertyType).name());
	}
};

template<typename PropertyType>
class ClassNameTrait<PropertyType, boost::false_type>
{
public:
	effString operator ()()
	{
		effString className = PropertyType::GetThisClass()->GetName();
		return className;
	}
};

template<typename PropertyType>
class ClassNameTrait<PropertyType *, boost::false_type>
{
public:
	effString operator ()()
	{
		effString className = PropertyType::GetThisClass()->GetName();
		return className;
	}
};

template<typename PropertyType>
class ClassNameTrait<PropertyType &, boost::false_type>
{
public:
	effString operator ()()
	{
		effString className = PropertyType::GetThisClass()->GetName();
		return className;
	}
};

template<typename PropertyType>
class ClassNameTrait<const PropertyType *, boost::false_type>
{
public:
	effString operator ()()
	{
		effString className = PropertyType::GetThisClass()->GetName();
		return className;
	}
};

template<typename PropertyType>
class ClassNameTrait<const PropertyType &, boost::false_type>
{
public:
	effString operator ()()
	{
		effString className = PropertyType::GetThisClass()->GetName();
		return className;
	}
};


template<>
class ClassNameTrait<effString, boost::true_type>
{
public:
	effString operator ()()
	{
		return _effT("effString");
	}
};

template<>
class ClassNameTrait<effString *, boost::true_type>
{
public:
	effString operator ()()
	{
		return _effT("effString");
	}
};

template<>
class ClassNameTrait<effString &, boost::true_type>
{
public:
	effString operator ()()
	{
		return _effT("effString");
	}
};

template<>
class ClassNameTrait<const effString *, boost::true_type>
{
public:
	effString operator ()()
	{
		return _effT("effString");
	}
};

template<>
class ClassNameTrait<const effString &, boost::true_type>
{
public:
	effString operator ()()
	{
		return _effT("effString");
	}
};


inline effUINT effUINT_cntbits_ref(effUINT value)
{
    effUINT count = 0;
    for (effUINT i = 0; i < 32; i++)
    {
        effUINT tmp = value & 0x1;
        if (tmp == 1)
        {
            count++;
        }
        value = value >> 1;
    }

    return count;
}

/// Count number of bits set.
inline effUINT effUINT_cntbits(effUINT _val)
{
#if EFF_COMPILER_GCC || EFF_COMPILER_CLANG
    return __builtin_popcount(_val);
#elif EFF_COMPILER_MSVC && EFF_PLATFORM_WINDOWS
    return __popcnt(_val);
#else
    return uint32_cntbits_ref(_val);
#endif // BX_COMPILER_
}


inline effUINT effUINT_cnttz_ref(effUINT value)
{
    const effUINT tmp0 = ~value;
    const effUINT tmp1 = value - 1;
    const effUINT tmp2 = tmp0 & tmp1;
    const effUINT result = effUINT_cntbits(tmp2);

    return result;
}

inline effUINT effUINT_cnttz(effUINT value)
{
#if EFF_COMPILER_GCC || EFF_COMPILER_CLANG
    return __builtin_ctz(_val);
#elif EFF_COMPILER_MSVC && EFF_PLATFORM_WINDOWS
    effDWORD index;
    _BitScanForward(&index, value);
    return index;
#else
    return uint32_cnttz_ref(_val);
#endif // BX_COMPILER_
}




#define RUN_ONCE(callback) {\
    static effBOOL runOnce = effTRUE;\
    if (runOnce)\
    {\
        callback();\
        runOnce = effFALSE;\
    }\
}

EFFBASE_END

#endif