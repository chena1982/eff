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





//资源分成数据资源和各种必须运行时创建的资源，比如在显卡中的贴图（不是指硬盘或内存里的贴图）
//VB，IB，RenderTarget等
//资源的加载分两步:
//   1.从内存或者硬盘读取数据
//   2.创建运行时需要的数据
class EFF3D_API EFF3DIResource : public EFFIUnknown
{
	RTTI_DECLARE_PURE(EFF3DIResource,EFFIUnknown)

	EFFIUNKNOWN_DECLARE

	BEGIN_INTERFACE_TABLE(EFF3DIResource)
		IMPLEMENTS_INTERFACE(EFFIUnknown)
	END_INTERFACE_TABLE()
public:

	virtual effVOID								SetOrigin(const effString & strOrigin) = 0;
	virtual const effString &				GetOrigin() const = 0;
	virtual effVOID								SetName(const effString & strName) = 0;
	virtual const effString &				GetName() const = 0;
	virtual effVOID								SetResourceManager(EFF3DResourceManager * pManager) = 0;
	virtual EFF3DResourceManager *	GetResourceManager() const = 0;
	virtual effVOID								SetUnloaded(effBOOL bUnloaded) = 0;
	virtual effBOOL								IsUnloaded() const = 0;

	virtual effDWORD							CalculateSize() = 0;
	virtual effDWORD							GetMemorySize() const = 0;
	virtual effDWORD							GetVideoMemorySize() const  = 0;
	virtual effDWORD							GetAGPMemorySize() const = 0;

	virtual effBOOL								LoadDataResourceFromFile(const effString & strFilePath) = 0;
	virtual effBOOL								CreateRuntimeResource() = 0;
	virtual effBOOL								Reload() = 0;
	virtual effVOID								Unload() = 0;

};

class EFF3D_API EFF3DResource : public EFF3DIResource
{



protected:
	EFF3DResource() : m_bUnloaded(effFALSE), m_dwMemorySize(0), m_dwVideoMemorySize(0),
					m_dwAGPMeorySize(0), m_pManager(NULL)
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




	effDWORD GetMemorySize() const
	{ 
		return m_dwMemorySize; 
	}

	effDWORD GetVideoMemorySize() const
	{
		return m_dwVideoMemorySize;
	}

	effDWORD GetAGPMemorySize() const
	{
		return m_dwAGPMeorySize;
	}





public:
	EFFEvent									OnBeforeCreate;
	EFFEvent									OnAfterCreate;

	EFFEvent									OnUnload;

protected:
	effString									m_strName;
	effString									m_strGroup;
	effBOOL									m_bUnloaded;
	effDWORD								m_dwMemorySize;
	effDWORD								m_dwVideoMemorySize;
	effDWORD								m_dwAGPMeorySize;
	effString									m_strOrigin;
	EFF3DResourceManager *			m_pManager;

};




EFF3D_END



#endif