/******************************************************************************
	created:	2008-12-17   0:15
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DResourceManager.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DResourceManager_2008_12_17__
#define __EFF3DResourceManager_2008_12_17__

EFF3D_BEGIN

class EFF3DResource;
class EFF3DDevice;

#define DECLARE_CREATE_FROM_FILE(N)\
template<__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __NOTHING__)>\
EFF3DIResource *		CreateFromFile(const effString & strFilePath,__REPEAT(N, __ARG__, __COMMA__, __NOTHING__));


#define DECLARE_CREATE_FROM_FILE_IMPL(N)\
template<__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __NOTHING__)>\
EFF3DIResource *		CreateFromFileImpl(const effString & strFilePath,__REPEAT(N, __ARG__, __COMMA__, __NOTHING__));

class EFF3D_API EFF3DResourceManager : public EFFObjectManager
{
	RTTI_DECLARE(EFF3DResourceManager, EFFObjectManager)
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

	/*template<typename T0>
	EFF3DResource * CreateFromFile(const effString & filePath, T0 t0)
	{
		effString name;
		effINT pos = filePath.rfind('\\');
		if ( pos != -1 )
		{
			name = filePath.substr(pos, filePath.length() - pos);
		}

		ResourceMap::iterator it = resources.find(name);
		if ( it != resources.end() )
		{
			it->second->AddRef();
			return it->second;
		}

		EFF3DResource * ret = NULL;
		OnCreateFromFile(&filePath, t0, &ret);

		if ( ret != NULL )
		{
			AddFirstCreateResource(ret);
		}

		return ret;
	}

	template<typename T0, typename T1>
	EFF3DResource * CreateFromFile(const effString & filePath, T0 t0, T1 t1)
	{
		effString name;
		effINT pos = filePath.rfind('\\');
		if ( pos != -1 )
		{
			name = filePath.substr(pos, filePath.length() - pos);
		}

		ResourceMap::iterator it = m_mapResources.find(name);
		if ( it != m_mapResources.end() )
		{
			it->second->AddRef();
			return it->second;
		}

		EFF3DResource * ret = NULL;
		OnCreateFromFile(strFilePath, t0, t1, &ret);
		if ( ret != NULL )
		{
			AddFirstCreateResource(ret);
		}

		return ret;
	}*/

	//DECLARE_CREATE_FROM_FILE_IMPL(0)
	//DECLARE_CREATE_FROM_FILE_IMPL(1)
	//DECLARE_CREATE_FROM_FILE_IMPL(2)
public:
	EFF3DResource *					AsyncCreateFromFile(const effString & filePath, EFF3DRESOURCETYPE resourceType, EFF3DDevice * device);
protected:
	effVOID							AddResource(EFF3DResource * res);
	//effVOID							AddResource(EFF3DResource * res);

	EFF3DResource *					GetResource(const effString & filePath);

public:
	typedef MAP<effString, EFF3DResource *>	ResourceMap;
	//typedef std::map<effULONG, EFF3DResource *>		ResourceIdMap;
protected:
	ResourceMap						resources;
	//ResourceIdMap					resourcesId;

	effUINT							memoryUsed;
	effUINT							videoMemoryUsed;
	effUINT							AGPMemroyUsed;


	EFFEvent						OnCreateFromFile;
	EFFEvent						OnAsyncCreateFromFile;
};


EFF3D_END

#endif