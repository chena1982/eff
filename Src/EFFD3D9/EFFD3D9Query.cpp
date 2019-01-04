/******************************************************************************
created:	20017-06-24   00:43
file path:	d:\EFF\Include\EFFD3D9Query.h
author:		ChenA

purpose:
******************************************************************************/



#include "EFFD3D9PCH.h"
#include "EFFD3D9Query.h"

EFFD3D9TimeQuery::EFFD3D9TimeQuery()
{
	for (effINT i = 0; i < QueryLatency; i++)
	{
		disjointQuery[i] = NULL;
		timestampStartQuery[i] = NULL;
		timestampEndQuery[i] = NULL;

		queryStarted[i] = effFALSE;
		queryFinished[i] = effFALSE;
	}
}


EFFD3D9TimeQuery::~EFFD3D9TimeQuery()
{
	Release();
}

effVOID EFFD3D9TimeQuery::StartQuery()
{
	_ASSERT(queryStarted[currentFrame] == effFALSE);
	_ASSERT(queryFinished[currentFrame] == effFALSE);

	// Start a disjoint query first
	disjointQuery[currentFrame]->Issue(D3DISSUE_BEGIN);

	// Insert the start timestamp    
	timestampStartQuery[currentFrame]->Issue(D3DISSUE_END);

	queryStarted[currentFrame] = effTRUE;
}

effVOID EFFD3D9TimeQuery::EndQuery()
{
	_ASSERT(queryStarted[currentFrame] == effTRUE);
	_ASSERT(queryFinished[currentFrame] == effFALSE);

	// Insert the end timestamp    
	timestampEndQuery[currentFrame]->Issue(D3DISSUE_END);

	// End the disjoint query
	disjointQuery[currentFrame]->Issue(D3DISSUE_END);

	queryStarted[currentFrame] = effFALSE;
	queryFinished[currentFrame] = effTRUE;
}

effBOOL EFFD3D9TimeQuery::EndFrame()
{
	effINT lastFrame = (currentFrame + 1) % QueryLatency;

	currentFrame = (currentFrame + 1) % QueryLatency;

	if (!queryFinished[lastFrame])
	{
		return effFALSE;
	}

	queryFinished[lastFrame] = effFALSE;

	// Get the query data
	while (timestampStartQuery[lastFrame]->GetData(&startTime, sizeof(startTime), 0) != S_OK);
	while (timestampEndQuery[lastFrame]->GetData(&endTime, sizeof(endTime), 0) != S_OK);
	while (disjointQuery[lastFrame]->GetData(&disjoint, sizeof(disjoint), 0) != S_OK);




	return effTRUE;
}

effVOID EFFD3D9TimeQuery::Init(LPDIRECT3DDEVICE9EX device)
{
	Release();

	for (effINT i = 0; i < 3; i++)
	{
		device->CreateQuery(D3DQUERYTYPE_TIMESTAMPDISJOINT, &disjointQuery[i]);
		device->CreateQuery(D3DQUERYTYPE_TIMESTAMP, &timestampStartQuery[i]);
		device->CreateQuery(D3DQUERYTYPE_TIMESTAMP, &timestampEndQuery[i]);
	}

}

effVOID EFFD3D9TimeQuery::Release()
{
	for (effINT i = 0; i < QueryLatency; i++)
	{
		SF_RELEASE(disjointQuery[i]);
		SF_RELEASE(timestampStartQuery[i]);
		SF_RELEASE(timestampEndQuery[i]);
	}
}