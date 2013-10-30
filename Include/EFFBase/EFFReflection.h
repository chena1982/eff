/******************************************************************************
	created:	2008-12-1   22:32
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFReflection.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFReflection_2008_12_1__
#define __EFFReflection_2008_12_1__


#pragma warning(disable:4251)


#include <boost/type_traits.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/function.hpp>

EFFBASE_BEGIN

class EFFClass;
class EFFProperty;

//calculates the offset of a field
#define __OFFSET__(C, M)\
	((unsigned long)(&((const C *)0)->M))


std::string EFFBASE_API getClassNameFromTypeName(const char * pszTypeName);

#define __REPEAT0(M, C, S)         
#define __REPEAT1(M, C, S)         S M(1)
#define __REPEAT2(M, C, S)         __REPEAT1(M, C, S)  C M(2)
#define __REPEAT3(M, C, S)         __REPEAT2(M, C, S)  C M(3)
#define __REPEAT4(M, C, S)         __REPEAT3(M, C, S)  C M(4)
#define __REPEAT5(M, C, S)         __REPEAT4(M, C, S)  C M(5)
#define __REPEAT6(M, C, S)         __REPEAT5(M, C, S)  C M(6)
#define __REPEAT7(M, C, S)         __REPEAT6(M, C, S)  C M(7)
#define __REPEAT8(M, C, S)         __REPEAT7(M, C, S)  C M(8)
#define __REPEAT9(M, C, S)         __REPEAT8(M, C, S)  C M(9)
#define __REPEAT10(M, C, S)        __REPEAT9(M, C, S)  C M(10)
#define __REPEAT11(M, C, S)        __REPEAT10(M, C, S) C M(11)
#define __REPEAT12(M, C, S)        __REPEAT11(M, C, S) C M(12)
#define __REPEAT13(M, C, S)        __REPEAT12(M, C, S) C M(13)
#define __REPEAT14(M, C, S)        __REPEAT13(M, C, S) C M(14)
#define __REPEAT15(M, C, S)        __REPEAT14(M, C, S) C M(15)
#define __REPEAT16(M, C, S)        __REPEAT15(M, C, S) C M(16)
#define __REPEAT17(M, C, S)        __REPEAT16(M, C, S) C M(17)
#define __REPEAT18(M, C, S)        __REPEAT17(M, C, S) C M(18)
#define __REPEAT19(M, C, S)        __REPEAT18(M, C, S) C M(19)
#define __REPEAT20(M, C, S)        __REPEAT19(M, C, S) C M(20)
#define __REPEAT(N, M, C, S)       __REPEAT##N(M, C, S)

//N表示重复的次数，M是要重复的字符前缀，C表示分隔符，S表示在结果字符串的最前面加的一个字符，可以为空。
//那么最后的结果就是SM0CM1......CMN
//比如_REPEAT(4,__TYPE_ARG__(N),__COMMA__,__NOTHING__)的结果就是T1,T2,T3,T4

//various defs needed for parameters
#define __MAX_PARAMS__       20
#define __NOTHING__          
#define __COMMA__            ,
#define __TEMPLATE_ARG__(N)  typename T##N
#define __TYPE_ARG__(N)      T##N 
#define __ARG__(N)           T##N t##N
#define __PARAM__(N)         t##N
#define __PYTHON_PARAM__(N)	ptr(t##N)
#define __NOT_VIRTUAL__
#define __ADD_METHOD_ARGS__(N) info->argTypes.push_back(GetParameterProperty<T##N>(_effT("")).parameterProperty);
#define __SEMICOLON__   ;

//被委托人
class	__delegatee__
{
};

class __callable__info__
{
public:
	std::string						methodName;
	EFFProperty *					returnType;
	EFFProperty *					classType;
	std::vector<EFFProperty *>		argTypes;

public:
	__callable__info__()
	{
		returnType = NULL;
		classType = NULL;
	}
};

class __callable__
{
public:
	__callable__() { info = EFFNEW __callable__info__; }
	virtual ~__callable__() { SF_DELETE(info); }
public:
	__callable__info__ *	info;
};




// #define __CALLABLE__(N)\
// template <class R, class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
// class __callable##N##__ : public __callable__\
// {\
// public:\
// 	typedef R (__delegatee__::*MethodType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__));\
// 	typedef R (C::*MethodTypeReal)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__));\
// 	MethodType method;\
// 	typedef R ReturnValueType;\
// 	__callable##N##__(MethodTypeReal m)\
// 	{\
// 		method = (MethodType)m;\
// 		pInfo->m_strReturnTypeName = getClassNameFromTypeName(typeid(R).name());								/*得到返回值类型*/  \
// 		__REPEAT(N,__ADD_METHOD_ARGS__,__SEMICOLON__,__NOTHING__);			/*得到参数类型*/  \
// 	}\
// 	R invoke(C *object __REPEAT(N, __ARG__, __COMMA__, __COMMA__)) const		/*invoke函数*/  \
// 	{\
// 		return (object->*method)(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\
// 	}\
// };

