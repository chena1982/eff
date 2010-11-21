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



//资源分成数据资源和各种必须运行时资源(这名字起的不太好)，比如在显卡中的贴图(不是指硬盘或内存里的贴图)
//VB，IB，RenderTarget等
//资源的同步加载分两步:
//   1.从内存或者硬盘读取数据资源
//   2.创建运行时资源
//这些都是在渲染线程里执行

//资源的异步加载就比较复杂了
//   1.创建一个Request发送到io线程
//   2.io线程在后台读取数据资源
//   3.数据处理线程对刚读取的数据做必要的处理，比如解压缩等等
//   4.再发送到渲染线程，创建运行时资源并把资源锁定，因为D3D要支持多线程会造成很大的性能下降(所以在渲染线程创建并锁定运行时资源)
//   5.如果有必要，在io线程里把数据拷贝到运行时资源，(如果使用文件映射的话，第2步实际并没有执行真正的io，
//      如果第3步也没有执行任何操作，那么真正的io将在这一步骤执行，所以有必要把数据拷贝放到5执行，而不是直接在第4步执行)
//   6.数据拷贝完成之后解锁运行时资源
//其中io只有一个线程，数据处理线程可以有多个

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