/******************************************************************************
	created:	2008-12-2   0:19
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9Device_2008_12_2__
#define __EFFD3D9Device_2008_12_2__


class EFFD3D9Texture;


class EFFD3D9Device : public EFF3DDevice
{
public:
	EFFD3D9Device();
	~EFFD3D9Device();
public:
	virtual effBOOL				BeginScene();
	virtual effBOOL				EndScene();
	virtual effBOOL				Clear(effUINT count, const EFFRect * rects, effUINT flags, EFF3DCOLOR color, effFLOAT z, effUINT stencil);
	virtual effBOOL				Present(const EFFRect * sourceRect, const EFFRect * destRect);
	virtual effBOOL				Reset(effBOOL window, effINT width, effINT height);
	

	/*virtual effBOOL				CreateRenderTarget(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample,
													effUINT multisampleQuality, effBOOL lockable, EFF3DSurface ** surface);

	virtual effBOOL				CreateDepthStencilSurface(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample,
													effUINT multisampleQuality, effBOOL discard, EFF3DSurface ** surface);*/
	
	virtual effBOOL				CreateIndexBuffer(effVOID * data, effUINT size, effUINT flag, EFF3DIndexBufferHandle * ibHandle);
    virtual effBOOL             UpdateIndexBuffer(effUINT offset, effVOID * data, effUINT size);

	virtual effBOOL				CreateVertexBuffer(effVOID * data, effUINT size, effUINT flag, EFF3DVertexBufferHandle * vbHandle);
    virtual effBOOL             UpdateVertexBuffer(effUINT offset, effVOID * data, effUINT size);

	//virtual effBOOL				CreateVertexDeclaration(const EFF3DVertexElement * vertexElements, EFF3DVertexDeclaration ** decl);

    virtual effBOOL             CreateQuery(EFF3DQueryType type, effUINT flag, EFF3DQuery ** query);

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
	virtual effVOID				SetVertexDeclaration();
	//virtual effBOOL				SetStreamSource(effUINT streamNumber, EFF3DVertexBufferHandle vbHandle, effUINT offsetInBytes, effUINT stride);
	virtual effBOOL				SetIndices(EFF3DIndexBufferHandle ibHandle);
	//virtual effBOOL				SetRenderState(EFF3DRENDERSTATETYPE state, effUINT value);

    virtual effVOID             SetRenderState(EFF3DDrawCommand & drawCommand);
    virtual effVOID             SetDepthStencilState(EFF3DDrawCommand & drawCommand);
    virtual effVOID             SetTextures(EFF3DDrawCommand & drawCommand);

    effVOID                     Draw(EFF3DDrawCommand & drawCommand);


	//virtual effBOOL				SetTextureStageState(effUINT stage, EFF3DTEXTURESTAGESTATETYPE type, effUINT value);
	//virtual effBOOL				SetSamplerState(effUINT Sampler, EFF3DSAMPLERSTATETYPE Type, effUINT Value);
	virtual effBOOL				SetRenderTarget(effUINT renderTargetIndex, EFF3DTextureHandle renderTarget);
	virtual effBOOL				SetTexture(effUINT sampler, EFF3DTextureHandle textureHandle);
	virtual effBOOL				SetDepthStencil(EFF3DTextureHandle depthStencilHandle);

	virtual effBOOL				SetShader(EFF3DShader * shader);
	virtual effBOOL				SetScissorRect(const EFFRect * rect);

	virtual EFF3DTextureHandle  GetRenderTarget(effUINT index);
	//virtual effBOOL				GetViewport(EFF3DVIEWPORT9 * viewport);
	//virtual effBOOL				CheckFormatSupport(effUINT * width, effUINT * height, effUINT * numMipLevels, effUINT usage, EFF3DTextureFormat * format);

    effVOID                     SetSamplerState(effUINT stage, effUINT flags);
public:
	virtual effVOID				Release();

public:
	LPDIRECT3D9EX				GetD3D9() { return D3D9; }
	effVOID						SetD3D9(LPDIRECT3D9EX D3D9) { this->D3D9 = D3D9; } 
	LPDIRECT3DDEVICE9EX	&	    GetD3D9Device() { return D3D9Device; }
	//effVOID						SetCGContex(CGcontext cgContext) { this->cgContext = cgContext; }
	//CGcontext					GetCGContex() { return cgContext; }

protected:
    /*virtual effBOOL				_CreateSharedTexture(effUINT width, effUINT height, effUINT levels, effUINT usage, EFF3DFORMAT format,
                                                    EFF3DSharedTexture ** texture);

    virtual effBOOL             _CreateSharedTexture(SharedTextureInfo * sharedTextureInfo, EFF3DSharedTexture ** texture);*/

    virtual effBOOL				CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT flag, EFF3DTextureFormat format, EFF3DResourceType resourceType,
                                                    EFF3DTexture * texture);

    virtual effBOOL				CreateTextureFromMemory(effVOID * srcData, effUINT srcDataSize, effINT width, effINT height, effINT level, effUINT flag,
                                                    EFF3DTextureFormat format, EFF3DResourceType resourceType, EFF3DTexture * texture);


	virtual EFF3DResource *		CreateEmptyResourceImpl(EFF3DResourceType resourceType);


    effVOID                     SetInputLayout(effBYTE numStreams, const EFF3DVertexDeclaration ** vertexDecls, effUINT16 numInstanceData);


protected:
	LPDIRECT3D9EX				D3D9;       
	LPDIRECT3DDEVICE9EX			D3D9Device;
	//CGcontext					cgContext;

    EFFD3D9Texture *            currentRenderTarget;

    EFF3DDrawCommand            currentDrawCommand;
    EFF3DRenderBind             currentDrawBind;
    effBOOL                     programChanged;

    EFF3DVertexDeclaration      vertexDecls[EFF3D_CONFIG_MAX_VERTEX_DECLS];
    effUINT                     samplerFlags[EFF3D_CONFIG_MAX_TEXTURE_SAMPLERS];
    effUINT                     maxAnisotropy;
    effUINT64                   renderDebugFlags;

    MAP<effUINT, LPDIRECT3DVERTEXDECLARATION9>  inputLayoutCache;


};

effVOID InitFullScreen(effINT width, effINT height, D3DPRESENT_PARAMETERS * d3dpp, HWND hWnd);
effVOID InitWindow(effINT width, effINT height, effBOOL enableAA, D3DPRESENT_PARAMETERS * d3dpp, effUINT depthStencilFormat);

EFFD3D9_API effBOOL effCreate3DDevice(EFF3DDevice ** eff3DDevice, effBOOL window, HWND hWnd, effINT width, effINT height);




#endif