#define __CALLABLE__(N)\
template <typename R __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
class __callable##N##__ : public __callable__\
{\
public:\
	typedef R (*FunctionPtrType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__));\
	typedef R ReturnValueType;\
	__callable##N##__(FunctionPtrType m)\
	{\
		functionPtr = m;\
		info->returnType = GetParameterProperty<R>(_effT("")).parameterProperty;		/*得到返回值类型*/  \
		__REPEAT(N,__ADD_METHOD_ARGS__,__SEMICOLON__,__NOTHING__);			/*得到参数类型*/  \
	}\
	R invoke(__REPEAT(N, __ARG__, __COMMA__, __NOTHING__)) const\
	{\
		functionPtr(__REPEAT(N,__PARAM__,__COMMA__,__NOTHING__));\
	}\
protected:\
	boost::function<R (__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __NOTHING__))>	functionPtr;\
};


#define __MEMBER_CALLABLE__(N)\
template <typename R, class C  __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
class __member_callable##N##__ : public __callable__\
{\
public:\
	typedef R (C::*FunctionPtrTypeReal)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__));\
	typedef R (__delegatee__::*FunctionPtrType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__));\
	typedef R ReturnValueType;\
	typedef C ClassType;\
	__member_callable##N##__(FunctionPtrTypeReal m)\
	{\
		funcPtr = (FunctionPtrType)m;\
		info->returnType = GetParameterProperty<R>(_effT("")).parameterProperty; /*得到返回值类型*/ \
		__REPEAT(N, __ADD_METHOD_ARGS__, __SEMICOLON__, __NOTHING__); /*得到参数类型*/ \
	}\
public:\
	FunctionPtrType		funcPtr;\
};


__CALLABLE__(0)
__CALLABLE__(1)
__CALLABLE__(2)
__CALLABLE__(3)
__CALLABLE__(4)

__MEMBER_CALLABLE__(0)
__MEMBER_CALLABLE__(1)
__MEMBER_CALLABLE__(2)
__MEMBER_CALLABLE__(3)

#define __CREATE_CALLABLE__(N)\
template <typename R __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
static inline __callable##N##__<R __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__)> * create(R (*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__)))\
{\
	return new __callable##N##__<R __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__)>(method);\
}

#define __CREATE_MEMBER_CALLABLE__(N)\
template <typename R, class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
	static inline __member_callable##N##__<R,C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__)> * create(R (C::*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__)))\
{\
	return new __member_callable##N##__<R,C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__)>(method);\
}



template <class R>
class __callable_factory__
{
public:
	__CREATE_CALLABLE__(0)
	__CREATE_CALLABLE__(1)
	__CREATE_CALLABLE__(2)
	__CREATE_CALLABLE__(3)
	__CREATE_CALLABLE__(4)


	__CREATE_MEMBER_CALLABLE__(0)
	__CREATE_MEMBER_CALLABLE__(1)
	__CREATE_MEMBER_CALLABLE__(2)
	__CREATE_MEMBER_CALLABLE__(3)

};


#define __MEMBER_METHOD__(N)\
template <class R,class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
__member_method__(R (C::*memberMethodAddress)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__)),char * pszMethodName)\
{\
	__callable__ * pMethod = __callable_factory__<R>::create(memberMethodAddress);\
	pMethod->pInfo->m_strMethodName = pszMethodName;\
	C::GetThisClass()->AddMemberMethod(pMethod);\
}

class __member_method__
{
public:
	__MEMBER_METHOD__(0)
	__MEMBER_METHOD__(1)
	__MEMBER_METHOD__(2)
	__MEMBER_METHOD__(3)
};

#define BEGIN_METHOD_MAP\
	static effVOID RegisterMethod()\
	{


#define END_METHOD_MAP\
	}

#define REGISTER_MEMBER_METHOD(METHOD_NAME)\
static __member_method__ reg##METHOD_NAME(&METHOD_NAME,#METHOD_NAME);

