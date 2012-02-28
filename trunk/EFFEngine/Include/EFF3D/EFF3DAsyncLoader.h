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
	EFF3DResource * resource;
	effBOOL * result;
	effVOID ** deviceObject;
	effString filePath;
	effBOOL lock;
	effBOOL copy;
	effBOOL error;
};

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
class EFF3D_API EFF3DAsyncLoader
{
public:
	EFF3DAsyncLoader(EFF3DDevice * device, effUINT processingThreadsCount);
	~EFF3DAsyncLoader();

public:
	friend effUINT WINAPI _FileIOThreadProc(effVOID * parameter);
	friend effUINT WINAPI _ProcessingThreadProc(effVOID * parameter);



	effBOOL				AddWorkItem(EFF3DResource * resource, effBOOL * result);
	effVOID				WaitForAllItems();
	effVOID				ProcessDeviceWorkItems(effUINT currentToServiceResourcesCount, effBOOL retryLoads = effTRUE);

private:
	effUINT				FileIOThreadProc();
	effUINT				ProcessingThreadProc();
	effBOOL				InitAsyncLoadingThreadObjects(effUINT NumProcessingThreads);
	effVOID				DestroyAsyncLoadingThreadObjects();
private:
	effBOOL done;
	effBOOL processThreadDone;
	effBOOL IOThreadDone;
	effUINT toServiceResourcesCount;
	effUINT oustandingResourcesCount;
	std::vector<RESOURCE_REQUEST> IOQueue;
	std::vector<RESOURCE_REQUEST> processQueue;
	std::vector<RESOURCE_REQUEST> enderThreadQueue;

	CRITICAL_SECTION IOQueueCriticalSection;
	CRITICAL_SECTION processQueueCriticalSection;
	CRITICAL_SECTION renderThreadQueueCriticalSection;

	effHANDLE IOQueueSemaphore;
	effHANDLE processQueueSemaphore;
	effHANDLE copyQueueSemaphore;
	effHANDLE IOThread;
	
	effUINT processingThreadsCount;
	
	effHANDLE * processThreads;
	effUINT IORequestsCount;
	effUINT processRequestsCount;

	EFF3DDevice * device;
};

EFF3D_END

#endif