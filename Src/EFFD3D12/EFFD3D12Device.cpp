/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\Src\EFFD3D12\EFFD3D12Device.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D12PCH.h"
#include "EFFD3D12Device.h"
#include "EFFD3D12Texture.h"
#include "EFFD3D12IndexBuffer.h"
#include "EFFD3D12VertexBuffer.h"
#include "EFFD3D12VertexDeclaration.h"
#include "EFFD3D12Shader.h"
#include "EFFD3D12Query.h"
#include "EFFD3D12ResourceStateManager.h"
#include "EFFD3D12DeviceCommandQueue.h"
#include "EFFD3D12DeviceCommandList.h"
#include "EFFD3D12Descriptor.h"

#include "murmur3/murmur3.h"

#include <chrono>
//#define new EFFNEW

struct EFF3DFormatInfo
{
	effUINT bpp;
	effUINT rmask;
	effUINT gmask;
	effUINT bmask;
	effUINT amask;
	EFF3DFormat format;
};

EFF3DFormatInfo formatInfo[] =
{
	{ 1, 0xFF, 0, 0, 0, EFF3D_FORMAT_A8 },
	{ 2, 0xf800, 0x07e0, 0x001f, 0x0000, EFF3D_FORMAT_R5G6B5 },
	{ 2, 0x7c00, 0x03e0, 0x001f, 0x8000, EFF3D_FORMAT_RGB5A1 },
	{ 2, 0x0f00, 0x00f0, 0x000f, 0xf000, EFF3D_FORMAT_RGBA4 },
	{ 3, 0xff0000, 0x00ff00, 0x0000ff, 0x000000, EFF3D_FORMAT_RGB8 },
	{ 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000, EFF3D_FORMAT_RGBA8 },
	{ 4, 0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000, EFF3D_FORMAT_RGB10A2 },
	{ 4, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000, EFF3D_FORMAT_RG16 },
};

D3D12_RESOURCE_FLAGS flagsInfo[] = {
	D3D12_RESOURCE_FLAG_NONE
};


effBOOL CheckTearingSupport()
{
	effBOOL allowTearing = effFALSE;

	// Rather than create the DXGI 1.5 factory interface directly, we create the
	// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
	// graphics debugging tools which will not support the 1.5 factory interface 
	// until a future update.
	ComPtr<IDXGIFactory4> factory4;
	if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
	{
		ComPtr<IDXGIFactory5> factory5;
		if (SUCCEEDED(factory4.As(&factory5)))
		{
			if (FAILED(factory5->CheckFeatureSupport(
				DXGI_FEATURE_PRESENT_ALLOW_TEARING,
				&allowTearing, sizeof(allowTearing))))
			{
				allowTearing = effFALSE;
			}
		}
	}

	return allowTearing;
}

effBOOL effCreate3DDevice(EFF3DDevice ** eff3DDevice, effBOOL window, HWND hWnd, effINT width, effINT height)
{

	EFFD3D12Device * D3DDevice = EFFNEW EFFD3D12Device;

	D3DDevice->Init(window, hWnd, width, height);

	*eff3DDevice =	D3DDevice;

	return effTRUE;
}

EFFD3D12Device::EFFD3D12Device()
{
}


EFFD3D12Device::~EFFD3D12Device()
{
	//cgD3D9SetDevice(NULL);
	//cgDestroyContext(cgContext);

	// Make sure the command queue has finished all commands before closing.

	::CloseHandle(fenceEvent);
}


effBOOL EFFD3D12Device::Init(effBOOL window, HWND hWnd, effINT width, effINT height)
{

#if defined(_DEBUG)
	// Always enable the debug layer before doing anything DX12 related
	// so all possible errors generated while creating DX12 objects
	// are caught by the debug layer.
	ComPtr<ID3D12Debug1> debugInterface;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
	debugInterface->EnableDebugLayer();
	// Enable these if you want full validation (will slow down rendering a lot).
	//debugInterface->SetEnableGPUBasedValidation(TRUE);
	//debugInterface->SetEnableSynchronizedCommandQueueValidation(TRUE);
#endif

	tearingSupported = CheckTearingSupport();

	auto dxgiAdapter = GetAdapter(effFALSE);
	if (!dxgiAdapter)
	{
		// If no supporting DX12 adapters exist, fall back to WARP
		dxgiAdapter = GetAdapter(effTRUE);
	}

	if (dxgiAdapter)
	{
		d3d12Device = CreateD3D12Device(dxgiAdapter);
	}
	else
	{
		return effFALSE;
	}


	directCommandQueue = EFFNEW EFFD3D12DeviceCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
	computeCommandQueue = EFFNEW EFFD3D12DeviceCommandQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE);
	copyCommandQueue = EFFNEW EFFD3D12DeviceCommandQueue(D3D12_COMMAND_LIST_TYPE_COPY);

	dxgiSwapChain = CreateSwapChain(hWnd, width, height);


	RTVDescriptorSize = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);



	// Create Command Allocator
	for (int i = 0; i < numFrames; ++i)
	{
		DX_CHECK(d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocators[i])));
	}



	// Create Fence
	DX_CHECK(d3d12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

	// Create Fence Event
	fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent && "Failed to create fence event.");


	resourceStateManager = EFFNEW EFFD3D12ResourceStateManager();

	// Create descriptor allocators
	for (effINT i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i)
	{
		descriptorAllocators[i] = EFFNEW EFFD3D12DescriptorAllocator((D3D12_DESCRIPTOR_HEAP_TYPE)i);
	}

	frame = 0;

	return effTRUE;
}


ComPtr<IDXGIAdapter4> EFFD3D12Device::GetAdapter(effBOOL useWarp)
{
	ComPtr<IDXGIFactory4> dxgiFactory;
	UINT createFactoryFlags = 0;

#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	DX_CHECK(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

	ComPtr<IDXGIAdapter1> dxgiAdapter1;
	ComPtr<IDXGIAdapter4> dxgiAdapter4;

	if (useWarp)
	{
		DX_CHECK(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
		DX_CHECK(dxgiAdapter1.As(&dxgiAdapter4));
	}
	else
	{
		effUINT64 maxDedicatedVideoMemory = 0;
		for (effUINT32 i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
			dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

			// Check to see if the adapter can create a D3D12 device without actually 
			// creating it. The adapter with the largest dedicated video memory
			// is favored.
			if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
				SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
				D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
				dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
			{
				maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
				DX_CHECK(dxgiAdapter1.As(&dxgiAdapter4));
			}
		}
	}

	return dxgiAdapter4;
}

ComPtr<ID3D12Device2> EFFD3D12Device::CreateD3D12Device(ComPtr<IDXGIAdapter4> adapter)
{
	ComPtr<ID3D12Device2> d3d12Device2;
	DX_CHECK(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device2)));
	//NAME_D3D12_OBJECT(d3d12Device2);

	// Enable debug messages in debug mode.
	#if defined(_DEBUG)
		ComPtr<ID3D12InfoQueue> pInfoQueue;
		if (SUCCEEDED(d3d12Device2.As(&pInfoQueue)))
		{
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

			// Suppress whole categories of messages
			//D3D12_MESSAGE_CATEGORY Categories[] = {};

			// Suppress messages based on their severity level
			D3D12_MESSAGE_SEVERITY Severities[] =
			{
				D3D12_MESSAGE_SEVERITY_INFO
			};

			// Suppress individual messages by their ID
			D3D12_MESSAGE_ID DenyIds[] = {
				D3D12_MESSAGE_ID_COPY_DESCRIPTORS_INVALID_RANGES,				// This started happening after updating to an RTX 2080 Ti. I believe this to be an error in the validation layer itself.
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
				D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
				D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
			};

			D3D12_INFO_QUEUE_FILTER NewFilter = {};
			//NewFilter.DenyList.NumCategories = _countof(Categories);
			//NewFilter.DenyList.pCategoryList = Categories;
			NewFilter.DenyList.NumSeverities = _countof(Severities);
			NewFilter.DenyList.pSeverityList = Severities;
			NewFilter.DenyList.NumIDs = _countof(DenyIds);
			NewFilter.DenyList.pIDList = DenyIds;

			DX_CHECK(pInfoQueue->PushStorageFilter(&NewFilter));
		}
	#endif

	return d3d12Device2;
}

