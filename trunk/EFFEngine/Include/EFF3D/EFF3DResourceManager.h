/******************************************************************************
	created:	2008-12-17   0:15
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DResourceManager.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DResourceManager_2008_12_17__
#define __EFF3DResourceManager_2008_12_17__

EFF3D_BEGIN

class EFF3DIResource;

#define DECLARE_CREATE_FROM_FILE(N)\
template<__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __NOTHING__)>\
EFF3DIResource *		CreateFromFile(const effString & strFilePath,__REPEAT(N, __ARG__, __COMMA__, __NOTHING__));


#define DECLARE_CREATE_FROM_FILE_IMPL(N)\
template<__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __NOTHING__)>\
EFF3DIResource *		CreateFromFileImpl(const effString & strFilePath,__REPEAT(N, __ARG__, __COMMA__, __NOTHING__));

class EFF3D_API EFF3DResourceManager
{
public:
	EFF3DResourceManager();
	virtual ~EFF3DResourceManager();

public:
	//EFF3DIResource *					CreateFromFile(const effString & strFilePath);
	//virtual EFF3DIResource *		CreateFromFileImpl(const effString & strFilePath) = 0;
	//DECLARE_CREATE_FROM_FILE(0)
	//DECLARE_CREATE_FROM_FILE(1)
	//DECLARE_CREATE_FROM_FILE(2)

	//为了便于调试，这里不用宏来生成代码，手动把宏展开

	template<class T0>
	EFF3DIResource * CreateFromFile(const effString & strFilePath,T0 t0)
	{
		ResourceMap::iterator it = m_mapResources.find(strFilePath);
		if ( it != m_mapResources.end() )
		{
			it->second->AddRef();
			return it->second;
		}

		EFF3DIResource * ret = NULL;
		//OnCreateFromFile(strFilePath, t0);
		if ( ret != NULL )
		{
			AddFirstCreateResource(ret);
		}

		return ret;
	}

	template<class T0,class T1>
	EFF3DIResource * CreateFromFile(const effString & strFilePath,T0 t0,T1 t1)
	{
		ResourceMap::iterator it = m_mapResources.find(strFilePath);
		if ( it != m_mapResources.end() )
		{
			it->second->AddRef();
			return it->second;
		}

		EFF3DIResource * ret = NULL;
		OnCreateFromFile(strFilePath, t0, t1, &ret);
		if ( ret != NULL )
		{
			AddFirstCreateResource(ret);
		}

		return ret;
	}


	//DECLARE_CREATE_FROM_FILE_IMPL(0)
	//DECLARE_CREATE_FROM_FILE_IMPL(1)
	//DECLARE_CREATE_FROM_FILE_IMPL(2)

protected:
	effVOID								AddFirstCreateResource(EFF3DIResource * res);
	effVOID								AddResource(EFF3DIResource * res);
	effVOID								CalculateNextId();
public:
	typedef std::map<effString,EFF3DIResource *>			ResourceMap;
	typedef std::map<effULONG,EFF3DIResource *>		ResourceIdMap;
protected:
	ResourceMap						m_mapResources;
	ResourceIdMap						m_mapResourcesById;
	effULONG								m_ulCurrentId;
	effDWORD							m_dwMemoryUsage;
	effDWORD							m_dwVideoMemoryUsage;
	effDWORD							m_dwAGPMemroyUsage;
	std::vector<effULONG>		m_aryRecoveredId;
	effBOOL								m_bLastIdIsRecovered;
	EFFEvent								OnCreateFromFile;
};


EFF3D_END

#endif