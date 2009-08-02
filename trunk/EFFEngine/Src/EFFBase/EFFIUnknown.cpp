/******************************************************************************
	created:	2008-12-10   0:56
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFIUnknown.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFIUnknown.h"

#define new EFFNEW

EFFBASE_BEGIN

EFFRunTimeTypeInfoImplPureVirtual<EFFNullClass> EFFNullClass::runtimeInfoClassEFFNullClass = EFFRunTimeTypeInfoImplPureVirtual<EFFNullClass>(0,"EFFNullClass",NULL);

EFFClass * EFFNullClass::GetThisClass()
{
	return RTTI_CLASS(EFFNullClass);
}

EFFClass * __stdcall EFFNullClass::GetRuntimeClass() const
{
	return GetThisClass();
}

RTTI_IMPLEMENT_PURE(EFFIUnknown,0)


effHRESULT __stdcall InterfaceTableQueryInterface(effVOID *pThis, const INTERFACE_ENTRY *pTable, const ClassID & riid, effVOID **ppv)
{
	if ( riid == EFFIUnknown::GetThisClass()->GetID() )
	{
		((EFFIUnknown*)(*ppv = (effCHAR*)pThis + pTable->dwData))->AddRef();
		return S_OK;
	}
	else
	{
		effHRESULT hr = E_NOINTERFACE;
		while (pTable->pfnFinder)
		{
			if ( !pTable->pIID || riid == *pTable->pIID )
			{
				if ( pTable->pfnFinder == ENTRY_IS_OFFSET )
				{
					((EFFIUnknown*)(*ppv = (effCHAR*)pThis + pTable->dwData))->AddRef();
					hr = S_OK;
					break;
				}
				else
				{
					hr = pTable->pfnFinder(pThis, pTable->dwData, riid, ppv);
					if (hr == S_OK)
						break;
				}
			}
			pTable++;
		}
		if (hr != S_OK)
			*ppv = 0;
		return hr;
	}
}

EFFBASE_END