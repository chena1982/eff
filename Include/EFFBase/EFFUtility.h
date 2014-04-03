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


EFFBASE_END

#endif