//#define MEMBER_METHOD(N,METHOD_NAME) __MEMBER_METHOD__(N,METHOD_NAME,__LINE__)

//#define __REGISTER_MEMBER_METHOD__()





class __property__
{
public:
	__property__() {}
	virtual ~__property__() {}
};

template<class T>
class __real_property__ : public __property__
{
public:
	__real_property__(unsigned long ulOffset,unsigned long ulSize,const char * pszName, const T & defaultValue) : 
		m_ulOffset(ulOffset),m_ulSize(ulSize),m_strName(pszName),m_defaultValue(defaultValue) {}

	__real_property__(unsigned long ulOffset,unsigned long ulSize,const char * pszName, const T & defaultValue, const T & minValue, const T & maxValue) : 
		m_ulOffset(ulOffset),m_ulSize(ulSize),m_strName(pszName),m_defaultValue(defaultValue),m_minValue(minValue),m_maxValue(maxValue) {}

	unsigned long		m_ulOffset;
	unsigned long		m_ulSize;
	std::string				m_strName;
	T							m_defaultValue;
	T							m_minValue;
	T							m_maxValue;


};

/*class Attribute
{

	Value z;	
};*/


class  __register_property__ 
{
public:
	template<class T>
	__register_property__(unsigned long ulOffset, unsigned long ulSize, const char * pszName, EFFClass * pClass, const T & defaultValue)
	{
		__real_property__<T> * pProperty = new __real_property__<T>(ulOffset, ulSize, pszName, defaultValue);
		pClass->AddProperty(pProperty);
	}
};


class __register_property_minmax__
{
public:
	template<class T>
	__register_property_minmax__(unsigned long ulOffset, unsigned long ulSize, const char * pszName, EFFClass * pClass, const T & defaultValue, const T & minValue, const T & maxValue)
	{
		__real_property__<T> * pProperty = new __real_property__<T>(ulOffset, ulSize, pszName, defaultValue, minValue, maxValue);
		pClass->AddProperty(pProperty);
	}
};






#define BEGIN_PROPERTY_MAP\
	static effVOID RegisterProperty()\
	{


#define END_PROPERTY_MAP\
	}

//__REGISTER_PROPERTY__(PROPERTY_NAME,PROPERTY_TYPE)



#define PROPERTY(PROPERTY_NAME)\
	VisitProperty(PROPERTY_NAME,pArg,boost::is_pod<BOOST_TYPEOF(PROPERTY_NAME)>());

#define PROPERTY_STL_CONTAINER(PROPERTY_NAME)\
	VisitProperty(PROPERTY_NAME,pArg,boost::is_pod<BOOST_TYPEOF(*(PROPERTY_NAME.begin()))>());

#define PROPERTY_ARRAY(PROPERTY_NAME,PROPERTY_NUM_NAME)\
	VisitProperty(PROPERTY_NAME,PROPERTY_NUM_NAME,pArg,boost::is_pod<boost::remove_pointer<BOOST_TYPEOF(PROPERTY_NAME)>::type>());

#define  __REGISTER_PROPERTY__(PROPERTY_NAME,PROPERTY_TYPE)\
	static __register_property__ reg##PROPERTY_NAME(__OFFSET__(classThis,PROPERTY_NAME),sizeof(PROPERTY_TYPE),#PROPERTY_NAME,classThis::GetThisClass());

#define  REGISTER_PROPERTY(CLASS_NAME, MEMBER_NAME, PROPERTY_NAME, PROPERTY_TYPE, DEFAULT_VALUE)\
	static __register_property__ reg##PROPERTY_NAME(__OFFSET__(CLASS_NAME, MEMBER_NAME), sizeof(PROPERTY_TYPE), #PROPERTY_NAME, CLASS_NAME::GetThisClass(), DEFAULT_VALUE);


#define REGISTER_PROPERTY_MINMAX(CLASS_NAME, MEMBER_NAME, PROPERTY_NAME, PROPERTY_TYPE, DEFAULT_VALUE, MIN_VALUE, MAX_VALUE)\
	static __register_property_minmax__ reg##PROPERTY_NAME(__OFFSET__(CLASS_NAME, MEMBER_NAME), sizeof(PROPERTY_TYPE), #PROPERTY_NAME, CLASS_NAME::GetThisClass(), DEFAULT_VALUE, MIN_VALUE, MAX_VALUE);





EFFBASE_END

#endif