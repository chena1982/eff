#pragma once

#pragma warning(disable:4251)

#include "EFFSerialize.h"
#include "boost/type_traits.hpp"

class EFFClass;

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
#define __TEMPLATE_ARG__(N)  class T##N
#define __TYPE_ARG__(N)      T##N 
#define __ARG__(N)           T##N t##N
#define __PARAM__(N)         t##N
#define __NOT_VIRTUAL__
#define __ADD_METHOD_ARGS__(N) m_vArgsName.push_back(getClassNameFromTypeName(typeid(T##N).name()));
#define __SEMICOLON__   ;

class __callable__
{
public:
	virtual ~__callable__() {}

	
	std::string m_strMethodName;
	std::string m_strReturnTypeName;
	std::vector<std::string> m_vArgsName;
};


#define __CALLABLE__(N)\
template <class R, class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
class __callable##N##__ : public __callable__\
{\
public:\
	typedef R (C::*MethodType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__));\
	MethodType method;\
	__callable##N##__(MethodType m) : method(m)\
	{\
		m_strReturnTypeName = getClassNameFromTypeName(typeid(R).name());								/*得到返回值类型*/  \
		__REPEAT(N,__ADD_METHOD_ARGS__,__SEMICOLON__,__NOTHING__);			/*得到参数类型*/  \
	}\
	R invoke(C *object __REPEAT(N, __ARG__, __COMMA__, __COMMA__)) const		/*invoke函数*/  \
	{\
		return (object->*method)(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\
	}\
};

__CALLABLE__(0)
__CALLABLE__(1)
__CALLABLE__(2)


#define __CREATE_CALLABLE__(N)\
template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
static inline __callable##N##__<R, C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__)> * create(R (C::*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__)))\
{\
	return new __callable##N##__<R, C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__)>(method);\
}



template <class R>
class __callable_factory__
{
public:
	__CREATE_CALLABLE__(0)
	__CREATE_CALLABLE__(1)
	__CREATE_CALLABLE__(2)
};


#define __MEMBER_METHOD__(N)\
template <class R,class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
	__member_method__(R (C::*memberMethodAddress)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__)),char * pszMethodName)\
{\
	__callable__ * pMethod = __callable_factory__<R>::create(memberMethodAddress);\
	pMethod->m_strMethodName = pszMethodName;\
	C::GetThisClass()->AddMemberMethod(pMethod);\
}\

class __member_method__
{
public:
	__MEMBER_METHOD__(0)
	__MEMBER_METHOD__(1)
	__MEMBER_METHOD__(2)
};

#define __REGISTER_MEMBER_METHOD__(CLASS_NAME,METHOD_NAME)\
static __member_method__ reg##METHOD_NAME(&##CLASS_NAME##::##METHOD_NAME,#METHOD_NAME);

//#define MEMBER_METHOD(N,METHOD_NAME) __MEMBER_METHOD__(N,METHOD_NAME,__LINE__)

//#define __REGISTER_MEMBER_METHOD__()





class __property__
{
public:
	__property__(unsigned long ulOffset,unsigned long ulSize,const char * pszName) : m_ulOffset(ulOffset),m_ulSize(ulSize),m_strName(pszName) {}

	unsigned long		m_ulOffset;
	unsigned long		m_ulSize;
	std::string				m_strName;
};

class EFFBASE_API __register_property__ 
{
public:
	__register_property__(unsigned long ulOffset,unsigned long ulSize,const char * pszName,EFFClass * pClass);
};



#define __PROPERTY__(PROPERTY_NAME,PROPERTY_TYPE)\
	VisitMeta<PROPERTY_TYPE>(PROPERTY_NAME,arg,boost::is_pod<PROPERTY_TYPE>());

#define __PROPERTY_ARRAY__(PROPERTY_NAME,PROPERTY_ELEMENT_TYPE)\
	VisitMeta<PROPERTY_ELEMENT_TYPE>(PROPERTY_NAME,arg,boost::is_pod<PROPERTY_ELEMENT_TYPE>());

#define  __REGISTER_PROPERTY__(CLASS_NAME,PROPERTY_NAME,PROPERTY_TYPE)\
	static __register_property__ reg##PROPERTY_NAME(__OFFSET__(CLASS_NAME,PROPERTY_NAME),sizeof(PROPERTY_TYPE),#PROPERTY_NAME,CLASS_NAME##::GetThisClass());


#define __EFFEVENT_MEMBER_FUCTION__(N)\
template <class R,class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
EFFEvent(R (C::*memberFuctionAddress)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__)))\
{\
	m_pFunction = __callable_factory__<R>::create(memberFuctionAddress);\
}


#define __EFFEVENT_INVOKE__(N)\
template <class R, class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
void operator ()(R & result, C *object __REPEAT(N, __ARG__, __COMMA__, __COMMA__)) const\
{\
	typedef __callable##N##__<R, C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__)> CallableType1;\
	CallableType1 *cb1 = dynamic_cast<CallableType1 *>(m_pFunction);\
	if ( cb1 != NULL )\
	{\
		result = cb1->invoke(object,__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\
		return;\
	}\
	else\
	{\
		printf("callable is null.\n");\
	}\
}

class EFFEvent
{
public:
	//__EFFEVENT_MEMBER_FUCTION__(0)
	//__INVOKE__(0)
	__EFFEVENT_MEMBER_FUCTION__(1)
	__EFFEVENT_INVOKE__(1)
	__EFFEVENT_MEMBER_FUCTION__(2)
	__EFFEVENT_INVOKE__(2)
	__callable__ * m_pFunction;
};





/*#define EFFEvent_Constructor(N)\
	template <class R,class C,class T0>\
	EFFEvent(R (C::*)(T0) a)\
{\
	boost::function<R(C*,T0)> function;\
	function = a;\
}*/

/*class EFFEvent
{
public:
	template <class R,class C,class T0>
	EFFEvent(R (C::*a)(T0) )
	{
		boost::function<R(C*,T0)> function;
		function = a;
	}
};*/


