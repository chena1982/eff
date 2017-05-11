/******************************************************************************
	created:	2008-12-2   0:19
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9Device_2008_12_2__
#define __EFFD3D9Device_2008_12_2__


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
	
	virtual effBOOL				CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT usage, EFF3DFORMAT format, EFF3DPOOL pool,
													EFF3DTexture ** texture);


	virtual effBOOL				CreateTextureFromFile(const effString & filePath, EFF3DTexture ** texture);

	virtual effBOOL				CreateTextureFromMemory(effVOID * srcData, effUINT srcDataSize, effUINT usage, EFF3DFORMAT format, effINT width, effINT height,
													effINT level, EFF3DTexture ** texture);

	virtual effBOOL				CreateRenderTarget(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample,
													effUINT multisampleQuality, effBOOL lockable, EFF3DSurface ** surface);

	virtual effBOOL				CreateDepthStencilSurface(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample,
													effUINT multisampleQuality, effBOOL discard, EFF3DSurface ** surface);
	
	virtual effBOOL				CreateIndexBuffer(effUINT length, effUINT usage, EFF3DFORMAT format, EFF3DPOOL pool,
													EFF3DIndexBuffer ** indexBuffer);

	virtual effBOOL				CreateVertexBuffer(effUINT length, effUINT usage, effUINT FVF, EFF3DPOOL pool, EFF3DVertexBuffer ** vertexBuffer);

	virtual effBOOL				CreateVertexDeclaration(const EFF3DVERTEXELEMENT * vertexElements, EFF3DVertexDeclaration ** decl);

	virtual effBOOL				DrawIndexedPrimitive(EFF3DPRIMITIVETYPE type, effINT baseVertexIndex, effUINT minIndex,effUINT numVertices,
													effUINT startIndex, effUINT primitiveCount);

	virtual effBOOL				DrawIndexedPrimitiveUP(EFF3DPRIMITIVETYPE primitiveType, effUINT minVertexIndex, effUINT numVertices, effUINT primitiveCount,
													const effVOID * indexData, EFF3DFORMAT indexDataFormat,
													const effVOID * vertexStreamZeroData, effUINT vertexStreamZeroStride);

	virtual effBOOL				DrawPrimitive(EFF3DPRIMITIVETYPE primitiveType, effUINT startVertex, effUINT primitiveCount);

	virtual effBOOL				DrawPrimitiveUP(EFF3DPRIMITIVETYPE primitiveType, effUINT primitiveCount, const effVOID * vertexStreamZeroData,
													effUINT vertexStreamZeroStride);

	virtual effBOOL				SetTransform(EFF3DTRANSFORMSTATETYPE state, const EFFMatrix4 * matrix);
	virtual effBOOL				SetFVF(effUINT FVF);
	virtual effBOOL				SetVertexDeclaration(EFF3DVertexDeclaration * decl);
	virtual effBOOL				SetStreamSource(effUINT streamNumber, EFF3DVertexBuffer * streamData, effUINT offsetInBytes, effUINT stride);
	virtual effBOOL				SetIndices(EFF3DIndexBuffer * indexData);
	virtual effBOOL				SetRenderState(EFF3DRENDERSTATETYPE state, effUINT value);
	virtual effBOOL				SetTextureStageState(effUINT stage, EFF3DTEXTURESTAGESTATETYPE type, effUINT value);
	virtual effBOOL				SetSamplerState(effUINT Sampler, EFF3DSAMPLERSTATETYPE Type, effUINT Value);
	virtual effBOOL				SetRenderTarget(effUINT renderTargetIndex, EFF3DSurface * renderTarget);
	virtual effBOOL				SetTexture(effUINT sampler, EFF3DImage * texture);
	virtual effBOOL				SetDepthStencilSurface(EFF3DSurface * newZStencil);

	virtual effBOOL				SetShader(EFF3DShader * shader);
	virtual effBOOL				SetScissorRect(const EFFRect * rect);

	virtual effBOOL				GetRenderTarget(effUINT index, EFF3DSurface ** surface);
	virtual effBOOL				GetViewport(EFF3DVIEWPORT9 * viewport);
	virtual effBOOL				CheckFormatSupport(effUINT * width, effUINT * height, effUINT * numMipLevels, effUINT usage, EFF3DFORMAT * format, EFF3DPOOL pool);
public:
	virtual effVOID				Release();

public:
	LPDIRECT3D9EX &				GetD3D9() { return D3D9; }
	effVOID						SetD3D9(LPDIRECT3D9EX D3D9) { this->D3D9 = D3D9; } 
	LPDIRECT3DDEVICE9EX &		GetD3D9Device() { return D3D9Device; }
	effVOID						SetCGContex(CGcontext cgContext) { this->cgContext = cgContext; }
	CGcontext					GetCGContex() { return cgContext; }

protected:
    virtual effBOOL				_CreateSharedTexture(effUINT width, effUINT height, effUINT levels, effUINT usage, EFF3DFORMAT format,
                                                    EFF3DSharedTexture ** texture);

    virtual effBOOL             _CreateSharedTexture(SharedTextureInfo * sharedTextureInfo, EFF3DSharedTexture ** texture);

	virtual EFF3DResource *		CreateEmptyResource(EFF3DRESOURCETYPE resourceType);
protected:
	LPDIRECT3D9EX				D3D9;       
	LPDIRECT3DDEVICE9EX			D3D9Device;
	CGcontext					cgContext;
};

effVOID InitFullScreen(effINT width, effINT height, D3DPRESENT_PARAMETERS * d3dpp, HWND hWnd);
effVOID InitWindow(effINT width, effINT height, effBOOL enableAA, D3DPRESENT_PARAMETERS * d3dpp, effUINT depthStencilFormat);

EFFD3D9_API effBOOL effCreate3DDevice(EFF3DDevice ** eff3DDevice, effBOOL window, HWND hWnd, effINT width, effINT height);




#endif