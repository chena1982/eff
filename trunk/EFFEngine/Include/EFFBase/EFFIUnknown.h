/******************************************************************************
	created:	2008-12-1   23:33
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFIUnknown.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFIUnknown_2008_12_1__
#define __EFFIUnknown_2008_12_1__

#include "EFFRtti.h"

EFFBASE_BEGIN

class EFFClass;

class EFFNullClass
{
public:
	   friend EFFRunTimeTypeInfoImplPureVirtual<EFFNullClass>;
	   static EFFRunTimeTypeInfoImplPureVirtual<EFFNullClass> runtimeInfoClassEFFNullClass;
	   static EFFClass* GetThisClass();
	   virtual EFFClass * __stdcall GetRuntimeClass() const;
};



class EFFBASE_API EFFIUnknown
{
	RTTI_DECLARE_PURE(EFFIUnknown,EFFNullClass)

public:
	virtual effHRESULT __stdcall QueryInterface(const ClassID & classID,effVOID ** ppInterface) = 0;
	virtual effULONG __stdcall AddRef() = 0;
	virtual effULONG __stdcall Release() = 0;
	virtual effULONG __stdcall GetObjectID() = 0;
	virtual effVOID __stdcall SetObjectID(effULONG ulObjectId) = 0;
	virtual effHRESULT __stdcall AddComponent(const ClassID & classID,const ClassID & interfaceID,effVOID ** ppInterface) = 0;
	//virtual EFFClass * _stdcall GetRuntimeClass() const = 0;
protected:
private:
};


typedef effHRESULT (__stdcall *INTERFACE_FINDER)(effVOID *pThis, effDWORD dwData, const ClassID & riid, effVOID **ppv);
#define ENTRY_IS_OFFSET INTERFACE_FINDER(-1)

typedef struct _INTERFACE_ENTRY
{
	const ClassID * pIID;           // the IID to match
	INTERFACE_FINDER pfnFinder; // finder function
	effULONG        dwData;         // aux data for finder function
} INTERFACE_ENTRY;


#define BASE_OFFSET(ClassName, BaseName) \
	(effDWORD(static_cast<BaseName*>(reinterpret_cast<ClassName*>(0x10000000))) - 0x10000000)

#define COMPOSITE_OFFSET(ClassName, BaseName, MemberType, MemberName) \
	(effDWORD(static_cast<BaseName*>(reinterpret_cast<MemberType*>(0x10000000 + offsetof(ClassName, MemberName)))) - 0x10000000)

#define BEGIN_INTERFACE_TABLE(ClassName) \
typedef ClassName _InterfaceTableClassName;\
static const INTERFACE_ENTRY *GetInterfaceTable(void) \
{\
	static const INTERFACE_ENTRY table[] =\
	{\

#define IMPLEMENTS_INTERFACE(ItfName) \
	{ &(ItfName::GetThisClass()->GetID()), ENTRY_IS_OFFSET,  BASE_OFFSET(_InterfaceTableClassName, ItfName) },


#define END_INTERFACE_TABLE() \
		{ 0, 0, 0 }\
	};\
	return table;\
}

effHRESULT EFFBASE_API __stdcall InterfaceTableQueryInterface(effVOID *pThis, const INTERFACE_ENTRY *pTable, const ClassID & riid, effVOID **ppv);

#define EFFIUNKNOWN_DECLARE \
public:\
	virtual effHRESULT __stdcall		QueryInterface(const ClassID & classID,effVOID ** ppInterface);\
	virtual effULONG __stdcall			AddRef();\
	virtual effULONG __stdcall			Release();\
	virtual effULONG __stdcall			GetObjectID() { return m_ulObjectId; }\
	virtual effVOID __stdcall			SetObjectID(effULONG ulObjectId) { m_ulObjectId = ulObjectId; }\
	virtual effHRESULT __stdcall		AddComponent(const ClassID & classID,const ClassID & interfaceID,effVOID ** ppInterface);\
protected:\
	effULONG										m_ulObjectId;\
	effULONG										m_ulRefCount;\
	std::vector<EFFIUnknown *>		m_aryDynamicComponent;
	//��̬��ӵ������һ�㶼��ʹ����������ʵ�ָ������������ȽϹ̶��Ŀ�����������ʵ�ָ������
	//�����кܶ�����Ҫ��̬�޸ĵģ���������ʱ���������������ģ���ʱ�ö�̬��ӵ�����ȽϺ�
	//Ŀǰ��ʱ��Ƴ�һ���ض����ֻ�����һ��


//������ʱ�и����⣬���һ��Object������������̬����������������̬�����֧��ͬһ���ӿڣ���ô
//�޷��жϷ����ĸ��ӿ�

#define EFFIUNKNOWN_IMPLEMENT(CLASS)\
effHRESULT __stdcall CLASS::QueryInterface(const ClassID & classID,effVOID ** ppInterface)\
{\
    if ( InterfaceTableQueryInterface(this, GetInterfaceTable(), classID, ppInterface) == E_NOINTERFACE )\
	{\
		for ( effUINT i = 0; i < m_aryDynamicComponent.size(); i++ )\
		{\
			if ( m_aryDynamicComponent[i]->QueryInterface(classID,ppInterface) == S_OK )\
			{\
				return S_OK;\
			}\
		}\
		return E_NOINTERFACE;\
	}\
	return S_OK;\
}\
effULONG __stdcall CLASS::AddRef()\
{\
	return InterlockedIncrement((effLONG *)&m_ulRefCount);\
}\
effULONG __stdcall CLASS::Release()\
{\
	effULONG ulRefCount = InterlockedDecrement((effLONG *)&m_ulRefCount);\
	if ( ulRefCount == 0 )\
	{\
		delete this;\
	}\
	return ulRefCount;\
}\
effHRESULT __stdcall CLASS::AddComponent(const ClassID & classID,const ClassID & interfaceID,effVOID ** ppInterface)\
{\
	EFFIUnknown * pUnknown = static_cast<EFFIUnknown *>(EFFCreateObject(classID));\
	if ( pUnknown == NULL )\
	{\
		return E_FAIL;\
	}\
	m_aryDynamicComponent.push_back(pUnknown);\
	return pUnknown->QueryInterface(interfaceID,ppInterface);\
}

EFFBASE_END

#endif