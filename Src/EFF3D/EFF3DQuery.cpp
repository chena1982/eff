/******************************************************************************
created:	20017-06-24   00:13
file path:	d:\EFF\Include\EFF3DQuery.cpp
author:		ChenA

purpose:
******************************************************************************/


#include "EFF3DPCH.h"
#include "EFF3DDevice.h"
#include "EFF3DQuery.h"
#include "EFF3DTexture.h"

//#define new EFFNEW


EFF3D_BEGIN


EFF3DGPUProfiler EFF3DGPUProfiler::GlobalProfiler;

EFF3DGPUProfiler::EFF3DGPUProfiler()
{
    currFrame = 0;
}

EFF3DGPUProfiler::~EFF3DGPUProfiler()
{

}


effVOID EFF3DGPUProfiler::StartProfile(const effString & name)
{

    ProfileData & profileData = profiles[name];
    _ASSERT(profileData.QueryStarted == effFALSE);
    _ASSERT(profileData.QueryFinished == effFALSE);

    EFF3DDevice * device = EFF3DGetDevice();

    if (profileData.DisjointQuery[currFrame] == NULL)
    {
        // Create the queries
        //device->CreateQuery(EFF3DQUERYTYPE_TIMESTAMPDISJOINT, 0, &profileData.DisjointQuery[currFrame]);

        //device->CreateQuery(EFF3DQUERYTYPE_TIMESTAMP, 0, &profileData.TimestampStartQuery[currFrame]);
        //device->CreateQuery(EFF3DQUERYTYPE_TIMESTAMP, 0, &profileData.TimestampEndQuery[currFrame]);
    }

    // Start a disjoint query first
    //profileData.DisjointQuery[currFrame]->Issue(EFF3DISSUE_BEGIN);

    // Insert the start timestamp    
    //profileData.TimestampStartQuery[currFrame]->Issue(EFF3DISSUE_END);

    profileData.QueryStarted = effTRUE;
}

effVOID EFF3DGPUProfiler::EndProfile(const effString & name)
{


    ProfileData& profileData = profiles[name];
    _ASSERT(profileData.QueryStarted == effTRUE);
    _ASSERT(profileData.QueryFinished == effFALSE);

    // Insert the end timestamp    
    //profileData.TimestampEndQuery[currFrame]->Issue(EFF3DISSUE_END);

    // End the disjoint query
    //profileData.DisjointQuery[currFrame]->Issue(EFF3DISSUE_END);

    profileData.QueryStarted = effFALSE;
    profileData.QueryFinished = effTRUE;
}

effVOID EFF3DGPUProfiler::EndFrame()
{


    currFrame = (currFrame + 1) % QueryLatency;


    float queryTime = 0.0f;

    // Iterate over all of the profiles
    ProfileMap::iterator iter;
    for (iter = profiles.begin(); iter != profiles.end(); iter++)
    {
        ProfileData& profile = (*iter).second;
        if (profile.QueryFinished == FALSE)
            continue;

        profile.QueryFinished = FALSE;

        if (profile.DisjointQuery[currFrame] == NULL)
            continue;


        // Get the query data
        effUINT64 startTime = 0;
        while (profile.TimestampStartQuery[currFrame]->GetData(&startTime, sizeof(startTime), 0) != S_OK);

        effUINT64 endTime = 0;
        while (profile.TimestampEndQuery[currFrame]->GetData(&endTime, sizeof(endTime), 0) != S_OK);

        effBOOL disjoint;
        while (profile.DisjointQuery[currFrame]->GetData(&disjoint, sizeof(disjoint), 0) != S_OK);


        if (!disjoint)
        {
            EFF3DDevice * device = EFF3DGetDevice();
            if ((*iter).first == _effT("Render") && !device->IsHost())
            {
                device->GetSharedRenderTarget()->NotifyHostStartRendering(currFrame);
            }

            UINT64 delta = endTime - startTime;
            //float frequency = static_cast<float>(disjointData.Frequency);
            //time = (delta / frequency) * 1000.0f;
        }
    }
}

// == ProfileBlock ================================================================================

EFF3DGPUProfileBlock::EFF3DGPUProfileBlock(const effString & name) : name(name)
{
    EFF3DGPUProfiler::GlobalProfiler.StartProfile(name);
}

EFF3DGPUProfileBlock::~EFF3DGPUProfileBlock()
{
    EFF3DGPUProfiler::GlobalProfiler.EndProfile(name);
}


EFF3D_END