ComPtr<IDXGISwapChain4> EFFD3D12Device::CreateSwapChain(HWND hWnd, effINT width, effINT height)
{
	ComPtr<IDXGISwapChain4> dxgiSwapChain4;
	ComPtr<IDXGIFactory4> dxgiFactory4;
	effUINT32 createFactoryFlags = 0;

	#if defined(_DEBUG)
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
	#endif

	DX_CHECK(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = numFrames;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	// It is recommended to always allow tearing if tearing support is available.
	swapChainDesc.Flags = tearingSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
	ID3D12CommandQueue* pCommandQueue = GetCommandQueue()->GetD3D12CommandQueue().Get();

	ComPtr<IDXGISwapChain1> swapChain1;
	DX_CHECK(dxgiFactory4->CreateSwapChainForHwnd(
		pCommandQueue,
		hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain1));

	// Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen
	// will be handled manually.
	DX_CHECK(dxgiFactory4->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

	DX_CHECK(swapChain1.As(&dxgiSwapChain4));

	currentBackBufferIndex = dxgiSwapChain4->GetCurrentBackBufferIndex();

	return dxgiSwapChain4;
}

effBOOL EFFD3D12Device::BeginScene()
{
	const effUINT64 primitiveType = renderDebugFlags & EFF3D_DEBUG_WIREFRAME ? EFF3D_STATE_PT_LINES : 0;
	effBYTE primitiveIndex = effBYTE(primitiveType >> EFF3D_STATE_PT_SHIFT);
	//primitiveInfo = s_primInfo[primitiveIndex];

	currentDrawCommand.Clear();
	currentDrawCommand.stateFlags = EFF3D_STATE_NONE;
	currentDrawCommand.stencil = PackStencil(EFF3D_STENCIL_NONE, EFF3D_STENCIL_NONE);

	// invalidate sampler state
	for (effUINT stage = 0; stage < EFF3D_CONFIG_MAX_TEXTURE_SAMPLERS; ++stage)
	{
		samplerFlags[stage] = UINT32_MAX;
	}

	auto commandAllocator = commandAllocators[currentBackBufferIndex];
	auto backBuffer = backBuffers[currentBackBufferIndex];

	commandAllocator->Reset();
	commandList->Reset(commandAllocator.Get(), nullptr);


	// Clear the render target.
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		commandList->ResourceBarrier(1, &barrier);

		FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			currentBackBufferIndex, RTVDescriptorSize);

		commandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
	}

	return effTRUE;
}

effBOOL EFFD3D12Device::EndScene()
{
	//return SUCCEEDED(D3D9Device->EndScene());
	return effTRUE;
}

effBOOL EFFD3D12Device::Clear(effUINT count, const EFFRect * rects, effUINT flags, EFF3DCOLOR color, effFLOAT z, effUINT stencil)
{
	//return SUCCEEDED(D3D9Device->Clear(count,(const D3DRECT *)rects, flags, color, z, stencil));
	return effTRUE;
}

effBOOL EFFD3D12Device::Present(const EFFRect * sourceRect, const EFFRect * destRect)
{
	auto commandQueue = GetCommandQueue();
	auto commandList = commandQueue->GetCommandList();

	auto backBuffer = backBuffers[currentBackBufferIndex];

	commandList->TransitionBarrier(backBuffer, D3D12_RESOURCE_STATE_PRESENT);
	commandQueue->ExecuteCommandList(commandList);

	effUINT32 syncInterval = VSync ? 1 : 0;
	effUINT32 presentFlags = tearingSupported && !VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
	DX_CHECK(dxgiSwapChain->Present(syncInterval, presentFlags));

	fenceValues[currentBackBufferIndex] = commandQueue->Signal();
	frameFenceValues[currentBackBufferIndex] = frame;

	currentBackBufferIndex = dxgiSwapChain->GetCurrentBackBufferIndex();

	commandQueue->WaitForFenceValue(frameFenceValues[currentBackBufferIndex]);
	ReleaseStaleDescriptors(frameFenceValues[currentBackBufferIndex]);

	return effTRUE;
}

effVOID UnloadTexture(EFF3DResource * resource, effVOID * userData)
{

    EFF3DImage * image = (EFF3DImage *)resource;
    const EFF3DImageInfo & imageInfo = image->GetImageInfo();

    /*if (imageInfo.pool == EFF3DPOOL_DEFAULT)
    {
        image->Unload();
        return;
    }*/

    if (imageInfo.type == EFF3DResourceType_Texture2D)
    {
        /*if (imageInfo.surfaceType == DepthStencil_Surface || imageInfo.surfaceType == GetFromTexture_Surface)
        {
            EFF3DSurface * surface = (EFF3DSurface *)resource;
            surface->Unload();
            return;
        }*/

        /*if ( imageInfo.surfaceType == RenderTo_Surface )
        {
        EFF3DRenderToSurface * renderToSurface = reinterpret_cast<EFF3DRenderToSurface *>(it->second);
        renderToSurface->UnloadResource();
        continue;
        }*/

    }
    //it->second->UnloadResource();
   
}

effVOID ReloadTexture(EFF3DResource * resource, effVOID * userData)
{
    EFF3DImage * image = (EFF3DImage *)resource;
    if (image->isUnloaded)
    {
        if (!image->Reload())
        {
            return;
        }
    }
}

effBOOL EFFD3D12Device::Reset(effBOOL window, effINT width, effINT height)
{


	return effTRUE;
}

EFF3DResource * EFFD3D12Device::CreateEmptyResourceImpl(EFF3DResourceType resourceType)
{
	EFF3DResource * resource = NULL;
	
	switch(resourceType)
	{
        case EFF3DResourceType_Texture2D:
        case EFF3DResourceType_Texture3D:
        case EFF3DResourceType_TextureCube:
        case EFF3DResourceType_RenderTarget:
	        {
		        resource = EFFNEW EFFD3D12Texture();
	        }
	        break;
        case EFF3DResourceType_IndexBuffer:
            {
                resource = EFFNEW EFFD3D12IndexBuffer();
            }
            break;
        case EFF3DResourceType_VertexBuffer:
            {
                resource = EFFNEW EFFD3D12VertexBuffer();
            }
            break;
		case EFF3DResourceType_TimeQuery:
			{
				resource = EFFNEW EFFD3D12TimeQuery();
			}
			break;
	    default:
		    break;
	}

	resource->type = resourceType;
	return resource;
}

effBOOL EFFD3D12Device::CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT flag, EFF3DFormat format, 
	EFF3DResourceType resourceType, EFF3DTextureHandle * textureHandle, effSIZE sharedHandle)
{
	//assert(texture != NULL);

    EFFD3D12Texture * effD3D12Texture = (EFFD3D12Texture *)CreateEmptyResource(resourceType, *textureHandle);

    /*effHRESULT hr;
    effUINT usage = 0;

	if (resourceType == EFF3DResourceType_RenderTarget)
	{
		usage |= D3DUSAGE_RENDERTARGET;
	}

	if( FAILED(hr = D3D9Device->CreateTexture(width, height, levels, usage, s_textureFormat[format].d3d9Format, 
		D3DPOOL_DEFAULT, &effD3D12Texture->texture, (HANDLE *)&sharedHandle)) )
	{
		SF_DELETE(effD3D12Texture);
		return effFALSE;
	}*/

	effD3D12Texture->device = this;

	effD3D12Texture->imageInfo.width = width;
	effD3D12Texture->imageInfo.height = height;
	effD3D12Texture->imageInfo.mipLevels = levels;
	effD3D12Texture->imageInfo.format = format;
	effD3D12Texture->imageInfo.type = resourceType;
	effD3D12Texture->imageInfo.sharedHandle = (effHANDLE)sharedHandle;

    *textureHandle = effD3D12Texture->id;

	return effTRUE;	
}



