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


EFF3DTimeQuery::EFF3DTimeQuery()
{
	startTime = 0;
	endTime = 0;
	disjoint = effFALSE;

	currentFrame = 0;
}

effBOOL EFF3DTimeQuery::LoadDataFromFile(const effString & filePath)
{
	return effTRUE;
}


EFF3DTimeQueryManager::EFF3DTimeQueryManager() : EFF3DResourceManager()
{

}

EFF3DTimeQueryManager::~EFF3DTimeQueryManager()
{

}

effBOOL EFF3DTimeQueryManager::CreateFromFileImpl(const effString & strFilePath, EFF3DResource * resource, EFF3DResourceType resourceType)
{
	return effTRUE;
}

EFF3DGPUProfiler::EFF3DGPUProfiler()
{

}

EFF3DGPUProfiler::~EFF3DGPUProfiler()
{

}


effVOID EFF3DGPUProfiler::StartProfile(const effString & name)
{
    EFF3DTimeQuery * timeQuery = profiles[name];
	if (timeQuery == NULL)
	{
		EFF3DTimeQueryHandle timeQueryHandle;
		EFF3DGetDevice()->CreateTimeQuery(0, &timeQueryHandle);
		timeQuery = (EFF3DTimeQuery *)EFF3DGetDevice()->GetTimeQueryManager()->GetResource(timeQueryHandle);
		profiles[name] = timeQuery;
	}

	timeQuery->StartQuery();
}

effVOID EFF3DGPUProfiler::EndProfile(const effString & name)
{

	EFF3DTimeQuery * timeQuery = profiles[name];

	timeQuery->EndQuery();

}

EFF3DTimeQuery * EFF3DGPUProfiler::EndFrame()
{
    float queryTime = 0.0f;

    // Iterate over all of the profiles
    ProfileMap::iterator iter;
    for (iter = profiles.begin(); iter != profiles.end(); iter++)
    {
		EFF3DTimeQuery * timeQuery = (*iter).second;

		if (!timeQuery->EndFrame())
		{
			continue;
		}

		if (!timeQuery->disjoint)
		{
			if ((*iter).first == _effT("Render"))
			{
				return timeQuery;
			}

			//UINT64 delta = timeQuery->endTime - timeQuery->startTime;
			//float frequency = static_cast<float>(disjointData.Frequency);
			//time = (delta / frequency) * 1000.0f;
		}
    }

	return NULL;
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