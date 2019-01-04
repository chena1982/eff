/******************************************************************************
created:	20017-06-24   00:13
file path:	d:\EFF\Include\EFF3DQuery.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFF3DQuery_2017_06_24__
#define __EFF3DQuery_2017_06_24__


#include "EFF3DResource.h"
#include "EFF3DResourceManager.h"


EFF3D_BEGIN

class EFF3D_API EFF3DTimeQuery : public EFF3DResource
{
	friend class EFF3DGPUProfiler;
protected:
	EFF3DTimeQuery();
    virtual ~EFF3DTimeQuery() {};
public:
	virtual effBOOL LoadDataFromFile(const effString & filePath);
    virtual effVOID StartQuery() = 0;
	virtual effVOID EndQuery() = 0;
	virtual effBOOL EndFrame() = 0;

public:
	effUINT currentFrame;
protected:
	effUINT64 startTime;
	effUINT64 endTime;
	effBOOL disjoint;
};

class EFF3D_API EFF3DTimeQueryManager : public EFF3DResourceManager
{
	friend class EFF3DDevice;
protected:
	EFF3DTimeQueryManager();
	virtual ~EFF3DTimeQueryManager();
public:
	virtual effBOOL CreateFromFileImpl(const effString & strFilePath, EFF3DResource * resource, EFF3DResourceType resourceType);
};

class EFF3D_API EFF3DGPUProfiler
{

public:

    EFF3DGPUProfiler();
    ~EFF3DGPUProfiler();

    static EFF3DGPUProfiler GlobalProfiler;

public:
    effVOID StartProfile(const effString & name);
    effVOID EndProfile(const effString & name);
    EFF3DTimeQuery * EndFrame();

protected:
    typedef MAP<effString, EFF3DTimeQuery *> ProfileMap;
    ProfileMap profiles;
};


class EFF3D_API EFF3DGPUProfileBlock
{
public:

    EFF3DGPUProfileBlock(const effString & name);
    ~EFF3DGPUProfileBlock();

protected:

    effString name;
};

EFF3D_END



#endif