/*effBOOL EFFD3D12Device::_CreateSharedTexture(effUINT width, effUINT height, effUINT levels, effUINT usage, EFF3DFORMAT format, EFF3DSharedTexture** texture)
{
    assert(texture != NULL);

    EFFD3D9SharedTexture * effD3D9Texture = EFFNEW EFFD3D9SharedTexture();

    effHRESULT hr;

    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        if (FAILED(hr = D3D9Device->CreateTexture(width, height, levels, usage, (D3DFORMAT)format, D3DPOOL_DEFAULT, &effD3D9Texture->texture[i], &effD3D9Texture->sharedHandle[i])))
        {
            SF_DELETE(effD3D9Texture);
            return effFALSE;
        }
    }

    effD3D9Texture->m_ImageInfo.width = width;
    effD3D9Texture->m_ImageInfo.height = height;
    effD3D9Texture->m_ImageInfo.mipLevels = levels;
    effD3D9Texture->m_ImageInfo.usage = usage;
    effD3D9Texture->m_ImageInfo.format = (EFF3DFORMAT)format;
    effD3D9Texture->m_ImageInfo.pool = EFF3DPOOL_DEFAULT;
    effD3D9Texture->m_ImageInfo.resourceType = EFF3DRTYPE_TEXTURE;

    effD3D9Texture->AddRef();


    *texture = effD3D9Texture;

    return effTRUE;
}

effBOOL EFFD3D12Device::_CreateSharedTexture(SharedTextureInfo * sharedTextureInfo, EFF3DSharedTexture ** texture)
{

    EFFD3D9SharedTexture * effD3D9Texture = EFFNEW EFFD3D9SharedTexture();

    effD3D9Texture->sharedHandle[0] = (HANDLE)sharedTextureInfo->sharedTextureHandle[0];
    effD3D9Texture->sharedHandle[1] = (HANDLE)sharedTextureInfo->sharedTextureHandle[1];
	effD3D9Texture->sharedHandle[2] = (HANDLE)sharedTextureInfo->sharedTextureHandle[2];

    effHRESULT hr;

    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        if (FAILED(hr = D3D9Device->CreateTexture(sharedTextureInfo->width, sharedTextureInfo->height, 1, 0, (D3DFORMAT)sharedTextureInfo->format, D3DPOOL_DEFAULT, &effD3D9Texture->texture[i], &effD3D9Texture->sharedHandle[i])))
        {
            SF_DELETE(effD3D9Texture);
            return effFALSE;
        }
    }


    effD3D9Texture->sharedHandle[0] = NULL;
    effD3D9Texture->sharedHandle[1] = NULL;
	effD3D9Texture->sharedHandle[2] = NULL;

    effD3D9Texture->AddRef();


    *texture = effD3D9Texture;

    return effTRUE;
}*/



effBOOL EFFD3D12Device::CreateTextureFromMemory(effVOID * srcData, effUINT srcDataSize, effINT width, effINT height, effINT level, effUINT flag,
                                            EFF3DFormat format, EFF3DResourceType resourceType, EFF3DTextureHandle * textureHandle)
{
	//assert(texture != NULL);

	effINT pixelSize = EFF3DGetPixelSizeFromFormat(format);
	if (width * height * pixelSize > (effINT)srcDataSize)
	{
		return effFALSE;
	}

    EFFD3D12Texture * effD3D12Texture = (EFFD3D12Texture *)CreateEmptyResource(resourceType, *textureHandle);
    //effHRESULT hr;


    effHANDLE shareHandle = NULL;
	/*if (FAILED(hr = D3D9Device->CreateTexture(width, height, level, 0, s_textureFormat[format].d3d9Format, D3DPOOL_DEFAULT, &effD3D9Texture->texture, &shareHandle)))
	{
		SF_DELETE(effD3D9Texture);
		return effFALSE;
	}*/

    effD3D12Texture->userData = shareHandle;

    EFF3DLockedRect rc;
	if (FAILED(effD3D12Texture->LockRect(0, &rc, NULL, 0)))
	{
		SF_DELETE(effD3D12Texture);
		return effFALSE;
	}

	if (rc.pitch < pixelSize * width)
	{
		SF_DELETE(effD3D12Texture);
		effD3D12Texture->UnlockRect(0);
		return effFALSE;
	}

	for (effINT i = 0; i < height; i++)
	{
		memcpy(((effBYTE *)rc.data) + rc.pitch * i, ((effBYTE *)srcData) + i * width * pixelSize, width * pixelSize);
	}

	effD3D12Texture->UnlockRect(0);

	effD3D12Texture->imageInfo.width = width;
	effD3D12Texture->imageInfo.height = height;
	//effD3D9Texture->imageInfo.usage = 0;
	effD3D12Texture->imageInfo.format = format;
	//effD3D9Texture->imageInfo.pool = EFF3DPOOL_DEFAULT;
	effD3D12Texture->imageInfo.type = resourceType;


    *textureHandle = effD3D12Texture->id;

	return effTRUE;	
}

/*effBOOL EFFD3D12Device::CreateRenderTarget(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample, effUINT multisampleQuality, effBOOL lockable, EFF3DSurface ** surface)
{
	assert(surface != NULL);

	EFFD3D9Surface * effD3D9Surface = EFFNEW EFFD3D9Surface();

	effHRESULT hr;
	if ( FAILED(hr = D3D9Device->CreateRenderTarget(width, height, (D3DFORMAT)format, (D3DMULTISAMPLE_TYPE)multiSample, multisampleQuality, lockable, &effD3D9Surface->m_pSurface, NULL)) )
	{
		SF_DELETE(effD3D9Surface);
		return effFALSE;
	}

	effD3D9Surface->m_ImageInfo.width = width;
	effD3D9Surface->m_ImageInfo.height = height;
	effD3D9Surface->m_ImageInfo.format = (EFF3DFORMAT)format;
	effD3D9Surface->m_ImageInfo.surfaceType = RenderTo_Surface;

	effD3D9Surface->AddRef();
	*surface = effD3D9Surface;

	return effTRUE;
}

effBOOL EFFD3D12Device::CreateDepthStencilSurface(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample, effUINT multisampleQuality, effBOOL discard, EFF3DSurface ** surface)
{
	assert(surface != NULL);

	EFFD3D9Surface * effD3D9Surface = EFFNEW EFFD3D9Surface();

	effHRESULT hr;
	if ( FAILED(hr = D3D9Device->CreateDepthStencilSurface(width, height, (D3DFORMAT)format, (D3DMULTISAMPLE_TYPE)multiSample, multisampleQuality, discard, &effD3D9Surface->m_pSurface, NULL)) )
	{
		SF_DELETE(effD3D9Surface);
		return effFALSE;
	}

	effD3D9Surface->m_ImageInfo.width = width;
	effD3D9Surface->m_ImageInfo.height = height;
	effD3D9Surface->m_ImageInfo.format = (EFF3DFORMAT)format;
	effD3D9Surface->m_ImageInfo.surfaceType = DepthStencil_Surface;

	effD3D9Surface->AddRef();
	*surface = effD3D9Surface;

	return effTRUE;
}*/

effBOOL EFFD3D12Device::CreateIndexBuffer(effVOID * data, effUINT size, effUINT flags, EFF3DIndexBufferHandle * indexBufferHandle)
{
	assert(indexBufferHandle != NULL && size > 0);
	
	EFFD3D12IndexBuffer * effD3D12IndexBuffer = (EFFD3D12IndexBuffer *)CreateEmptyResource(EFF3DResourceType_IndexBuffer, *indexBufferHandle);

	ComPtr<ID3D12Resource> d3d12Resource;

	DX_CHECK(d3d12Device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((effUINT64)size, flagsInfo[flags]),
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&d3d12Resource)));

	effD3D12IndexBuffer->d3d12Resource = d3d12Resource;

	// Add the resource to the global resource state tracker.
	resourceStateManager->AddGlobalResourceState(d3d12Resource.Get(), D3D12_RESOURCE_STATE_COMMON);

    if (data != NULL)
    {
		UpdateBuffer(effD3D12IndexBuffer->d3d12Resource, 0, size, data, D3D12_RESOURCE_FLAG_NONE);
    }

	effUINT32 elementSize = 2;

	effD3D12IndexBuffer->CreateViews(size / elementSize, elementSize);


	*indexBufferHandle = effD3D12IndexBuffer->id;
	return effTRUE;
}

