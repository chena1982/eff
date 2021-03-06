/******************************************************************************
	created:	2008-12-15   2:20
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFEvent.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFEvent_2008_12_15__
#define __EFFEvent_2008_12_15__

//#include <boost/function.hpp>
//#include <boost/python.hpp>

#include "EFFReflection.h"

EFFBASE_BEGIN


#define __EFFEVENTCALL_MEMBER_FUNCTION__(N)\
template <class R, class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
EFFEventCall(C * pDelegatee, R (C::*memberFunctionAddress)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__)))\
{\
	m_pFunction = __callable_factory__<R>::create(memberFunctionAddress);\
	m_pDelegatee = (__delegatee__ *)pDelegatee;\
}


#define __EFFEVENTCALL_FUNCTION__(N)\
template <class R __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
EFFEventCall(R (*functionAddress)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__)))\
{\
	m_pFunction = __callable_factory__<R>::create(functionAddress);\
	m_pDelegatee = NULL;\
}

/*#define __EFFEVENTCALL_INVOKE__(N)\
template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
void operator ()(C *object __REPEAT(N, __ARG__, __COMMA__, __COMMA__)) const\
{\
	typedef __callable##N##__<void, C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__)> CallableType;\
	CallableType *cb1 = dynamic_cast<CallableType *>(m_pFunction);\
	UCEventMFR * pMFR = (UCEventMFR *)object;\
	if ( cb1 != NULL )\
	{\
		return (pMFR->*(cb1->method))(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\
	}\
	else\
	{\
		printf("callable is null.\n");\
	}\
}*/

/*#define __EFFEVENTCALL_INVOKE__(N)\
template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
void operator ()(C *object __REPEAT(N, __ARG__, __COMMA__, __COMMA__)) const\
{\
	typedef __callable##N##__<void,__delegatee__ __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__)> CallableType;\
	CallableType *cb1 = dynamic_cast<CallableType *>(m_pFunction);\
	if ( cb1 != NULL )\
	{\
		return (m_pDelegatee->*(cb1->method))(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\
	}\
	else\
	{\
		printf("callable is null.\n");\
	}\
}*/

//using namespace boost::python;

	/*if ( m_pPyFunction != NULL )\
	{\
		if ( m_pPyDelegatee != NULL )\
		{\
			call_method<void,__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__)>(m_pPyDelegatee,m_pPyFunctionName,__REPEAT(N, __PYTHON_PARAM__, __COMMA__, __NOTHING__));\
		}\
		else\
		{\
			call<void>(m_pPyFunction,__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\
		}\
	}\
	else\*/

#ifdef EFF_PLATFORM_WIN64

