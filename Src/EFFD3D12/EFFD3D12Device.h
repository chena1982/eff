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
class EFFD3D12DeviceCommandQueue;
class EFFD3D12ResourceStateManager;
class EFFD3D12DescriptorAllocation;
class EFFD3D12DescriptorAllocator;

class EFFD3D12Device : public EFF3DDevice
{
	friend class EFFD3D12IndexBuffer;
	friend class EFFD3D12VertexBuffer;
public:
	EFFD3D12Device();
	~EFFD3D12Device();
public:
	virtual effBOOL				BeginScene();
	virtual effBOOL				EndScene();
	virtual effBOOL				Clear(effUINT count, const EFFRect * rects, effUINT flags, EFF3DCOLOR color, effFLOAT z, effUINT stencil);
	virtual effBOOL				Present(const EFFRect * sourceRect, const EFFRect * destRect);
	virtual effBOOL				Reset(effBOOL window, effINT width, effINT height);
	


	virtual effBOOL				CreateIndexBuffer(effVOID * data, effUINT size, effUINT flags, EFF3DIndexBufferHandle * ibHandle);
	virtual effBOOL             UpdateIndexBuffer(effUINT offset, effVOID * data, effUINT size);

	virtual effBOOL				CreateVertexBuffer(effVOID * data, effUINT size, effUINT flags, EFF3DVertexDeclarationHandle vertexDeclHandle,
									EFF3DVertexBufferHandle * vertexBufferHandle);
	virtual effBOOL             UpdateVertexBuffer(effUINT offset, effVOID * data, effUINT size);

    virtual effBOOL				CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT flags, EFF3DFormat format, EFF3DResourceType resourceType,
                                    EFF3DTextureHandle * textureHandle, effSIZE sharedHandle);

    virtual effBOOL				CreateTextureFromMemory(effVOID * srcData, effUINT srcDataSize, effINT width, effINT height, effINT level, effUINT flags,
                                    EFF3DFormat format, EFF3DResourceType resourceType, EFF3DTextureHandle * textureHandle);


	/*virtual effBOOL				CreateRenderTarget(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample,
													effUINT multisampleQuality, effBOOL lockable, EFF3DSurface ** surface);

	virtual effBOOL				CreateDepthStencilSurface(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample,
													effUINT multisampleQuality, effBOOL discard, EFF3DSurface ** surface);*/
	


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
	//virtual effBOOL				CheckFormatSupport(effUINT * width, effUINT * height, effUINT * numMipLevels, effUINT usage, EFF3DFormat * format);

    effVOID                     SetSamplerState(effUINT stage, effUINT flags, const float rgba[4]);
public:
	virtual effVOID				Release();


	effBOOL						Init(effBOOL window, HWND hWnd, effINT width, effINT height);

public:

	ID3D12Device2 *					GetD3D12Device() { return d3d12Device.Get(); }

	EFFD3D12DeviceCommandQueue *	GetCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
	ID3D12GraphicsCommandList *		GetCommandList() { return commandList.Get(); }
	EFFD3D12ResourceStateManager *	GetResourceStateManager() { return resourceStateManager; }


protected:
    /*virtual effBOOL				_CreateSharedTexture(effUINT width, effUINT height, effUINT levels, effUINT usage, EFF3DFORMAT format,
                                                    EFF3DSharedTexture ** texture);

    virtual effBOOL             _CreateSharedTexture(SharedTextureInfo * sharedTextureInfo, EFF3DSharedTexture ** texture);

    virtual effBOOL				CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT flag, EFF3DFormat format, EFF3DResourceType resourceType,
                                                    EFF3DTexture * texture);

    virtual effBOOL				CreateTextureFromMemory(effVOID * srcData, effUINT srcDataSize, effINT width, effINT height, effINT level, effUINT flag,
                                                    EFF3DFormat format, EFF3DResourceType resourceType, EFF3DTexture * texture);*/


	virtual EFF3DResource *			CreateEmptyResourceImpl(EFF3DResourceType resourceType);

	ComPtr<IDXGIAdapter4>			GetAdapter(effBOOL useWarp);
	ComPtr<ID3D12Device2>			CreateD3D12Device(ComPtr<IDXGIAdapter4> adapter);
	ComPtr<IDXGISwapChain4>			CreateSwapChain(HWND hWnd, effINT width, effINT height);

	effVOID							SetVertices();
	effVOID							SetIndexBuffer();
    effVOID							SetInputLayout(effBYTE numStreams, const EFF3DVertexDeclaration ** vertexDecls, effUINT16 numInstanceData);

	effBOOL							HasVertexStreamChanged();

	effVOID							Flush();

	EFFD3D12DescriptorAllocation	AllocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE type, effUINT32 numDescriptors);
	effVOID							ReleaseStaleDescriptors(effUINT64 finishedFrame);


	ComPtr<ID3D12DescriptorHeap>	CreateDescriptorHeap(effUINT32 numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type);
	effUINT32						GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type) const;

	effVOID							UpdateBuffer(ComPtr<ID3D12Resource> d3d12Resource, effUINT32 offset, effUINT32 size, const effVOID * data, D3D12_RESOURCE_FLAGS flags);
protected:


	EFFD3D12ResourceStateManager *		resourceStateManager;

	const static effUINT32 numFrames = 3;

	ComPtr<ID3D12Device2>				d3d12Device;
	EFFD3D12DeviceCommandQueue *		directCommandQueue;
	EFFD3D12DeviceCommandQueue *		computeCommandQueue;
	EFFD3D12DeviceCommandQueue *		copyCommandQueue;
	EFFD3D12DescriptorAllocator *		descriptorAllocators[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];


	ComPtr<IDXGISwapChain4>				dxgiSwapChain;

	ComPtr<ID3D12Resource>				backBuffers[numFrames];
	ComPtr<ID3D12GraphicsCommandList>	commandList;
	ComPtr<ID3D12CommandAllocator>		commandAllocators[numFrames];
	ComPtr<ID3D12DescriptorHeap>		RTVDescriptorHeap;

	effUINT32						RTVDescriptorSize = 0;
	effUINT32						currentBackBufferIndex = 0;
	//CGcontext					cgContext;


	// Synchronization objects
	ComPtr<ID3D12Fence>			fence;
	effUINT64					fenceValues[numFrames] = {};
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