effBOOL EFFD3D12Device::UpdateIndexBuffer(effUINT offset, effVOID * data, effUINT size)
{

    return effTRUE;
}

effBOOL EFFD3D12Device::CreateVertexBuffer(effVOID * data, effUINT size, effUINT flags, EFF3DVertexDeclarationHandle vertexDeclHandle,
	EFF3DVertexBufferHandle * vertexBufferHandle)
{
	assert(vertexBufferHandle != NULL && size > 0);

	//https://docs.microsoft.com/en-us/windows/desktop/api/d3d9/nf-d3d9-idirect3ddevice9-createvertexbuffer

	EFFD3D12VertexBuffer * effD3D12VertexBuffer = (EFFD3D12VertexBuffer *)CreateEmptyResource(EFF3DResourceType_VertexBuffer, *vertexBufferHandle);
	effD3D12VertexBuffer->vertexDeclHandle = vertexDeclHandle;


	effUINT32 elementSize = (effUINT32)vertexDeclManager->GetVertexDeclaration(vertexDeclHandle)->GetStride();

	ComPtr<ID3D12Resource> d3d12Resource;

	DX_CHECK(d3d12Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((effUINT64)size, flagsInfo[flags]),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&d3d12Resource)));

	effD3D12VertexBuffer->d3d12Resource = d3d12Resource;

	// Add the resource to the global resource state tracker.
	resourceStateManager->AddGlobalResourceState(d3d12Resource.Get(), D3D12_RESOURCE_STATE_COMMON);

	if (data != NULL)
	{
		UpdateBuffer(effD3D12VertexBuffer->d3d12Resource, 0, size, data, D3D12_RESOURCE_FLAG_NONE);
	}

	effD3D12VertexBuffer->CreateViews(size / elementSize, elementSize);


	*vertexBufferHandle = effD3D12VertexBuffer->id;
	return effTRUE;
}

effBOOL EFFD3D12Device::UpdateVertexBuffer(effUINT offset, effVOID * data, effUINT size)
{
    return effTRUE;
}

/*static D3DVERTEXELEMENT9 * fillVertexDecl(effBYTE stream, D3DVERTEXELEMENT9 * out, const EFF3DVertexDeclaration & vertexDecl)
{
    D3DVERTEXELEMENT9 * elem = out;

    for (effUINT attr = 0; attr < EFF3DVertexAttrib::Count; ++attr)
    {
        if (UINT16_MAX != vertexDecl.attributes[attr].valueUINT16)
        {
            effBYTE num;
            EFF3DVertexAttribType::Enum type;
            effBOOL normalized;
            effBOOL asInt;
            vertexDecl.Decode(EFF3DVertexAttrib::Enum(attr), num, type, normalized, asInt);

            memcpy(elem, &s_attrib[attr], sizeof(D3DVERTEXELEMENT9));

            elem->Stream = stream;
            elem->Type = s_attribType[type][num - 1][normalized];
            elem->Offset = vertexDecl.offset[attr];
            ++elem;
        }
    }

    return elem;
}*/


effVOID EFFD3D12Device::SetVertices()
{
    effBOOL vertexStreamChanged = HasVertexStreamChanged();

    if (programChanged || vertexStreamChanged)
    {
        currentDrawCommand.streamMask = newDrawCommand->streamMask;
        currentDrawCommand.instanceDataBufferHandle = newDrawCommand->instanceDataBufferHandle;
        currentDrawCommand.instanceDataOffset = newDrawCommand->instanceDataOffset;
        currentDrawCommand.instanceDataStride = newDrawCommand->instanceDataStride;

        const EFF3DVertexDeclaration * decls[EFF3D_CONFIG_MAX_VERTEX_STREAMS];

        const effBOOL instanced = effTRUE && newDrawCommand->instanceDataBufferHandle.IsValid();

        //const effUINT freq = instanced ? D3DSTREAMSOURCE_INDEXEDDATA | newDrawCommand->numInstances : 1;

        effUINT numVertices = newDrawCommand->numVertices;
        effBYTE numStreams = 0;
        for (effUINT idx = 0, streamMask = newDrawCommand->streamMask, ntz = effUINT_cnttz(streamMask)
            ; 0 != streamMask
            ; streamMask >>= 1, idx += 1, ntz = effUINT_cnttz(streamMask), ++numStreams
            )
        {
            streamMask >>= ntz;
            idx += ntz;

            currentDrawCommand.stream[idx].vertexDeclHandle = newDrawCommand->stream[idx].vertexDeclHandle;
            currentDrawCommand.stream[idx].vertexBufferHandle = newDrawCommand->stream[idx].vertexBufferHandle;
            currentDrawCommand.stream[idx].startVertex = newDrawCommand->stream[idx].startVertex;

            const EFF3DVertexBufferHandle vertexBufferHandle = newDrawCommand->stream[idx].vertexBufferHandle;
            const EFFD3D12VertexBuffer * vb = (EFFD3D12VertexBuffer *)GetVertexBufferManager()->GetResource(vertexBufferHandle);
            const EFF3DVertexDeclarationHandle vertexDeclHandle = !vb->vertexDeclHandle.IsValid() ? newDrawCommand->stream[idx].vertexDeclHandle : vb->vertexDeclHandle;
            const EFF3DVertexDeclaration * vertexDecl = vertexDeclManager->GetVertexDeclaration(vertexDeclHandle);
            const effUINT stride = vertexDecl->stride;

            decls[numStreams] = vertexDecl;

            numVertices = std::min(UINT32_MAX == newDrawCommand->numVertices ? vb->size / stride : newDrawCommand->numVertices, numVertices);

            //DX_CHECK(D3D9Device->SetStreamSourceFreq(0, freq));
            //DX_CHECK(D3D9Device->SetStreamSource(numStreams, vb->d3d9VertexBuffer, 0, stride));
        }

        currentDrawCommand.numVertices = numVertices;

        if (0 < numStreams)
        {
            if (instanced)
            {
                const EFFD3D12VertexBuffer & inst = *(EFFD3D12VertexBuffer *)GetVertexBufferManager()->GetResource(newDrawCommand->instanceDataBufferHandle);
                //DX_CHECK(D3D9Device->SetStreamSourceFreq(numStreams, UINT(D3DSTREAMSOURCE_INSTANCEDATA | 1)));
                //DX_CHECK(D3D9Device->SetStreamSource(numStreams, inst.d3d9VertexBuffer, newDrawCommand->instanceDataOffset, newDrawCommand->instanceDataStride));
                SetInputLayout(numStreams, decls, newDrawCommand->instanceDataStride / 16);
            }
            else
            {
                //DX_CHECK(D3D9Device->SetStreamSource(numStreams, NULL, 0, 0));
                SetInputLayout(numStreams, decls, 0);
            }
        }
        else
        {
            //DX_CHECK(D3D9Device->SetStreamSource(0, NULL, 0, 0));
            //DX_CHECK(D3D9Device->SetStreamSource(1, NULL, 0, 0));
        }
    }

}

