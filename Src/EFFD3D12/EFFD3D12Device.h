/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\EFFEngine\Src\EFFD3D12.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D12Device_2019_03_23__
#define __EFFD3D12Device_2019_03_23__


#include <EASTL/bonus/ring_buffer.h>

class EFFD3D12Texture;

struct CommandQueueD3D12
{
	CommandQueueD3D12()
		: currentFence(0)
		, completedFence(0)
		, control(EFF_COUNTOF(commandList))
	{
		EFF_STATIC_ASSERT(EFF_COUNTOF(commandList) == EFF_COUNTOF(release));
	}

	effVOID Init(ID3D12Device * device);
	effVOID Shutdown();
	ID3D12GraphicsCommandList * Alloc();
	effUINT64 Kick();
	effVOID Finish(effUINT64 waitFence = UINT64_MAX, effBOOL finishAll = effFALSE);
	effBOOL TryFinish(effUINT64 waitFence);
	effVOID Release(ID3D12Resource * ptr);
	effBOOL Consume(effUINT ms = INFINITE);

	struct CommandList
	{
		ID3D12GraphicsCommandList * commandList;
		ID3D12CommandAllocator * commandAllocator;
		HANDLE event;
	};

	ID3D12CommandQueue * commandQueue;
	effUINT64 currentFence;
	effUINT64 completedFence;
	ID3D12Fence * fence;
	CommandList commandList[256];
	typedef VECTOR<ID3D12Resource *> ResourceArray;
	ResourceArray release[256];
	RingBufferControl control;
};


class EFFD3D12Device : public EFF3DDevice
{
public:
	EFFD3D12Device();
	~EFFD3D12Device();
public:
	virtual effBOOL				BeginScene();
	virtual effBOOL				EndScene();
	virtual effBOOL				Clear(effUINT count, const EFFRect * rects, effUINT flags, EFF3DCOLOR color, effFLOAT z, effUINT stencil);
	virtual effBOOL				Present(const EFFRect * sourceRect, const EFFRect * destRect);
	virtual effBOOL				Reset(effBOOL window, effINT width, effINT height);
	

    virtual effBOOL				CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT flag, EFF3DTextureFormat format, EFF3DResourceType resourceType,
                                        EFF3DTextureHandle * textureHandle, effSIZE sharedHandle);

    virtual effBOOL				CreateTextureFromMemory(effVOID * srcData, effUINT srcDataSize, effINT width, effINT height, effINT level, effUINT flag,
                                        EFF3DTextureFormat format, EFF3DResourceType resourceType, EFF3DTextureHandle * textureHandle);


	/*virtual effBOOL				CreateRenderTarget(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample,
													effUINT multisampleQuality, effBOOL lockable, EFF3DSurface ** surface);

	virtual effBOOL				CreateDepthStencilSurface(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample,
													effUINT multisampleQuality, effBOOL discard, EFF3DSurface ** surface);*/
	
	virtual effBOOL				CreateIndexBuffer(effVOID * data, effUINT size, effUINT flag, EFF3DIndexBufferHandle * ibHandle);
    virtual effBOOL             UpdateIndexBuffer(effUINT offset, effVOID * data, effUINT size);

	virtual effBOOL				CreateVertexBuffer(effVOID * data, effUINT size, effUINT flag, EFF3DVertexDeclarationHandle vertexDeclHandle,
									EFF3DVertexBufferHandle * vertexBufferHandle);
    virtual effBOOL             UpdateVertexBuffer(effUINT offset, effVOID * data, effUINT size);

	//virtual effBOOL				CreateVertexDeclaration(const EFF3DVertexElement * vertexElements, EFF3DVertexDeclaration ** decl);

    virtual effBOOL             CreateTimeQuery(effUINT flag, EFF3DTimeQueryHandle * query);

    virtual effBOOL				DrawIndexedPrimitive(EFF3DPrimitiveType type, effINT baseVertexIndex, effUINT minIndex, effUINT numVertices,
                                        effUINT startIndex, effUINT primitiveCount);

    virtual effBOOL				DrawIndexedPrimitiveUP(EFF3DPrimitiveType primitiveType, effUINT minVertexIndex, effUINT numVertices, effUINT primitiveCount,
                                        const effVOID * indexData, effBOOL index16,
                                        const effVOID * vertexStreamZeroData, effUINT vertexStreamZeroStride);

    virtual effBOOL				DrawPrimitive(EFF3DPrimitiveType primitiveType, effUINT startVertex, effUINT primitiveCount);

    virtual effBOOL				DrawPrimitiveUP(EFF3DPrimitiveType primitiveType, effUINT primitiveCount, const effVOID * vertexStreamZeroData,
                                        effUINT vertexStreamZeroStride);

	//virtual effBOOL				SetTransform(EFF3DTRANSFORMSTATETYPE state, const EFFMatrix4 * matrix);
	//virtual effBOOL				SetFVF(effUINT FVF);

	//virtual effBOOL				SetStreamSource(effUINT streamNumber, EFF3DVertexBufferHandle vbHandle, effUINT offsetInBytes, effUINT stride);
	virtual effBOOL				SetIndexBuffer(EFF3DIndexBufferHandle ibHandle);
	//virtual effBOOL				SetRenderState(EFF3DRENDERSTATETYPE state, effUINT value);

    virtual effVOID             SetRenderState(EFF3DDrawCommand & drawCommand);
    virtual effVOID             SetDepthStencilState(EFF3DDrawCommand & drawCommand);
    virtual effVOID             SetTextures(EFF3DDrawCommand & drawCommand);

    virtual effVOID             Draw(EFF3DDrawCommand & drawCommand);


	//virtual effBOOL				SetTextureStageState(effUINT stage, EFF3DTEXTURESTAGESTATETYPE type, effUINT value);
	//virtual effBOOL				SetSamplerState(effUINT Sampler, EFF3DSAMPLERSTATETYPE Type, effUINT Value);
	virtual effBOOL				SetRenderTarget(effUINT renderTargetIndex, EFF3DTextureHandle renderTargetHandle);
	virtual effBOOL				SetTexture(effUINT sampler, EFF3DTextureHandle textureHandle);
	virtual effBOOL				SetDepthStencil(EFF3DTextureHandle depthStencilHandle);

	virtual effBOOL				SetShader(EFF3DShader * shader);
	virtual effBOOL				SetScissorRect(const EFFRect * rect);

	virtual EFF3DTextureHandle  GetRenderTarget(effUINT index);
	//virtual effBOOL				GetViewport(EFF3DVIEWPORT9 * viewport);
	//virtual effBOOL				CheckFormatSupport(effUINT * width, effUINT * height, effUINT * numMipLevels, effUINT usage, EFF3DTextureFormat * format);

    effVOID                     SetSamplerState(effUINT stage, effUINT flags, const float rgba[4]);
