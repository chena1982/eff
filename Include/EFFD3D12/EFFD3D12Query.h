/******************************************************************************
created:	2019-03-23   00:41
file path:	d:\EFF\Include\EFFD3D12Query.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFD3D12Query_2019_03_23__
#define __EFFD3D12Query_2019_03_23__

// Constants
static const effUINT QueryLatency = 3;

class EFFD3D12TimeQuery : public EFF3DTimeQuery
{
    friend class EFFD3D12Device;
public:
	EFFD3D12TimeQuery();
    virtual ~EFFD3D12TimeQuery();
public:
	virtual effVOID StartQuery();
	virtual effVOID EndQuery();
	virtual effBOOL EndFrame();
protected:
	effVOID Release();
	effVOID Init();
protected:
	//IDirect3DQuery9 * disjointQuery[QueryLatency];
	//IDirect3DQuery9 * timestampStartQuery[QueryLatency];
	//IDirect3DQuery9 * timestampEndQuery[QueryLatency];

	effBOOL queryStarted[QueryLatency];
	effBOOL queryFinished[QueryLatency];
};

#endif