effVOID EFFD3D12Device::SetInputLayout(effBYTE numStreams, const EFF3DVertexDeclaration ** vertexDecls, effUINT16 numInstanceData)
{
    effBYTE key[4 * EFF3D_CONFIG_MAX_VERTEX_STREAMS];
    effUINT size = 0;
    memset(key, 0, sizeof(key));

    //memcpy(key, &numInstanceData, sizeof(effUINT16));
    //size += 2;

    for (effBYTE stream = 0; stream < numStreams; ++stream)
    {
        memcpy(key + stream * 4, &vertexDecls[stream]->hash, sizeof(effUINT));
        size += 4;
    }

    effUINT layoutHash;
    MurmurHash3_x86_32(key, size, 9582, &layoutHash);

    /*LPDIRECT3DVERTEXDECLARATION9 layout = NULL;
    auto it = inputLayoutCache.find(layoutHash);
    if (it == inputLayoutCache.end())
    {
        D3DVERTEXELEMENT9 vertexElements[EFF3DVertexAttrib::Count + 1 + EFF3D_CONFIG_MAX_INSTANCE_DATA_COUNT];
        D3DVERTEXELEMENT9* elem = vertexElements;

        for (effBYTE stream = 0; stream < numStreams; ++stream)
        {
            elem = fillVertexDecl(stream, elem, *vertexDecls[stream]);
        }

        const D3DVERTEXELEMENT9 inst = { numStreams, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 };

        for (effBYTE ii = 0; ii < numInstanceData; ++ii)
        {
            memcpy(elem, &inst, sizeof(D3DVERTEXELEMENT9));
            elem->UsageIndex = effBYTE(7 - ii); // TEXCOORD7 = i_data0, TEXCOORD6 = i_data1, etc.
            elem->Offset = ii * 16;
            ++elem;
        }

        memcpy(elem, &s_attrib[EFF3DVertexAttrib::Count], sizeof(D3DVERTEXELEMENT9));

        DX_CHECK(D3D9Device->CreateVertexDeclaration(vertexElements, &layout));

        inputLayoutCache[layoutHash] = layout;
    }
	else
	{
		layout = it->second;
	}

    DX_CHECK(D3D9Device->SetVertexDeclaration(layout));*/
}

effVOID EFFD3D12Device::Draw(EFF3DDrawCommand & drawCommand)
{

	newDrawCommand = &drawCommand;




	SetRenderState(drawCommand);
	SetDepthStencilState(drawCommand);

	SetVertices();
	SetIndexBuffer();
	SetTextures(drawCommand);



    if (0 != currentDrawCommand.streamMask)
    {
        effUINT numVertices = drawCommand.numVertices;
        effUINT numIndices = 0;
        effUINT numPrimsSubmitted = 0;
        effUINT numInstances = 0;
        effUINT numPrimsRendered = 0;

        if (drawCommand.indexBufferHandle.IsValid())
        {
            if (UINT32_MAX == drawCommand.numIndices)
            {
				EFFD3D12IndexBuffer * effD3D12IB = (EFFD3D12IndexBuffer *)indexBufferManager->GetResource(drawCommand.indexBufferHandle);
                const uint32_t indexSize = 0 == (effD3D12IB->flags & EFF3D_BUFFER_INDEX32) ? 2 : 4;
                numIndices = effD3D12IB->count;
                numPrimsSubmitted = numIndices / primitiveInfo.div - primitiveInfo.sub;
                numInstances = drawCommand.numInstances;
                numPrimsRendered = numPrimsSubmitted * drawCommand.numInstances;

                /*DX_CHECK(D3D9Device->DrawIndexedPrimitive(primitiveInfo.type
                    , drawCommand.stream[0].startVertex
                    , 0
                    , numVertices
                    , 0
                    , numPrimsSubmitted
                ));*/
            }
            else if (primitiveInfo.min <= drawCommand.numIndices)
            {
                numIndices = drawCommand.numIndices;
                numPrimsSubmitted = numIndices / primitiveInfo.div - primitiveInfo.sub;
                numInstances = drawCommand.numInstances;
                numPrimsRendered = numPrimsSubmitted * drawCommand.numInstances;

                /*DX_CHECK(D3D9Device->DrawIndexedPrimitive(primitiveInfo.type
                    , drawCommand.stream[0].startVertex
                    , 0
                    , numVertices
                    , drawCommand.startIndex
                    , numPrimsSubmitted
                ));*/
            }
        }
        else
        {
            numPrimsSubmitted = numVertices / primitiveInfo.div - primitiveInfo.sub;
            numInstances = drawCommand.numInstances;
            numPrimsRendered = numPrimsSubmitted * drawCommand.numInstances;

            /*DX_CHECK(D3D9Device->DrawPrimitive(primitiveInfo.type
                , drawCommand.stream[0].startVertex
                , numPrimsSubmitted
            ));*/
        }


        //statsNumPrimsSubmitted[primIndex] += numPrimsSubmitted;
        //statsNumPrimsRendered[primIndex] += numPrimsRendered;
        //statsNumInstances[primIndex] += numInstances;
        //statsNumIndices += numIndices;
    }
}

effBOOL EFFD3D12Device::CreateTimeQuery(effUINT flag, EFF3DTimeQueryHandle * queryHandle)
{
	EFFD3D12TimeQuery * effD3D12TimeQuery = (EFFD3D12TimeQuery *)CreateEmptyResource(EFF3DResourceType_TimeQuery, *queryHandle);
	effD3D12TimeQuery->Init();

	*queryHandle = effD3D12TimeQuery->id;
	return effTRUE;
}

effBOOL EFFD3D12Device::DrawIndexedPrimitive(EFF3DPrimitiveType type, effINT baseVertexIndex, effUINT minIndex, effUINT numVertices,
    effUINT startIndex, effUINT primitiveCount)
{
    return effTRUE;
	//return SUCCEEDED(D3D9Device->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount));
}

effBOOL EFFD3D12Device::DrawIndexedPrimitiveUP(EFF3DPrimitiveType primitiveType, effUINT minVertexIndex, effUINT numVertices, effUINT primitiveCount,
    const effVOID * indexData, effBOOL index16,
    const effVOID * vertexStreamZeroData, effUINT vertexStreamZeroStride)
{
    return effTRUE;
	//return SUCCEEDED(D3D9Device->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)primitiveType, minVertexIndex, numVertices, primitiveCount, indexData, (D3DFORMAT)indexDataFormat, vertexStreamZeroData, vertexStreamZeroStride));
}

effBOOL EFFD3D12Device::DrawPrimitive(EFF3DPrimitiveType primitiveType, effUINT startVertex, effUINT primitiveCount)
{
	//HRESULT hr = D3D9Device->DrawPrimitive((D3DPRIMITIVETYPE)primitiveType, startVertex, primitiveCount);
	//return SUCCEEDED(hr);
    return effTRUE;
}

effBOOL EFFD3D12Device::DrawPrimitiveUP(EFF3DPrimitiveType primitiveType, effUINT primitiveCount, const effVOID * vertexStreamZeroData,
    effUINT vertexStreamZeroStride)
{
    return effTRUE;
	//return SUCCEEDED(D3D9Device->DrawPrimitiveUP((D3DPRIMITIVETYPE)primitiveType, primitiveCount, vertexStreamZeroData, vertexStreamZeroStride));
}

/*effBOOL EFFD3D12Device::SetTransform(EFF3DPrimitiveType state, const EFFMatrix4 * matrix)
{
	return SUCCEEDED(D3D9Device->SetTransform((D3DTRANSFORMSTATETYPE)state, (const D3DMATRIX *)matrix));
}*/

effVOID	EFFD3D12Device::SetIndexBuffer()
{
	if (currentDrawCommand.indexBufferHandle != newDrawCommand->indexBufferHandle)
	{
		currentDrawCommand.indexBufferHandle = newDrawCommand->indexBufferHandle;
		SetIndexBuffer(newDrawCommand->indexBufferHandle);
	}
}


effBOOL EFFD3D12Device::SetIndexBuffer(EFF3DIndexBufferHandle ibHandle)
{
	if (ibHandle.IsValid())
	{
		EFFD3D12IndexBuffer * effD3D12IB = (EFFD3D12IndexBuffer *)indexBufferManager->GetResource(ibHandle);
		//return SUCCEEDED(D3D9Device->SetIndices(effD3D12IB->d3d9IndexBuffer));
		return effTRUE;
	}
	else
	{
		//DX_CHECK(device->SetIndices(NULL));
		return effFALSE;
	}
}

//effBOOL EFFD3D12Device::SetRenderState(EFF3DRENDERSTATETYPE state, effUINT value)
//{
//	return SUCCEEDED(D3D9Device->SetRenderState((D3DRENDERSTATETYPE)state, value));
//}