public:
	virtual effVOID				Release();


	effBOOL						Init(effBOOL window, HWND hWnd, effINT width, effINT height);

public:

	//effVOID						SetCGContex(CGcontext cgContext) { this->cgContext = cgContext; }
	//CGcontext					GetCGContex() { return cgContext; }

protected:
    /*virtual effBOOL				_CreateSharedTexture(effUINT width, effUINT height, effUINT levels, effUINT usage, EFF3DFORMAT format,
                                                    EFF3DSharedTexture ** texture);

    virtual effBOOL             _CreateSharedTexture(SharedTextureInfo * sharedTextureInfo, EFF3DSharedTexture ** texture);

    virtual effBOOL				CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT flag, EFF3DTextureFormat format, EFF3DResourceType resourceType,
                                                    EFF3DTexture * texture);

    virtual effBOOL				CreateTextureFromMemory(effVOID * srcData, effUINT srcDataSize, effINT width, effINT height, effINT level, effUINT flag,
                                                    EFF3DTextureFormat format, EFF3DResourceType resourceType, EFF3DTexture * texture);*/


	virtual EFF3DResource *		CreateEmptyResourceImpl(EFF3DResourceType resourceType);



	effVOID						SetVertices();
	effVOID						SetIndexBuffer();
    effVOID                     SetInputLayout(effBYTE numStreams, const EFF3DVertexDeclaration ** vertexDecls, effUINT16 numInstanceData);

	effBOOL						HasVertexStreamChanged();


	effUINT64					Signal();
	effVOID						WaitForFenceValue(effUINT64 _fenceValue);
	effVOID						Flush();

protected:


	const static effUINT numFrames = 3;

	ComPtr<ID3D12Device2>		d3d12Device;
	ComPtr<ID3D12CommandQueue>	d3d12CommandQueue;
	ComPtr<IDXGISwapChain4>		dxgiSwapChain;

	ComPtr<ID3D12Resource>		backBuffers[numFrames];
	ComPtr<ID3D12GraphicsCommandList>	commandList;
	ComPtr<ID3D12CommandAllocator>		commandAllocators[numFrames];
	ComPtr<ID3D12DescriptorHeap>		RTVDescriptorHeap;
	effUINT						RTVDescriptorSize = 0;
	effUINT						currentBackBufferIndex = 0;
	//CGcontext					cgContext;


	// Synchronization objects
	ComPtr<ID3D12Fence>			fence;
	effUINT64					fenceValue = 0;
	effUINT64					frameFenceValues[numFrames] = {};
	effHANDLE					fenceEvent;

	// By default, enable V-Sync.
	// Can be toggled with the V key.
	effBOOL						VSync = effFALSE;
	effBOOL						tearingSupported = effFALSE;

	// By default, use windowed mode.
	// Can be toggled with the Alt+Enter or F11
	effBOOL						fullscreen = effFALSE;

    EFFD3D12Texture *           currentRenderTarget;

    EFF3DDrawCommand            currentDrawCommand;
	EFF3DDrawCommand *			newDrawCommand;
    effBOOL                     programChanged;

    EFF3DVertexDeclaration      vertexDecls[EFF3D_CONFIG_MAX_VERTEX_DECLS];
    effUINT                     samplerFlags[EFF3D_CONFIG_MAX_TEXTURE_SAMPLERS];
    effUINT                     maxAnisotropy;

	PrimInfo					primitiveInfo;
    //MAP<effUINT, LPDIRECT3DVERTEXDECLARATION9>  inputLayoutCache;


};

effVOID InitFullScreen(effINT width, effINT height, HWND hWnd);
effVOID InitWindow(effINT width, effINT height, effBOOL enableAA, effUINT depthStencilFormat);

EFFD3D12_API effBOOL effCreate3DDevice(EFF3DDevice ** eff3DDevice, effBOOL window, HWND hWnd, effINT width, effINT height);




#endif