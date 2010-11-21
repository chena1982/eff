/******************************************************************************
	created:	2008-12-11   0:42
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFIProxy.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFIProxy_2008_12_11__
#define __EFFIProxy_2008_12_11__


#include "EFFReflection.h"

EFFBASE_BEGIN



template<class T>
effVOID ChangeVFT(T * pT,effULONG ulFunctionIndex,effUINT newFunctionAddress)
{
	effUINT *lpVtabl = *(effUINT **)pT;
	lpVtabl += ulFunctionIndex;

	HANDLE hProcess = GetCurrentProcess();

	if ( hProcess )
	{
		effUINT dwOldProtect;
		if ( VirtualProtectEx(hProcess,lpVtabl,4,PAGE_READWRITE,(PDWORD)&dwOldProtect) )
		{
			*lpVtabl = newFunctionAddress;
			VirtualProtectEx(hProcess, lpVtabl, 4, dwOldProtect, NULL);
		}
	}

}

#define  PROXY_DECLARE(CLASS)\
class CLASS##Proxy : public CLASS




#define REGISTER_PROXY_FUNCTION(FUNCTION_INDEX,NEW_FUNCTION_NAME)\
	effUINT newFunctionAddress = (effUINT)NEW_FUNCTION_NAME;\
	ChangeVFT(pThis,FUNCTION_INDEX,newFunctionAddress);

#define BEGIN_REGISTER_PROXY_FUNCTION \
public:\
	template<class T>\
	static effVOID RegisterProxyFunction(T * pThis)\
	{

#define END_REGISTER_PROXY_FUNCTION\
	}

#define PROXY_CONSTRUCTOR(CLASS)\
	CLASS##Proxy::CLASS##Proxy()\
	{\
		class CLASS##RegisterProxyFunction\
		{\
		public:\
			CLASS##RegisterProxyFunction(CLASS * pThis)\
			{\
				CLASS##Proxy::RegisterProxyFunction(pThis);\
			}\
		};\
		static CLASS##RegisterProxyFunction _##CLASS##RegisterProxyFunction(this);\
	}

/*#define PROXY_IMPLEMENT(CLASS)\
	class CLASS##RegisterProxyFunction\
	{\
	public:\
		CLASS##RegisterProxyFunction()\
		{\
			CLASS##Proxy::RegisterProxyFunction();\
		}\
	};\
	static CLASS##RegisterProxyFunction _##CLASS##RegisterProxyFunction;*/


EFFBASE_END

#endif