effVOID EFFD3D12Device::SetRenderState(EFF3DDrawCommand & drawCommand)
{
    const effUINT64 newFlags = drawCommand.stateFlags;
    effUINT64 changedFlags = currentDrawCommand.stateFlags ^ drawCommand.stateFlags;
    currentDrawCommand.stateFlags = newFlags;

    if ((EFF3D_STATE_CULL_MASK
        | EFF3D_STATE_WRITE_Z
        | EFF3D_STATE_DEPTH_TEST_MASK
        | EFF3D_STATE_WRITE_RGB
        | EFF3D_STATE_WRITE_A
        | EFF3D_STATE_BLEND_MASK
        | EFF3D_STATE_BLEND_EQUATION_MASK
        | EFF3D_STATE_ALPHA_REF_MASK
        | EFF3D_STATE_PT_MASK
        | EFF3D_STATE_POINT_SIZE_MASK
        | EFF3D_STATE_MSAA) & changedFlags)
    {
        if (EFF3D_STATE_CULL_MASK & changedFlags)
        {
            effUINT32 cull = (newFlags & EFF3D_STATE_CULL_MASK) >> EFF3D_STATE_CULL_SHIFT;
            //DX_CHECK(D3D9Device->SetRenderState(D3DRS_CULLMODE, s_cullMode[cull]));
        }

        if (EFF3D_STATE_WRITE_Z & changedFlags)
        {
            //DX_CHECK(D3D9Device->SetRenderState(D3DRS_ZWRITEENABLE, !!(EFF3D_STATE_WRITE_Z & newFlags)));
        }

        if (EFF3D_STATE_DEPTH_TEST_MASK & changedFlags)
        {
            effUINT32 func = (newFlags & EFF3D_STATE_DEPTH_TEST_MASK) >> EFF3D_STATE_DEPTH_TEST_SHIFT;
            //DX_CHECK(D3D9Device->SetRenderState(D3DRS_ZENABLE, 0 != func));

            if (0 != func)
            {
                //DX_CHECK(D3D9Device->SetRenderState(D3DRS_ZFUNC, s_cmpFunc[func]));
            }
        }

        if (EFF3D_STATE_ALPHA_REF_MASK & changedFlags)
        {
            effUINT32 ref = (newFlags & EFF3D_STATE_ALPHA_REF_MASK) >> EFF3D_STATE_ALPHA_REF_SHIFT;
            //viewState.m_alphaRef = ref / 255.0f;
        }

        if ((EFF3D_STATE_PT_POINTS | EFF3D_STATE_POINT_SIZE_MASK) & changedFlags)
        {
            //DX_CHECK(D3D9Device->SetRenderState(D3DRS_POINTSIZE, (effUINT32)((newFlags & EFF3D_STATE_POINT_SIZE_MASK) >> EFF3D_STATE_POINT_SIZE_SHIFT)));
        }

        if (EFF3D_STATE_MSAA & changedFlags)
        {
            //DX_CHECK(D3D9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, (newFlags & EFF3D_STATE_MSAA) == EFF3D_STATE_MSAA));
        }

        if (EFF3D_STATE_LINEAA & changedFlags)
        {
            //DX_CHECK(D3D9Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, !!(newFlags & EFF3D_STATE_LINEAA)));
        }

        if ((EFF3D_STATE_WRITE_A | EFF3D_STATE_WRITE_RGB) & changedFlags)
        {
            effUINT32 writeEnable = 0;
            //writeEnable |= (newFlags & EFF3D_STATE_WRITE_R) ? D3DCOLORWRITEENABLE_RED : 0;
            //writeEnable |= (newFlags & EFF3D_STATE_WRITE_G) ? D3DCOLORWRITEENABLE_GREEN : 0;
            //writeEnable |= (newFlags & EFF3D_STATE_WRITE_B) ? D3DCOLORWRITEENABLE_BLUE : 0;
            //writeEnable |= (newFlags & EFF3D_STATE_WRITE_A) ? D3DCOLORWRITEENABLE_ALPHA : 0;
            //DX_CHECK(D3D9Device->SetRenderState(D3DRS_COLORWRITEENABLE, writeEnable));
        }

        if (((EFF3D_STATE_BLEND_MASK
            | EFF3D_STATE_BLEND_EQUATION_MASK
            | EFF3D_STATE_BLEND_ALPHA_TO_COVERAGE
            ) & changedFlags)
            || BlendFactor != drawCommand.rgba)
        {
            effBOOL enabled = !!(EFF3D_STATE_BLEND_MASK & newFlags);
            //DX_CHECK(D3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, enabled));

            if (AtocSupport && EFF3D_STATE_BLEND_ALPHA_TO_COVERAGE & changedFlags)
            {
                //DX_CHECK(D3D9Device->SetRenderState(D3DRS_ADAPTIVETESS_Y, !!(newFlags & EFF3D_STATE_BLEND_ALPHA_TO_COVERAGE) ? D3DFMT_ATOC : 0));
            }

            if (enabled)
            {
                const effUINT32 blend = uint32_t((newFlags & EFF3D_STATE_BLEND_MASK) >> EFF3D_STATE_BLEND_SHIFT);
                const effUINT32 equation = uint32_t((newFlags & EFF3D_STATE_BLEND_EQUATION_MASK) >> EFF3D_STATE_BLEND_EQUATION_SHIFT);

                const effUINT32 srcRGB = (blend) & 0xf;
                const effUINT32 dstRGB = (blend >> 4) & 0xf;
                const effUINT32 srcA = (blend >> 8) & 0xf;
                const effUINT32 dstA = (blend >> 12) & 0xf;

                const effUINT32 equRGB = (equation) & 0x7;
                const effUINT32 equA = (equation >> 3) & 0x7;

                //DX_CHECK(D3D9Device->SetRenderState(D3DRS_SRCBLEND, s_blendFactor[srcRGB].m_src));
                //DX_CHECK(D3D9Device->SetRenderState(D3DRS_DESTBLEND, s_blendFactor[dstRGB].m_dst));
                //DX_CHECK(D3D9Device->SetRenderState(D3DRS_BLENDOP, s_blendEquation[equRGB]));

                const bool separate = srcRGB != srcA || dstRGB != dstA || equRGB != equA;

                //DX_CHECK(D3D9Device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, separate));
                if (separate)
                {
                    //DX_CHECK(D3D9Device->SetRenderState(D3DRS_SRCBLENDALPHA, s_blendFactor[srcA].m_src));
                    //DX_CHECK(D3D9Device->SetRenderState(D3DRS_DESTBLENDALPHA, s_blendFactor[dstA].m_dst));
                    //DX_CHECK(D3D9Device->SetRenderState(D3DRS_BLENDOPALPHA, s_blendEquation[equA]));
                }

                /*if ((s_blendFactor[srcRGB].m_factor || s_blendFactor[dstRGB].m_factor)
                    && BlendFactor != drawCommand.rgba)
                {
                    const effUINT rgba = drawCommand.rgba;
                    D3DCOLOR color = D3DCOLOR_RGBA(rgba >> 24
                        , (rgba >> 16) & 0xff
                        , (rgba >> 8) & 0xff
                        , (rgba) & 0xff
                    );
                    //DX_CHECK(D3D9Device->SetRenderState(D3DRS_BLENDFACTOR, color));
                }*/
            }

            BlendFactor = drawCommand.rgba;
        }


		const effUINT64 primitiveType = renderDebugFlags & EFF3D_DEBUG_WIREFRAME ? EFF3D_STATE_PT_LINES : newFlags & EFF3D_STATE_PT_MASK;
		effBYTE primitiveIndex = effBYTE(primitiveType >> EFF3D_STATE_PT_SHIFT);

		primitiveInfo = s_primInfo[primitiveIndex];
    }
}

