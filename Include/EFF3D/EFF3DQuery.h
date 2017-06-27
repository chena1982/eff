/******************************************************************************
created:	20017-06-24   00:13
file path:	d:\EFF\Include\EFF3DQuery.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFF3DQuery_2017_06_24__
#define __EFF3DQuery_2017_06_24__





EFF3D_BEGIN

class EFF3DQuery : public EFFReferenceCount
{
protected:
    EFF3DQuery() {};
    virtual ~EFF3DQuery() {};
public:

    virtual effHRESULT Issue(effUINT flag) = 0;
    virtual effHRESULT GetData(effVOID * data, effUINT size, effUINT flag) = 0;
};

class EFF3DGPUProfiler
{

public:

    EFF3DGPUProfiler();
    ~EFF3DGPUProfiler();

    static EFF3D_API EFF3DGPUProfiler GlobalProfiler;

public:
    effVOID StartProfile(const effString & name);
    effVOID EndProfile(const effString & name);

    effVOID EFF3D_API EndFrame();

protected:

    // Constants
    static const effUINT QueryLatency = 3;

    struct ProfileData
    {
        EFF3DQuery * DisjointQuery[QueryLatency];
        EFF3DQuery * TimestampStartQuery[QueryLatency];
        EFF3DQuery * TimestampEndQuery[QueryLatency];

        effBOOL QueryStarted;
        effBOOL QueryFinished;

        ProfileData()
        {
            QueryStarted = effFALSE;
            QueryFinished = effFALSE;

            for (effUINT i = 0; i < QueryLatency; i++)
            {
                DisjointQuery[i] = NULL;
                TimestampStartQuery[i] = NULL;
                TimestampEndQuery[i] = NULL;
            }
        }
    };

    typedef std::map<effString, ProfileData> ProfileMap;

    ProfileMap profiles;
    effUINT currFrame;
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