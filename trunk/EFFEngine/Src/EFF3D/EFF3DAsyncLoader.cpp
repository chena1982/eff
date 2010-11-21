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

#define new EFFNEW



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
EFF3DAsyncLoader::EFF3DAsyncLoader(EFF3DDevice * pDevice,effUINT NumProcessingThreads) : m_bDone(false),
m_bProcessThreadDone(false),
m_bIOThreadDone(false),
m_NumResourcesToService(0),
m_NumOustandingResources(0),
m_hIOQueueSemaphore(0),
m_hProcessQueueSemaphore(0),
m_hIOThread(0),
m_NumProcessingThreads(0),
m_phProcessThreads(NULL),
m_pDevice(pDevice)
{
	InitAsyncLoadingThreadObjects( NumProcessingThreads );
	m_NumIORequests = 0;
	m_NumProcessRequests = 0;
}

//--------------------------------------------------------------------------------------
EFF3DAsyncLoader::~EFF3DAsyncLoader()
{
	DestroyAsyncLoadingThreadObjects();
}

//--------------------------------------------------------------------------------------
// Add a work item to the queue of work items
//--------------------------------------------------------------------------------------
effHRESULT EFF3DAsyncLoader::AddWorkItem(EFF3DIResource * pResource, effHRESULT* pHResult)
{
	if ( pResource == NULL )
	{
		return E_FAIL;
	}

	RESOURCE_REQUEST ResourceRequest;
	ResourceRequest.pResource = pResource;
	ResourceRequest.pHR = pHResult;
	ResourceRequest.bCopy = false;
	ResourceRequest.bLock = false;
	ResourceRequest.bError = false;


	// Add the request to the read queue
	EnterCriticalSection( &m_csIOQueue );
	m_IOQueue.push_back( ResourceRequest );
	LeaveCriticalSection( &m_csIOQueue );

	// TODO: critsec around this?
	m_NumOustandingResources ++;

	// Signal that we have something to read
	ReleaseSemaphore( m_hIOQueueSemaphore, 1, NULL );

	return S_OK;
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
		if( 0 == m_NumOustandingResources )
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
	effHRESULT hr = S_OK;
	m_bIOThreadDone = false;

	RESOURCE_REQUEST ResourceRequest = {0};

	while( !m_bDone )
	{
		// Wait for a read or create request
		WaitForSingleObject( m_hIOQueueSemaphore, INFINITE );
		if( m_bDone )
			break;

		m_NumIORequests ++;

		// Pop a request off of the IOQueue
		EnterCriticalSection( &m_csIOQueue );
		ResourceRequest = m_IOQueue[0];
		m_IOQueue.erase(m_IOQueue.begin());
		LeaveCriticalSection( &m_csIOQueue );

		// Handle a read request
		if( !ResourceRequest.bCopy )
		{
			if( !ResourceRequest.bError )
			{
				// Load the data
				hr = ResourceRequest.pResource->LoadDataFromFile(ResourceRequest.strFilePath);

				if( FAILED( hr ) )
				{
					swprintf_s( szMessage, MAX_PATH, L"FileIO Error: hr = %x\n", hr );
					OutputDebugString( szMessage );

					ResourceRequest.bError = true;
					if( ResourceRequest.pHR )
						*ResourceRequest.pHR = hr;
				}
			}

			// Add it to the ProcessQueue
			EnterCriticalSection( &m_csProcessQueue );
			m_ProcessQueue.push_back( ResourceRequest );
			LeaveCriticalSection( &m_csProcessQueue );

			// Let the process thread know it's got work to do
			ReleaseSemaphore( m_hProcessQueueSemaphore, 1, NULL );
		}

		// Handle a copy request
		else
		{
			if( !ResourceRequest.bError )
			{
				// Create the data
				hr = ResourceRequest.pResource->CopyDataToRuntimeResource();

				if( FAILED( hr ) )
				{
					OutputDebugString( L"Failed to Copy Data to Device Object\n" );

					ResourceRequest.bError = true;
					if( ResourceRequest.pHR )
						*ResourceRequest.pHR = hr;
				}
			}
			else
			{
				ResourceRequest.pResource->LoadResourceError();
			}

			// send an unlock request
			ResourceRequest.bLock = false;
			EnterCriticalSection( &m_csRenderThreadQueue );
			m_RenderThreadQueue.push_back( ResourceRequest );
			LeaveCriticalSection( &m_csRenderThreadQueue );
		}
	}
	m_bIOThreadDone = true;
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

	effHRESULT hr = S_OK;
	m_bProcessThreadDone = false;
	while( !m_bDone )
	{
		// Acquire ProcessQueueSemaphore
		WaitForSingleObject( m_hProcessQueueSemaphore, INFINITE );
		if( m_bDone )
			break;

		m_NumProcessRequests ++;

		// Pop a request off of the ProcessQueue
		EnterCriticalSection( &m_csProcessQueue );
		RESOURCE_REQUEST ResourceRequest = m_ProcessQueue[0];
		m_ProcessQueue.erase(m_ProcessQueue.begin());
		LeaveCriticalSection( &m_csProcessQueue );

		// Decompress the data
		if( !ResourceRequest.bError )
		{
			hr = ResourceRequest.pResource->Process();
		}

		if( FAILED(hr) )
		{
			swprintf_s( szMessage, MAX_PATH, L"Processing Thread Error: hr = %x\n", hr );
			OutputDebugString( szMessage );

			ResourceRequest.bError = true;
			if( ResourceRequest.pHR )
				*ResourceRequest.pHR = hr;
		}

		// Add it to the RenderThreadQueue
		ResourceRequest.bLock = true;
		EnterCriticalSection( &m_csRenderThreadQueue );
		m_RenderThreadQueue.push_back( ResourceRequest );
		LeaveCriticalSection( &m_csRenderThreadQueue );
	}

	m_bProcessThreadDone = true;
	return 0;
}

