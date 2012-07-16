/********************************************************************
	created:	9:12:2009   21:09
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DAsyncLoader.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"

#include <process.h>
#include "EFF3DResource.h"
#include "EFF3DAsyncLoader.h"

////#define new EFFNEW



EFF3D_BEGIN

//--------------------------------------------------------------------------------------
effUINT WINAPI _FileIOThreadProc(effVOID * lpParameter)
{
	return ((EFF3DAsyncLoader *)lpParameter)->FileIOThreadProc();
}

//--------------------------------------------------------------------------------------
effUINT WINAPI _ProcessingThreadProc(effVOID * lpParameter)
{
	return ((EFF3DAsyncLoader *)lpParameter)->ProcessingThreadProc();
}

//--------------------------------------------------------------------------------------
// WarmIOCache tells the virtual memory subsystem to prefetch pages for this chunk
// of memory.  By touching 1 effBYTE in every 4k of data, we can ensure that each page
// we need is loaded into memory.
//--------------------------------------------------------------------------------------
effVOID WarmIOCache(effBYTE* pData, effUINT size)
{
	// read one effBYTE in every 4k page in order to force all of the pages to load
	effUINT start = 0;
	effBYTE byteTemp = 0;

	while( start < size )
	{
		byteTemp = pData[ start ];
		start += 4096;
	}
}

//--------------------------------------------------------------------------------------
EFF3DAsyncLoader::EFF3DAsyncLoader(EFF3DDevice * pDevice,effUINT NumProcessingThreads) : done(false),
processThreadDone(false),
IOThreadDone(false),
toServiceResourcesCount(0),
oustandingResourcesCount(0),
IOQueueSemaphore(0),
processQueueSemaphore(0),
IOThread(0),
processingThreadsCount(0),
processThreads(NULL),
device(pDevice)
{
	InitAsyncLoadingThreadObjects( NumProcessingThreads );
	IORequestsCount = 0;
	processRequestsCount = 0;
}

//--------------------------------------------------------------------------------------
EFF3DAsyncLoader::~EFF3DAsyncLoader()
{
	DestroyAsyncLoadingThreadObjects();
}

//--------------------------------------------------------------------------------------
// Add a work item to the queue of work items
//--------------------------------------------------------------------------------------
effBOOL EFF3DAsyncLoader::AddWorkItem(EFF3DResource * resource, effBOOL * result)
{
	if ( resource == NULL )
	{
		return effFALSE;
	}

	RESOURCE_REQUEST ResourceRequest;
	ResourceRequest.resource = resource;
	ResourceRequest.result = result;
	ResourceRequest.copy = false;
	ResourceRequest.lock = false;
	ResourceRequest.error = false;


	// Add the request to the read queue
	EnterCriticalSection( &IOQueueCriticalSection );
	IOQueue.push_back( ResourceRequest );
	LeaveCriticalSection( &IOQueueCriticalSection );

	// TODO: critsec around this?
	oustandingResourcesCount ++;

	// Signal that we have something to read
	ReleaseSemaphore( IOQueueSemaphore, 1, NULL );

	return effTRUE;
}

//--------------------------------------------------------------------------------------
// Wait for all work in the queues to finish
//--------------------------------------------------------------------------------------
effVOID EFF3DAsyncLoader::WaitForAllItems()
{
	ProcessDeviceWorkItems( UINT_MAX, FALSE );

	for(; ; )
	{
		// Only exit when all resources are loaded
		if( 0 == oustandingResourcesCount )
			return;

		// Service Queues
		ProcessDeviceWorkItems( UINT_MAX, FALSE );
		Sleep( 100 );
	}
}

//--------------------------------------------------------------------------------------
// FileIOThreadProc
//
// This is the one IO threadproc.  This function is responsible for processing read
// requests made by the application.  There should only be one IO thread per device.  
// This ensures that the disk is only trying to read one part of the disk at a time.
//
// This thread performs double-duty as the copy thread as well.  It manages the copying
// of resource data from temporary system memory buffer (or memory mapped pointer) into
// the locked data of the resource.
//--------------------------------------------------------------------------------------
effUINT EFF3DAsyncLoader::FileIOThreadProc()
{
	effWCHAR szMessage[MAX_PATH];
	IOThreadDone = effFALSE;

	RESOURCE_REQUEST ResourceRequest = {0};

	while( !done )
	{
		// Wait for a read or create request
		WaitForSingleObject(IOQueueSemaphore, INFINITE);
		if( done )
		{
			break;
		}

		IORequestsCount ++;

		// Pop a request off of the IOQueue
		EnterCriticalSection(&IOQueueCriticalSection);
		ResourceRequest = IOQueue[0];
		IOQueue.erase(IOQueue.begin());
		LeaveCriticalSection(&IOQueueCriticalSection);

		// Handle a read request
		if( !ResourceRequest.copy )
		{
			if( !ResourceRequest.error )
			{
				// Load the data
				effBOOL result = ResourceRequest.resource->LoadDataFromFile(ResourceRequest.filePath);

				if ( !result )
				{
					swprintf_s(szMessage, MAX_PATH, L"FileIO Error");
					OutputDebugString(szMessage);

					ResourceRequest.error = effTRUE;
					if( ResourceRequest.result != NULL )
					{
						*ResourceRequest.result = result;
					}
				}
			}

			// Add it to the ProcessQueue
			EnterCriticalSection(&processQueueCriticalSection);
			processQueue.push_back(ResourceRequest);
			LeaveCriticalSection(&processQueueCriticalSection);

			// Let the process thread know it's got work to do
			ReleaseSemaphore(processQueueSemaphore, 1, NULL);
		}

		// Handle a copy request
		else
		{
			if( !ResourceRequest.error )
			{
				// Create the data
				effBOOL result = ResourceRequest.resource->CopyDataToRuntimeResource();

				if( !result )
				{
					OutputDebugString(L"Failed to Copy Data to Device Object.\n");

					ResourceRequest.error = effTRUE;
					if( ResourceRequest.result != NULL )
					{
						*ResourceRequest.result = result;
					}
				}
			}
			else
			{
				ResourceRequest.resource->LoadResourceError();
			}

			// send an unlock request
			ResourceRequest.lock = effFALSE;
			EnterCriticalSection( &renderThreadQueueCriticalSection );
			enderThreadQueue.push_back( ResourceRequest );
			LeaveCriticalSection( &renderThreadQueueCriticalSection );
		}
	}

	IOThreadDone = effTRUE;
	return 0;
}

//--------------------------------------------------------------------------------------
// ProcessingThreadProc
// 
// This is the threadproc for the processing thread.  There are multiple processing
// threads.  The job of the processing thread is to uncompress, unpack, or otherwise
// manipulate the data loaded by the loading thread in order to get it ready for the
// ProcessDeviceWorkItems function in the graphics thread to lock or unlock the resource.
//--------------------------------------------------------------------------------------
effUINT EFF3DAsyncLoader::ProcessingThreadProc()
{
	effWCHAR szMessage[MAX_PATH];

	processThreadDone = effFALSE;
	while( !done )
	{
		// Acquire ProcessQueueSemaphore
		WaitForSingleObject( processQueueSemaphore, INFINITE );
		if( done )
		{
			break;
		}

		processRequestsCount ++;

		// Pop a request off of the ProcessQueue
		EnterCriticalSection( &processQueueCriticalSection );
		RESOURCE_REQUEST ResourceRequest = processQueue[0];
		processQueue.erase(processQueue.begin());
		LeaveCriticalSection( &processQueueCriticalSection );

		effBOOL result;
		// Decompress the data
		if( !ResourceRequest.error )
		{
			result = ResourceRequest.resource->Process();
		}

		if( !result )
		{
			swprintf_s(szMessage, MAX_PATH, L"Processing Thread Error.\n");
			OutputDebugString(szMessage);

			ResourceRequest.error = effTRUE;
			if( ResourceRequest.result != NULL )
			{
				*ResourceRequest.result = result;
			}
		}

		// Add it to the RenderThreadQueue
		ResourceRequest.lock = effTRUE;
		EnterCriticalSection( &renderThreadQueueCriticalSection );
		enderThreadQueue.push_back( ResourceRequest );
		LeaveCriticalSection( &renderThreadQueueCriticalSection );
	}

	processThreadDone = effTRUE;
	return 0;
}

//--------------------------------------------------------------------------------------
// Create 1 IO thread and multiple processing threads to handle all of our background
// data loading.
//--------------------------------------------------------------------------------------
effBOOL EFF3DAsyncLoader::InitAsyncLoadingThreadObjects(effUINT NumProcessingThreads)
{
	LONG MaxSemaphoreCount = LONG_MAX;

	// Create 2 semaphores
	IOQueueSemaphore = CreateSemaphore(NULL, 0, MaxSemaphoreCount, NULL);
	processQueueSemaphore = CreateSemaphore(NULL, 0, MaxSemaphoreCount, NULL);

	// Create the queue critical sections
	InitializeCriticalSection(&IOQueueCriticalSection);
	InitializeCriticalSection(&processQueueCriticalSection);
	InitializeCriticalSection(&renderThreadQueueCriticalSection);

	// Create the Processing threads
	processingThreadsCount = NumProcessingThreads;
	processThreads = EFFNEW effHANDLE[processingThreadsCount];
	
	if( !processThreads )
	{
		return effFALSE;
	}

	for( effUINT i = 0; i < processingThreadsCount; i++ )
	{
		processThreads[i] = (effHANDLE)_beginthreadex(NULL, 0, _ProcessingThreadProc, (effVOID *)this,
			CREATE_SUSPENDED, NULL);
		// we would set thread affinity here if we wanted to lock this thread to a processor
		ResumeThread(processThreads[i]);
	}

	// Create the IO thread
	IOThread = (effHANDLE)_beginthreadex(NULL, 0, _FileIOThreadProc, (effVOID *)this, CREATE_SUSPENDED, NULL);
	// we would set thread affinity here if we wanted to lock this thread to a processor
	ResumeThread(IOThread);

	return true;
}

//--------------------------------------------------------------------------------------
// Destroy
//--------------------------------------------------------------------------------------
effVOID EFF3DAsyncLoader::DestroyAsyncLoadingThreadObjects()
{
	done = effTRUE;

	ReleaseSemaphore(IOQueueSemaphore, 1, NULL);
	ReleaseSemaphore(processQueueSemaphore, 1, NULL);

	while( !IOThreadDone || !processThreadDone )
	{
		Sleep( 100 );
	}

	CloseHandle(IOQueueSemaphore);
	CloseHandle(processQueueSemaphore);

	DeleteCriticalSection(&IOQueueCriticalSection);
	DeleteCriticalSection(&processQueueCriticalSection);
	DeleteCriticalSection(&renderThreadQueueCriticalSection);

	for( effUINT i = 0; i < processingThreadsCount; i++ )
	{
		CloseHandle(processThreads[i]);
	}

	CloseHandle(IOThread);
}

//--------------------------------------------------------------------------------------
// ProcessDeviceWorkItems is called by the graphics thread.  Depending on the request
// it either Locks or Unlocks a resource (or calls UpdateSubresource for D3D10).  One of
// of the arguments is the number of resources to service.  This ensure that no matter
// how many items are in the queue, the graphics thread doesn't stall trying to process
// all of them.
//--------------------------------------------------------------------------------------
effVOID EFF3DAsyncLoader::ProcessDeviceWorkItems(effUINT currentToServiceResourcesNum, effBOOL retryLoads )
{
	if ( enderThreadQueue.size() == 0 )
	{
		return;
	}

	EnterCriticalSection( &renderThreadQueueCriticalSection );
	effUINT numJobs = enderThreadQueue.size();
	LeaveCriticalSection( &renderThreadQueueCriticalSection );

	for( effUINT i = 0; i < numJobs && i < currentToServiceResourcesNum; i++ )
	{
		EnterCriticalSection( &renderThreadQueueCriticalSection );
		RESOURCE_REQUEST ResourceRequest = enderThreadQueue[0];
		enderThreadQueue.erase(enderThreadQueue.begin());
		LeaveCriticalSection( &renderThreadQueueCriticalSection );

		if( ResourceRequest.lock )
		{
			if( !ResourceRequest.error )
			{
				effBOOL fail = effFALSE;
				effBOOL result = effTRUE;
				if ( ResourceRequest.resource->CreateRuntimeResource(device) )
				{
					result = ResourceRequest.resource->Lock();

					if( !result && retryLoads )
					{
						// add it back to the list
						EnterCriticalSection( &renderThreadQueueCriticalSection );
						enderThreadQueue.push_back( ResourceRequest );
						LeaveCriticalSection( &renderThreadQueueCriticalSection );

						// move on to the next guy
						continue;
					}
					else if( !result )
					{
						fail = effTRUE;
					}
				}
				else
				{
					fail = effTRUE;
				}

				if ( fail )
				{
					ResourceRequest.error = effTRUE;
					if( ResourceRequest.result != NULL )
					{
						*ResourceRequest.result = result;
					}
				}

			}

			ResourceRequest.copy = effTRUE;
			EnterCriticalSection(&IOQueueCriticalSection);
			IOQueue.push_back(ResourceRequest);
			LeaveCriticalSection(&IOQueueCriticalSection);

			// Signal that we have something to copy
			ReleaseSemaphore(IOQueueSemaphore, 1, NULL);
		}
		else
		{
			if( !ResourceRequest.error )
			{
				effBOOL result = ResourceRequest.resource->Unlock();
				if( ResourceRequest.result )
				{
					*ResourceRequest.result = result;
				}
			}

			// Decrement num oustanding resources
			oustandingResourcesCount--;
		}
	}
}


EFF3D_END