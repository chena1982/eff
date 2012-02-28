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
	virtual effHRESULT			BeginScene();
	virtual effHRESULT			EndScene();
	virtual effHRESULT			Clear(effUINT Count, const EFFRect * pRects, effUINT Flags, EFF3DCOLOR Color, effFLOAT Z, effUINT Stencil);
	virtual effHRESULT			Present(const EFFRect * pSourceRect, const EFFRect * pDestRect);


	virtual effHRESULT			CreateTexture(effUINT Width, effUINT Height, effUINT Levels, effUINT Usage, EFF3DFORMAT Format, EFF3DPOOL Pool, EFF3DTexture** ppTexture);
	virtual effHRESULT			CreateRenderTarget(effUINT Width, effUINT Height, EFF3DFORMAT Format, EFF3DMULTISAMPLE_TYPE MultiSample, effUINT MultisampleQuality, effBOOL Lockable, EFF3DSurface** ppSurface);
	virtual effHRESULT			CreateDepthStencilSurface(effUINT Width, effUINT Height, EFF3DFORMAT Format, EFF3DMULTISAMPLE_TYPE MultiSample, effUINT MultisampleQuality, effBOOL Discard, EFF3DSurface** ppSurface);

	virtual effHRESULT			CreateIndexBuffer(effUINT Length, effUINT Usage, EFF3DFORMAT Format, EFF3DPOOL Pool, EFF3DIndexBuffer** ppIndexBuffer);
	virtual effHRESULT			CreateVertexBuffer(effUINT Length, effUINT Usage, effUINT FVF,EFF3DPOOL Pool, EFF3DVertexBuffer** ppVertexBuffer);
	virtual effHRESULT			CreateVertexDeclaration(const EFF3DVERTEXELEMENT * pVertexElements, EFF3DVertexDeclaration** ppDecl);

	virtual effHRESULT			DrawIndexedPrimitive(EFF3DPRIMITIVETYPE Type, effINT BaseVertexIndex, effUINT MinIndex, effUINT NumVertices, effUINT StartIndex,effUINT PrimitiveCount);

	virtual effHRESULT			DrawIndexedPrimitiveUP(EFF3DPRIMITIVETYPE PrimitiveType, effUINT MinVertexIndex, effUINT NumVertices, effUINT PrimitiveCount,
															const void * pIndexData, EFF3DFORMAT IndexDataFormat,
															const void* pVertexStreamZeroData, effUINT VertexStreamZeroStride);

	virtual effHRESULT			DrawPrimitive(EFF3DPRIMITIVETYPE PrimitiveType,effUINT StartVertex,effUINT PrimitiveCount);
	virtual effHRESULT			DrawPrimitiveUP(EFF3DPRIMITIVETYPE PrimitiveType,effUINT PrimitiveCount,const void* pVertexStreamZeroData,
															effUINT VertexStreamZeroStride);

	virtual effHRESULT			SetTransform(EFF3DTRANSFORMSTATETYPE State, const EFFMatrix4 * pMatrix);
	virtual effHRESULT			SetFVF(effUINT FVF);
	virtual effHRESULT			SetVertexDeclaration(EFF3DVertexDeclaration * pDecl);
	virtual effHRESULT			SetStreamSource(effUINT StreamNumber, EFF3DVertexBuffer * pStreamData, effUINT OffsetInBytes, effUINT Stride);
	virtual effHRESULT			SetIndices(EFF3DIndexBuffer * pIndexData);
	virtual effHRESULT			SetRenderState(EFF3DRENDERSTATETYPE State, effUINT Value);
	virtual effHRESULT			SetTextureStageState(effUINT Stage, EFF3DTEXTURESTAGESTATETYPE Type, effUINT Value);
	virtual effHRESULT			SetRenderTarget(effUINT RenderTargetIndex, EFF3DSurface * pRenderTarget);
	virtual effHRESULT			SetTexture(effUINT Sampler, EFF3DBaseTexture * pTexture);
	virtual effHRESULT			SetDepthStencilSurface(EFF3DSurface * pNewZStencil);
public:
	virtual effVOID				Release();

public:
	LPDIRECT3D9 &				GetD3D9() { return D3D9; }
	effVOID						SetD3D9(LPDIRECT3D9 D3D9) { this->D3D9 = D3D9; } 
	LPDIRECT3DDEVICE9 &			GetD3D9Device() { return D3D9Device; }

protected:
	virtual EFF3DResource *	CreateEmptyResource(EFF3DRESOURCETYPE resourceType);
protected:
	LPDIRECT3D9					D3D9;       
	LPDIRECT3DDEVICE9			D3D9Device;
};

effVOID InitFullScreen(effINT nWidth, effINT nHeight, D3DPRESENT_PARAMETERS * d3dpp, HWND hWnd);
effVOID InitWindow(effINT with,effINT height, effBOOL bFSA, D3DPRESENT_PARAMETERS * d3dpp, effUINT dwDSFormat);

effBOOL EFFD3D9_API effCreate3DDevice(EFF3DDevice** lpp3DDevice, effBOOL bWindow, HWND hWnd, effINT nWidth, effINT nHeight, effBOOL bFSA = effFALSE, effBOOL bMultiThread = effFALSE, effUINT dwDSFormat = EFF3DFMT_D16);
typedef effBOOL (*effCREATE3DDEVICE)(EFF3DDevice** lpp3DDevice, effBOOL bWindow, HWND hWnd, effINT nWidth, effINT nHeight, effBOOL bFSA, effBOOL bMultiThread, effUINT dwDSFormat);


#endif