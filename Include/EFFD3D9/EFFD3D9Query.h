/******************************************************************************
created:	20017-06-24   00:43
file path:	d:\EFF\Include\EFFD3D9Query.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFD3D9Query_2017_06_24__
#define __EFFD3D9Query_2017_06_24__

// Constants
static const effUINT QueryLatency = 3;

class EFFD3D9TimeQuery : public EFF3DTimeQuery
{
    friend class EFFD3D9Device;
public:
	EFFD3D9TimeQuery();
    virtual ~EFFD3D9TimeQuery();
public:
	virtual effVOID StartQuery();
	virtual effVOID EndQuery();
	virtual effBOOL EndFrame();
protected:
	effVOID Release();
	effVOID Init(LPDIRECT3DDEVICE9EX device);
protected:
	IDirect3DQuery9 * disjointQuery[QueryLatency];
	IDirect3DQuery9 * timestampStartQuery[QueryLatency];
	IDirect3DQuery9 * timestampEndQuery[QueryLatency];

	effBOOL queryStarted[QueryLatency];
	effBOOL queryFinished[QueryLatency];
};

#endif