effVOID EFFD3D12Device::SetDepthStencilState(EFF3DDrawCommand & drawCommand)
{
    const effUINT64 newStencil = drawCommand.stencil;
    effUINT64 changedStencil = currentDrawCommand.stencil ^ drawCommand.stencil;
    currentDrawCommand.stencil = newStencil;

    if (0 != changedStencil)
    {
        effBOOL enable = 0 != newStencil;
        //DX_CHECK(D3D9Device->SetRenderState(D3DRS_STENCILENABLE, enable));

        if (0 != newStencil)
        {
            effUINT fstencil = UnpackStencil(0, newStencil);
            effUINT bstencil = UnpackStencil(1, newStencil);
            effBYTE frontAndBack = bstencil != EFF3D_STENCIL_NONE && bstencil != fstencil;
            //DX_CHECK(D3D9Device->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, 0 != frontAndBack));

            effUINT fchanged = UnpackStencil(0, changedStencil);
            if ((EFF3D_STENCIL_FUNC_REF_MASK | EFF3D_STENCIL_FUNC_RMASK_MASK) & fchanged)
            {
                effUINT ref = (fstencil & EFF3D_STENCIL_FUNC_REF_MASK) >> EFF3D_STENCIL_FUNC_REF_SHIFT;
                //DX_CHECK(D3D9Device->SetRenderState(D3DRS_STENCILREF, ref));

                effUINT rmask = (fstencil & EFF3D_STENCIL_FUNC_RMASK_MASK) >> EFF3D_STENCIL_FUNC_RMASK_SHIFT;
                //DX_CHECK(D3D9Device->SetRenderState(D3DRS_STENCILMASK, rmask));
            }

            // 						uint32_t bchanged = unpackStencil(1, changedStencil);
            // 						if (EFF3D_STENCIL_FUNC_RMASK_MASK & bchanged)
            // 						{
            // 							uint32_t wmask = (bstencil&EFF3D_STENCIL_FUNC_RMASK_MASK)>>EFF3D_STENCIL_FUNC_RMASK_SHIFT;
            // 							DX_CHECK(device->SetRenderState(D3DRS_STENCILWRITEMASK, wmask) );
            // 						}

            for (effBYTE ii = 0, num = frontAndBack + 1; ii < num; ++ii)
            {
                effUINT stencil = UnpackStencil(ii, newStencil);
                effUINT changed = UnpackStencil(ii, changedStencil);

                if ((EFF3D_STENCIL_TEST_MASK | EFF3D_STENCIL_FUNC_REF_MASK | EFF3D_STENCIL_FUNC_RMASK_MASK) & changed)
                {
                    effUINT func = (stencil & EFF3D_STENCIL_TEST_MASK) >> EFF3D_STENCIL_TEST_SHIFT;
                    //DX_CHECK(D3D9Device->SetRenderState(s_stencilFuncRs[ii], s_cmpFunc[func]));
                }

                if ((EFF3D_STENCIL_OP_FAIL_S_MASK | EFF3D_STENCIL_OP_FAIL_Z_MASK | EFF3D_STENCIL_OP_PASS_Z_MASK) & changed)
                {
                    effUINT sfail = (stencil & EFF3D_STENCIL_OP_FAIL_S_MASK) >> EFF3D_STENCIL_OP_FAIL_S_SHIFT;
                    //DX_CHECK(D3D9Device->SetRenderState(s_stencilFailRs[ii], s_stencilOp[sfail]));

                    effUINT zfail = (stencil & EFF3D_STENCIL_OP_FAIL_Z_MASK) >> EFF3D_STENCIL_OP_FAIL_Z_SHIFT;
                    //DX_CHECK(D3D9Device->SetRenderState(s_stencilZFailRs[ii], s_stencilOp[zfail]));

                    effUINT zpass = (stencil & EFF3D_STENCIL_OP_PASS_Z_MASK) >> EFF3D_STENCIL_OP_PASS_Z_SHIFT;
                    //DX_CHECK(D3D9Device->SetRenderState(s_stencilZPassRs[ii], s_stencilOp[zpass]));
                }
            }
        }
    }
}

/*static effVOID SetSamplerStateReal(LPDIRECT3DDEVICE9EX device, DWORD stage, D3DSAMPLERSTATETYPE type, DWORD value)
{
    DX_CHECK(device->SetSamplerState(stage, type, value));
    if (4 > stage)
    {
        DX_CHECK(device->SetSamplerState(D3DVERTEXTEXTURESAMPLER0 + stage, type, value));
    }
}*/

inline effBOOL NeedBorderColor(effUINT flags)
{
    return EFF3D_TEXTURE_U_BORDER == (flags & EFF3D_TEXTURE_U_BORDER)
        || EFF3D_TEXTURE_V_BORDER == (flags & EFF3D_TEXTURE_V_BORDER)
        || EFF3D_TEXTURE_W_BORDER == (flags & EFF3D_TEXTURE_W_BORDER);
}

effVOID EFFD3D12Device::SetSamplerState(effUINT stage, effUINT flags, const float rgba[4])
{
    const effUINT newFlags = flags & ((~EFF3D_TEXTURE_RESERVED_MASK) | EFF3D_TEXTURE_SAMPLER_BITS_MASK | EFF3D_TEXTURE_SRGB);

    if (samplerFlags[stage] != newFlags)
    {
        samplerFlags[stage] = newFlags;

        /*D3DTEXTUREADDRESS tau = s_textureAddress[(flags & EFF3D_TEXTURE_U_MASK) >> EFF3D_TEXTURE_U_SHIFT];
        D3DTEXTUREADDRESS tav = s_textureAddress[(flags & EFF3D_TEXTURE_V_MASK) >> EFF3D_TEXTURE_V_SHIFT];
        D3DTEXTUREADDRESS taw = s_textureAddress[(flags & EFF3D_TEXTURE_W_MASK) >> EFF3D_TEXTURE_W_SHIFT];
        D3DTEXTUREFILTERTYPE minFilter = s_textureFilter[(flags & EFF3D_TEXTURE_MIN_MASK) >> EFF3D_TEXTURE_MIN_SHIFT];
        D3DTEXTUREFILTERTYPE magFilter = s_textureFilter[(flags & EFF3D_TEXTURE_MAG_MASK) >> EFF3D_TEXTURE_MAG_SHIFT];
        D3DTEXTUREFILTERTYPE mipFilter = s_textureFilter[(flags & EFF3D_TEXTURE_MIP_MASK) >> EFF3D_TEXTURE_MIP_SHIFT];

        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_ADDRESSU, tau);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_ADDRESSV, tav);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_ADDRESSW, taw);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_MINFILTER, minFilter);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_MAGFILTER, magFilter);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_MIPFILTER, mipFilter);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_MAXANISOTROPY, maxAnisotropy);
        SetSamplerStateReal(D3D9Device, stage, D3DSAMP_SRGBTEXTURE, 0 != (newFlags & EFF3D_TEXTURE_SRGB));

		if (NULL != rgba)
		{
			if (NeedBorderColor(flags))
			{
				DWORD bc = D3DCOLOR_COLORVALUE(rgba[0], rgba[1], rgba[2], rgba[3]);
				SetSamplerStateReal(D3D9Device
					, stage
					, D3DSAMP_BORDERCOLOR
					, bc
				);
			}
		}*/
    }
}

effVOID EFFD3D12Device::SetTextures(EFF3DDrawCommand & drawCommand)
{

    for (effINT stage = 0; stage < EFF3D_CONFIG_MAX_TEXTURE_SAMPLERS; ++stage)
    {
        const EFF3DBinding & textureBind = drawCommand.textureBind.binds[stage];
        EFF3DBinding & currentTextureBind = currentDrawCommand.textureBind.binds[stage];

        if (currentTextureBind.handle != textureBind.handle
            || currentTextureBind.data.draw.flags != textureBind.data.draw.flags
            || programChanged)
        {

            EFFD3D12Texture * texture = (EFFD3D12Texture *)textureManager->GetResource(textureBind.handle);
            if (texture != NULL)
            {
                texture->Commit(stage, textureBind.data.draw.flags, NULL);
            }
            else
            {
                //DX_CHECK(D3D9Device->SetTexture(stage, NULL));
            }
        }

        currentTextureBind = textureBind;
    }
}

