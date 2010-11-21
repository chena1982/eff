/******************************************************************************
	created:	2008-12-16   23:24
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DResource.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DResource_2008_12_16__
#define __EFF3DResource_2008_12_16__

EFF3D_BEGIN


class EFF3DResourceManager;

class EFF3DDevice;



//��Դ�ֳ�������Դ�͸��ֱ�������ʱ��Դ(��������Ĳ�̫��)���������Կ��е���ͼ(����ָӲ�̻��ڴ������ͼ)
//VB��IB��RenderTarget��
//��Դ��ͬ�����ط�����:
//   1.���ڴ����Ӳ�̶�ȡ������Դ
//   2.��������ʱ��Դ
//��Щ��������Ⱦ�߳���ִ��

//��Դ���첽���ؾͱȽϸ�����
//   1.����һ��Request���͵�io�߳�
//   2.io�߳��ں�̨��ȡ������Դ
//   3.���ݴ����̶߳Ըն�ȡ����������Ҫ�Ĵ��������ѹ���ȵ�
//   4.�ٷ��͵���Ⱦ�̣߳���������ʱ��Դ������Դ��������ΪD3DҪ֧�ֶ��̻߳���ɺܴ�������½�(��������Ⱦ�̴߳�������������ʱ��Դ)
//   5.����б�Ҫ����io�߳�������ݿ���������ʱ��Դ��(���ʹ���ļ�ӳ��Ļ�����2��ʵ�ʲ�û��ִ��������io��
//      �����3��Ҳû��ִ���κβ�������ô������io������һ����ִ�У������б�Ҫ�����ݿ����ŵ�5ִ�У�������ֱ���ڵ�4��ִ��)
//   6.���ݿ������֮���������ʱ��Դ
//����ioֻ��һ���̣߳����ݴ����߳̿����ж��

class EFF3D_API EFF3DIResource : public EFFIUnknown
{
	RTTI_DECLARE_PURE(EFF3DIResource,EFFIUnknown)

	EFFIUNKNOWN_DECLARE

	BEGIN_INTERFACE_TABLE(EFF3DIResource)
		IMPLEMENTS_INTERFACE(EFFIUnknown)
	END_INTERFACE_TABLE()

	friend class EFF3DAsyncLoader;
public:

	virtual effVOID								SetOrigin(const effString & strOrigin) = 0;
	virtual const effString &				GetOrigin() const = 0;
	virtual effVOID								SetName(const effString & strName) = 0;
	virtual const effString &				GetName() const = 0;
	virtual effVOID								SetResourceManager(EFF3DResourceManager * pManager) = 0;
	virtual EFF3DResourceManager *	GetResourceManager() const = 0;
	virtual effVOID								SetUnloaded(effBOOL bUnloaded) = 0;
	virtual effBOOL								IsUnloaded() const = 0;
	virtual effBOOL								IsLoaded() const = 0;

	virtual effUINT								CalculateSize() = 0;
	virtual effUINT								GetMemorySize() const = 0;
	virtual effUINT								GetVideoMemorySize() const  = 0;
	virtual effUINT								GetAGPMemorySize() const = 0;

//	virtual effBOOL								CreateFromFile(const effString & strFilePath,EFF3DDevice * pDevice) = 0;
//	virtual effBOOL								AsyncCreateFromFile(const effString & strFilePath) = 0;
	virtual effBOOL								Reload() = 0;
	virtual effVOID								Unload() = 0;

protected:
	virtual effBOOL								LoadDataFromFile(const effString & strFilePath) = 0;
	virtual effBOOL								Process() = 0;
	virtual effBOOL								CreateRuntimeResource(EFF3DDevice * pDevice) = 0;

	virtual effHRESULT						Lock() = 0;
	virtual effBOOL								CopyDataToRuntimeResource() = 0;
	virtual effHRESULT						Unlock() = 0;

	virtual effVOID								LoadResourceError() = 0;
	virtual effVOID								LoadResourceEnd() = 0;
};

class EFF3D_API EFF3DResource : public EFF3DIResource
{



protected:
	EFF3DResource() : m_bUnloaded(effFALSE), m_bIsLoaded(effFALSE), m_dwMemorySize(0), m_dwVideoMemorySize(0),
					m_dwAGPMeorySize(0), m_pManager(NULL),m_pData(NULL),m_pRuntimeResourceLockPtr(NULL),
					m_uiDataSize(0)
	{ 
	}
public:
	virtual ~EFF3DResource() {}


public:



	effVOID SetOrigin(const effString & strOrigin)
	{
		m_strOrigin = strOrigin;
	}

	const effString & GetOrigin() const
	{
		return m_strOrigin;
	}

	effVOID SetName(const effString & strName)
	{
		m_strName = strName;
	}

	const effString & GetName() const 
	{ 
		return m_strName; 
	}

	effVOID SetResourceManager(EFF3DResourceManager * pManager)
	{
		m_pManager = pManager;
	}

	EFF3DResourceManager * GetResourceManager() const
	{
		return m_pManager;
	}

	effVOID SetUnloaded(effBOOL bUnloaded)
	{
		m_bUnloaded = bUnloaded;
	}

	effBOOL IsUnloaded() const
	{
		return m_bUnloaded;
	}


	effBOOL IsLoaded() const
	{
		return m_bIsLoaded;
	}

	effUINT GetMemorySize() const
	{ 
		return m_dwMemorySize; 
	}

	effUINT GetVideoMemorySize() const
	{
		return m_dwVideoMemorySize;
	}

	effUINT GetAGPMemorySize() const
	{
		return m_dwAGPMeorySize;
	}


protected:

	effBOOL									LoadDataFromFile(const effString & strFilePath) { return effTRUE; }
	effBOOL									Process() { return effTRUE; }
	effBOOL									CreateRuntimeResource(EFF3DDevice * pDevice) { return effTRUE; }

	effHRESULT								Lock() { return S_OK; }
	effBOOL									CopyDataToRuntimeResource() { return effTRUE; }
	effHRESULT								Unlock() { return S_OK; }

	effVOID									LoadResourceError() {}
	effVOID									LoadResourceEnd() { m_bIsLoaded = effTRUE; }
public:
	EFFEvent									OnBeforeCreate;
	EFFEvent									OnAfterCreate;

	EFFEvent									OnUnload;

protected:
	effString									m_strName;
	effString									m_strGroup;
	effBOOL									m_bUnloaded;
	effBOOL									m_bIsLoaded;
	effUINT									m_dwMemorySize;
	effUINT									m_dwVideoMemorySize;
	effUINT									m_dwAGPMeorySize;
	effString									m_strOrigin;
	EFF3DResourceManager *			m_pManager;
	effBYTE *									m_pData;
	effUINT									m_uiDataSize;
	effVOID *									m_pRuntimeResourceLockPtr;

};




EFF3D_END



#endif