//--------------------------------------------------------------------------------------
// Create 1 IO thread and multiple processing threads to handle all of our background
// data loading.
//--------------------------------------------------------------------------------------
bool EFF3DAsyncLoader::InitAsyncLoadingThreadObjects(effUINT NumProcessingThreads)
{
	LONG MaxSemaphoreCount = LONG_MAX;

	// Create 2 semaphores
	m_hIOQueueSemaphore = CreateSemaphore(NULL, 0, MaxSemaphoreCount, NULL);
	m_hProcessQueueSemaphore = CreateSemaphore(NULL, 0, MaxSemaphoreCount, NULL);

	// Create the queue critical sections
	InitializeCriticalSection(&m_csIOQueue);
	InitializeCriticalSection(&m_csProcessQueue);
	InitializeCriticalSection(&m_csRenderThreadQueue);

	// Create the Processing threads
	m_NumProcessingThreads = NumProcessingThreads;
	m_phProcessThreads = new effHANDLE[m_NumProcessingThreads];
	
	if( !m_phProcessThreads )
	{
		return false;
	}

	for( effUINT i = 0; i < m_NumProcessingThreads; i++ )
	{
		m_phProcessThreads[i] = (effHANDLE)_beginthreadex(NULL, 0, _ProcessingThreadProc, (effVOID *)this,
			CREATE_SUSPENDED, NULL);
		// we would set thread affinity here if we wanted to lock this thread to a processor
		ResumeThread(m_phProcessThreads[i]);
	}

	// Create the IO thread
	m_hIOThread = (effHANDLE)_beginthreadex(NULL, 0, _FileIOThreadProc, (effVOID *)this, CREATE_SUSPENDED, NULL);
	// we would set thread affinity here if we wanted to lock this thread to a processor
	ResumeThread(m_hIOThread);

	return true;
}