/*effBOOL EFFD3D12Device::SetTextureStageState(effUINT stage, EFF3DTEXTURESTAGESTATETYPE type, effUINT value)
{
	return SUCCEEDED(D3D9Device->SetTextureStageState(stage, (D3DTEXTURESTAGESTATETYPE)type, value));
}

effBOOL EFFD3D12Device::SetSamplerState(effUINT Sampler, EFF3DSAMPLERSTATETYPE Type, effUINT Value)
{
	return SUCCEEDED(D3D9Device->SetSamplerState(Sampler, (D3DSAMPLERSTATETYPE)Type, Value)); 
}*/

effBOOL EFFD3D12Device::SetRenderTarget(effUINT renderTargetIndex, EFF3DTextureHandle renderTargetHandle)
{
    //EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)renderTarget;
    EFFD3D12Texture * effD3D12Texture = (EFFD3D12Texture *)textureManager->GetResource(renderTargetHandle);
    currentRenderTarget = effD3D12Texture;

    /*LPDIRECT3DSURFACE9 surface = NULL;
	DX_CHECK(effD3D9Texture->texture->GetSurfaceLevel(0, &surface));

	//EFFD3D9Surface * effD3D9Surface = (EFFD3D9Surface *)renderTarget;
    DX_CHECK(D3D9Device->SetRenderTarget(renderTargetIndex, surface));

    surface->Release();*/



	return effTRUE;
}

effBOOL EFFD3D12Device::SetTexture(effUINT sampler, EFF3DTextureHandle textureHandle)
{
    EFFD3D12Texture * texture = NULL;

	if ( texture != NULL )
	{
		if ( texture->GetImageInfo().type == EFF3DResourceType_Texture2D )
		{
			EFFD3D12Texture * effD3D12Texture = (EFFD3D12Texture *)texture;
			//return SUCCEEDED(D3D9Device->SetTexture(sampler, (IDirect3DTexture9 *)effD3D9Texture->texture));
		}
		return effTRUE;
	}
	else
	{
		//return SUCCEEDED(D3D9Device->SetTexture(sampler, NULL));
	}

	return effTRUE;
}

effBOOL EFFD3D12Device::SetDepthStencil(EFF3DTextureHandle depthStencilHandle)
{
    //EFFD3D9Texture * effD3D9Texture = (EFFD3D9Texture *)depthStencil;
    EFFD3D12Texture * effD3D12Texture = NULL;


    /*LPDIRECT3DSURFACE9 surface = NULL;
    effD3D9Texture->texture->GetSurfaceLevel(0, &surface);


	effHRESULT hr = D3D9Device->SetDepthStencilSurface(surface);

    surface->Release();

    return SUCCEEDED(hr);*/

	return effTRUE;
}

effBOOL EFFD3D12Device::SetShader(EFF3DShader * shader)
{
	EFFD3D12Shader * effD3D12Shader = (EFFD3D12Shader *)shader;

	//cgD3D9BindProgram(effD3D12Shader->GetVertexShader());
	//cgD3D9BindProgram(effD3D12Shader->GetPixelShader());

	return effTRUE;
}

effBOOL EFFD3D12Device::SetScissorRect(const EFFRect * rect)
{
	RECT longRect;
	longRect.left = (long)rect->left;
	longRect.top = (long)rect->top;
	longRect.right = (long)rect->right;
	longRect.bottom = (long)rect->bottom;

	//return SUCCEEDED(D3D9Device->SetScissorRect(&longRect));
	return effTRUE;
}

effVOID EFFD3D12Device::Release()
{
	delete this;
}

EFF3DTextureHandle EFFD3D12Device::GetRenderTarget(effUINT index)
{
    return currentRenderTarget->id;
}


/*effBOOL EFFD3D12Device::GetViewport(EFF3DVIEWPORT9 * viewport)
{
	return SUCCEEDED(D3D9Device->GetViewport((D3DVIEWPORT9 *)viewport));
}*/


/*effBOOL	EFFD3D12Device::CheckFormatSupport(effUINT * width, effUINT * height, effUINT * numMipLevels, effUINT usage, EFF3DFORMAT * format, EFF3DPOOL pool)
{
	return SUCCEEDED(D3DXCheckTextureRequirements(D3D9Device, width, height, numMipLevels, usage, (D3DFORMAT *)format, (D3DPOOL)pool));
}*/

effBOOL EFFD3D12Device::HasVertexStreamChanged()
{
	if (currentDrawCommand.streamMask != newDrawCommand->streamMask
		|| currentDrawCommand.instanceDataBufferHandle != newDrawCommand->instanceDataBufferHandle
		|| currentDrawCommand.instanceDataOffset != newDrawCommand->instanceDataOffset
		|| currentDrawCommand.instanceDataStride != newDrawCommand->instanceDataStride)
	{
		return effTRUE;
	}

	for (effUINT idx = 0, streamMask = newDrawCommand->streamMask, ntz = effUINT_cnttz(streamMask)
		; 0 != streamMask
		; streamMask >>= 1, idx += 1, ntz = effUINT_cnttz(streamMask)
		)
	{
		streamMask >>= ntz;
		idx += ntz;

		if (currentDrawCommand.stream[idx].vertexBufferHandle != newDrawCommand->stream[idx].vertexBufferHandle
			|| currentDrawCommand.stream[idx].startVertex != newDrawCommand->stream[idx].startVertex)
		{
			return effTRUE;
		}
	}

	return effFALSE;
}


EFFD3D12DeviceCommandQueue * EFFD3D12Device::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type/* = D3D12_COMMAND_LIST_TYPE_DIRECT*/)
{
	EFFD3D12DeviceCommandQueue * commandQueue = NULL;
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		commandQueue = directCommandQueue;
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		commandQueue = computeCommandQueue;
		break;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		commandQueue = copyCommandQueue;
		break;
	default:
		assert(false && "Invalid command queue type.");
	}

	return commandQueue;
}


effVOID EFFD3D12Device::Flush()
{
	directCommandQueue->Flush();
	computeCommandQueue->Flush();
	copyCommandQueue->Flush();
}

EFFD3D12DescriptorAllocation EFFD3D12Device::AllocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE type, effUINT32 numDescriptors)
{
	return descriptorAllocators[type]->Allocate(numDescriptors);
}

effVOID EFFD3D12Device::ReleaseStaleDescriptors(effUINT64 finishedFrame)
{
	for (int i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i)
	{
		descriptorAllocators[i]->ReleaseStaleDescriptors(finishedFrame);
	}
}


ComPtr<ID3D12DescriptorHeap> EFFD3D12Device::CreateDescriptorHeap(effUINT32 numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = type;
	desc.NumDescriptors = numDescriptors;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	DX_CHECK(d3d12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

	return descriptorHeap;
}

effUINT32 EFFD3D12Device::GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type) const
{
	return d3d12Device->GetDescriptorHandleIncrementSize(type);
}

effVOID	EFFD3D12Device::UpdateBuffer(ComPtr<ID3D12Resource> d3d12Resource, effUINT32 offset, effUINT32 size, const effVOID * data, D3D12_RESOURCE_FLAGS flags)
{
	if (data == NULL)
	{
		return;
	}

	EFFD3D12Device * device = (EFFD3D12Device *)EFF3DGetDevice();
	EFFD3D12DeviceCommandQueue * commandQueue = device->GetCommandQueue(D3D12_COMMAND_LIST_TYPE_COPY);
	EFFD3D12DeviceCommandList * commandList = commandQueue->GetCommandList();
	EFFD3D12ResourceStateManager * resourceStateManager = device->GetResourceStateManager();

	// Create an upload resource to use as an intermediate buffer to copy the buffer resource 
	ComPtr<ID3D12Resource> uploadResource;
	DX_CHECK(device->GetD3D12Device()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploadResource)));

	D3D12_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pData = data;
	subresourceData.RowPitch = size;
	subresourceData.SlicePitch = subresourceData.RowPitch;

	resourceStateManager->TransitionResource(d3d12Resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST);
	resourceStateManager->FlushResourceBarriers(commandList);

	UpdateSubresources(commandList->GetD3D12CommandList(), d3d12Resource.Get(),
		uploadResource.Get(), 0, 0, 1, &subresourceData);

	// Add references to resources so they stay in scope until the command list is reset.
	commandList->TrackResource(uploadResource);

	commandList->TrackResource(d3d12Resource);
}