#define __EFFEVENTCALL_INVOKE__(N)\
template <__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __NOTHING__)>\
void operator ()(__REPEAT(N, __ARG__, __COMMA__, __NOTHING__)) const\
{\
	if ( m_pDelegatee == NULL )\
	{\
		typedef __callable##N##__<void __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__)> CallableType;\
		CallableType * cb = (CallableType *)m_pFunction;\
		cb->invoke(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\
	}\
	else\
	{\
		typedef void (__delegatee__::*MethodType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__));\
		effBYTE * pBaseAddress = (effBYTE *)m_pFunction;\
		pBaseAddress += 16;\
		MethodType m = *(MethodType *)((effVOID *)pBaseAddress);\
		(m_pDelegatee->*m)(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\
	}\
}

#else

#define __EFFEVENTCALL_INVOKE__(N)\
template <__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __NOTHING__)>\
void operator ()(__REPEAT(N, __ARG__, __COMMA__, __NOTHING__)) const\
{\
	if ( m_pDelegatee == NULL )\
	{\
		typedef __callable##N##__<void __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__)> CallableType;\
		CallableType * cb = (CallableType *)m_pFunction;\
		cb->invoke(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\
	}\
	else\
	{\
		typedef void (__delegatee__::*MethodType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__));\
		effBYTE * pBaseAddress = (effBYTE *)m_pFunction;\
		pBaseAddress += 8;\
		MethodType m = *(MethodType *)((effVOID *)pBaseAddress);\
		(m_pDelegatee->*m)(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\
	}\
}

#endif

// typedef void (__delegatee__::*MethodType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__));\
// effBYTE * pBaseAddress = (effBYTE *)m_pFunction;\
// pBaseAddress += 8;\
// MethodType m = *(MethodType *)((effVOID *)pBaseAddress);\
// (m_pDelegatee->*m)(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\

/*#define  __EFFEVENTCALL_CONSTRUCTOR__(N)\
template <class R,class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
EFFEventCall(__delegatee__ * pDelegatee,__callable##N##__<R,C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)> * pFunction)\
{\
	typedef R (C::*MethodTypeReal)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__));\
	MethodTypeReal address = (MethodTypeReal)(pFunction->method);\
	m_pFunction = __callable_factory__<R>::create(address);\
	m_pDelegatee = (__delegatee__ *)pDelegatee;\
	m_pPyDelegatee = NULL;\
	m_pPyFunction = NULL;\
}*/

class EFFEventCall
{
public:
	EFFEventCall()
	{
		//m_pPyDelegatee = NULL;
		//m_pPyFunction = NULL;
		m_pFunction = NULL;
		m_pDelegatee = NULL;
	}

	~EFFEventCall()
	{
		SF_DELETE(m_pFunction);
	}

	/*EFFEventCall(PyObject * pPyFunction)
	{
		m_pPyDelegatee = NULL;
		m_pPyFunction = pPyFunction;
		m_pFunction = NULL;
		m_pDelegatee = NULL;
	}

	EFFEventCall(PyObject * pPyDelegatee,char * pszPyFunctionName)
	{
		m_pPyDelegatee = pPyDelegatee;
		m_pPyFunctionName = pszPyFunctionName;
		m_pFunction = NULL;
		m_pDelegatee = NULL;
	}*/

	EFFEventCall(EFFEventCall & eventCall)
	{
		//this->m_pPyDelegatee = eventCall.m_pPyDelegatee;
		//this->m_pPyFunction = eventCall.m_pPyFunction;
		this->m_pFunction = eventCall.m_pFunction;
		this->m_pDelegatee = eventCall.m_pDelegatee;

		eventCall.m_pFunction = NULL;
		eventCall.m_pDelegatee = NULL;
	}
public:

	__EFFEVENTCALL_MEMBER_FUNCTION__(0)
    __EFFEVENTCALL_FUNCTION__(0)

	//0个参数的时候不能特化，因为没有模板参数
	void operator () () const
	{
		if ( m_pDelegatee == NULL )
		{
			typedef __callable0__<void> CallableType;
			CallableType * cb = (CallableType *)m_pFunction;
			cb->invoke();
		}
		else
		{
			typedef void (__delegatee__::*MethodType)();
			effBYTE * pBaseAddress = (effBYTE *)m_pFunction;

			#ifdef EFF_PLATFORM_WIN64
				pBaseAddress += 16;
			#else
				pBaseAddress += 8;
			#endif

			MethodType m = *(MethodType *)((effVOID *)pBaseAddress);
			(m_pDelegatee->*m)();
		}
	}



	__EFFEVENTCALL_MEMBER_FUNCTION__(1)
	__EFFEVENTCALL_FUNCTION__(1)
	__EFFEVENTCALL_INVOKE__(1)

	__EFFEVENTCALL_MEMBER_FUNCTION__(2)
	__EFFEVENTCALL_INVOKE__(2)
	__EFFEVENTCALL_MEMBER_FUNCTION__(3)
	__EFFEVENTCALL_INVOKE__(3)


	/*template<typename T0>
	void operator () (T0 t0)
	{
		if ( m_pDelegatee == NULL )
		{
			typedef __callable1__<void, T0> CallableType;
			CallableType * cb = (CallableType *)m_pFunction;
			cb->invoke(t0);
		}
		else
		{
			typedef void (__delegatee__::*MethodType)(T0);
			effBYTE * pBaseAddress = (effBYTE *)m_pFunction;
			pBaseAddress += 8;
			MethodType m = *(MethodType *)((effVOID *)pBaseAddress);
			(m_pDelegatee->*m)(t0);
		}
	}*/


	__callable__ *		m_pFunction;
	__delegatee__ *		m_pDelegatee;

	/*PyObject *			m_pPyDelegatee;
	union
	{
		PyObject *		m_pPyFunction;
		char *			m_pPyFunctionName;
	};*/


};


/*#define __EFFEVENT_MEMBER_FUCTION__(N)\
template <class R,class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__)>\
void operator += (C * pObject,R (C::*memberFuctionAddress)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__)))\
{\
	EFFEventCall * pEventCall = new EFFEventCall(pObject,memberFuctionAddress);\
}*/


#define __EFFEVENT_INVOKE__(N)\
template <__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __NOTHING__)>\
void operator () (__REPEAT(N, __ARG__, __COMMA__, __NOTHING__)) const\
{\
	for ( effUINT i = 0; i < m_aryFunction.size(); i++ )\
	{\
		(*m_aryFunction[i])(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__));\
	}\
}

class EFFBASE_API EFFEvent
{
public:
	EFFEvent();
	~EFFEvent();

	//0个参数的时候不能特化，因为没有模板参数
	effVOID operator () () const
	{
		for ( effUINT i = 0; i < m_aryFunction.size(); i++ )
		{
			(*m_aryFunction[i])();
		}
	}

	//__EFFEVENT_INVOKE__(0)
	__EFFEVENT_INVOKE__(1)
	__EFFEVENT_INVOKE__(2)
	__EFFEVENT_INVOKE__(3)



	effVOID operator += (EFFEventCall & eventCall)
	{
		EFFEventCall * pEventCall = new EFFEventCall(eventCall);
		m_aryFunction.push_back(pEventCall);
	}

	/*effVOID AddScriptEventCall(PyObject * pPyFunction)
	{
		EFFEventCall * pEventCall = new EFFEventCall(pPyFunction);
		m_aryFunction.push_back(pEventCall);
	}

	effVOID AddScriptMemberEventCall(PyObject * pPyDelegatee,char * pszPyFunctionName)
	{
		EFFEventCall * pEventCall = new EFFEventCall(pPyDelegatee,pszPyFunctionName);
		m_aryFunction.push_back(pEventCall);
	}*/

	VECTOR<EFFEventCall *> m_aryFunction;

};


EFFBASE_END

#endif