//--------------------------------------------------------------------------------------
// Destroy
//--------------------------------------------------------------------------------------
effVOID EFF3DAsyncLoader::DestroyAsyncLoadingThreadObjects()
{
	m_bDone = true;

	ReleaseSemaphore( m_hIOQueueSemaphore, 1, NULL );
	ReleaseSemaphore( m_hProcessQueueSemaphore, 1, NULL );

	while( !m_bIOThreadDone || !m_bProcessThreadDone )
	{
		Sleep( 100 );
	}

	CloseHandle( m_hIOQueueSemaphore );
	CloseHandle( m_hProcessQueueSemaphore );

	DeleteCriticalSection( &m_csIOQueue );
	DeleteCriticalSection( &m_csProcessQueue );
	DeleteCriticalSection( &m_csRenderThreadQueue );

	for( effUINT i = 0; i < m_NumProcessingThreads; i++ )
		CloseHandle( m_phProcessThreads[i] );

	CloseHandle( m_hIOThread );
}

//--------------------------------------------------------------------------------------
// ProcessDeviceWorkItems is called by the graphics thread.  Depending on the request
// it either Locks or Unlocks a resource (or calls UpdateSubresource for D3D10).  One of
// of the arguments is the number of resources to service.  This ensure that no matter
// how many items are in the queue, the graphics thread doesn't stall trying to process
// all of them.
//--------------------------------------------------------------------------------------
effVOID EFF3DAsyncLoader::ProcessDeviceWorkItems( effUINT CurrentNumResourcesToService, BOOL bRetryLoads )
{
	effHRESULT hr = S_OK;

	if ( m_RenderThreadQueue.size() == 0 )
	{
		return;
	}

	EnterCriticalSection( &m_csRenderThreadQueue );
	effUINT numJobs = m_RenderThreadQueue.size();
	LeaveCriticalSection( &m_csRenderThreadQueue );

	for( effUINT i = 0; i < numJobs && i < CurrentNumResourcesToService; i++ )
	{
		EnterCriticalSection( &m_csRenderThreadQueue );
		RESOURCE_REQUEST ResourceRequest = m_RenderThreadQueue[0];
		m_RenderThreadQueue.erase(m_RenderThreadQueue.begin());
		LeaveCriticalSection( &m_csRenderThreadQueue );

		if( ResourceRequest.bLock )
		{
			if( !ResourceRequest.bError )
			{
				bool bFail = false;
				if ( ResourceRequest.pResource->CreateRuntimeResource(m_pDevice) )
				{
					hr = ResourceRequest.pResource->Lock();

					if( FAILED(hr) && bRetryLoads )
					{
						// add it back to the list
						EnterCriticalSection( &m_csRenderThreadQueue );
						m_RenderThreadQueue.push_back( ResourceRequest );
						LeaveCriticalSection( &m_csRenderThreadQueue );

						// move on to the next guy
						continue;
					}
					else if( FAILED(hr) )
					{
						bFail = true;
					}
				}
				else
				{
					bFail = true;
				}

				if ( bFail )
				{
					ResourceRequest.bError = true;
					if( ResourceRequest.pHR )
						*ResourceRequest.pHR = hr;
				}

			}

			ResourceRequest.bCopy = true;
			EnterCriticalSection( &m_csIOQueue );
			m_IOQueue.push_back( ResourceRequest );
			LeaveCriticalSection( &m_csIOQueue );

			// Signal that we have something to copy
			ReleaseSemaphore( m_hIOQueueSemaphore, 1, NULL );
		}
		else
		{
			if( !ResourceRequest.bError )
			{
				HRESULT hr = ResourceRequest.pResource->Unlock();
				if( ResourceRequest.pHR )
					*ResourceRequest.pHR = hr;
			}

			// Decrement num oustanding resources
			m_NumOustandingResources --;
		}
	}
}


EFF3D_END