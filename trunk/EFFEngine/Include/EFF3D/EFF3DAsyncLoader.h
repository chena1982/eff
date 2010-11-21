/********************************************************************
	created:	9:12:2009   21:08
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DAsyncLoader.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3DAsyncLoader_2009_12_9__
#define __EFF3DAsyncLoader_2009_12_9__

EFF3D_BEGIN

class EFF3DIResource;

effVOID WarmIOCache(effBYTE* pData, effUINT size);

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct RESOURCE_REQUEST
{
	EFF3DIResource * pResource;
	effHRESULT* pHR;
	effVOID ** ppDeviceObject;
	effString strFilePath;
	bool bLock;
	bool bCopy;
	bool bError;
};

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
class EFF3D_API EFF3DAsyncLoader
{
private:
	effBOOL m_bDone;
	effBOOL m_bProcessThreadDone;
	effBOOL m_bIOThreadDone;
	effUINT m_NumResourcesToService;
	effUINT m_NumOustandingResources;
	std::vector<RESOURCE_REQUEST> m_IOQueue;
	std::vector<RESOURCE_REQUEST> m_ProcessQueue;
	std::vector<RESOURCE_REQUEST> m_RenderThreadQueue;

	CRITICAL_SECTION m_csIOQueue;
	CRITICAL_SECTION m_csProcessQueue;
	CRITICAL_SECTION m_csRenderThreadQueue;

	effHANDLE m_hIOQueueSemaphore;
	effHANDLE m_hProcessQueueSemaphore;
	effHANDLE m_hCopyQueueSemaphore;
	effHANDLE m_hIOThread;
	
	effUINT m_NumProcessingThreads;
	
	effHANDLE * m_phProcessThreads;
	effUINT m_NumIORequests;
	effUINT m_NumProcessRequests;

	EFF3DDevice *	m_pDevice;

private:
	effUINT									FileIOThreadProc();
	effUINT									ProcessingThreadProc();
	bool											InitAsyncLoadingThreadObjects(effUINT NumProcessingThreads );
	effVOID									DestroyAsyncLoadingThreadObjects();

public:
	friend effUINT WINAPI				_FileIOThreadProc(effVOID * lpParameter);
	friend effUINT WINAPI				_ProcessingThreadProc(effVOID * lpParameter);

	EFF3DAsyncLoader(EFF3DDevice * pDevice,effUINT NumProcessingThreads);
	~EFF3DAsyncLoader();

	effHRESULT								AddWorkItem(EFF3DIResource * pResource, effHRESULT* pHResult);
	effVOID									WaitForAllItems();
	effVOID									ProcessDeviceWorkItems(effUINT CurrentNumResourcesToService, effBOOL bRetryLoads = effTRUE);
};

EFF3D_END

#endif