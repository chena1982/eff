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
	virtual effHRESULT			Clear(effDWORD Count,const EFFRect * pRects,effDWORD Flags,EFF3DCOLOR Color,effFLOAT Z,effDWORD Stencil);
	virtual effHRESULT			Present(const EFFRect * pSourceRect,const EFFRect * pDestRect);


	virtual effHRESULT			CreateTexture(effUINT Width,effUINT Height,effUINT Levels,effDWORD Usage,EFF3DFORMAT Format,EFF3DPOOL Pool,
															EFF3DTexture** ppTexture);
	virtual effHRESULT			CreateRenderTarget(effUINT Width,effUINT Height,EFF3DFORMAT Format,EFF3DMULTISAMPLE_TYPE MultiSample,
															effDWORD MultisampleQuality,effBOOL Lockable,EFF3DSurface** ppSurface);
	virtual effHRESULT			CreateDepthStencilSurface(effUINT Width,effUINT Height,EFF3DFORMAT Format,EFF3DMULTISAMPLE_TYPE MultiSample,
															effDWORD MultisampleQuality,effBOOL Discard,EFF3DSurface** ppSurface);

	virtual effHRESULT			CreateIndexBuffer(effUINT Length,effDWORD Usage,EFF3DFORMAT Format,EFF3DPOOL Pool,
															EFF3DIndexBuffer** ppIndexBuffer);
	virtual effHRESULT			CreateVertexBuffer(effUINT Length,effDWORD Usage,effDWORD FVF,EFF3DPOOL Pool,EFF3DVertexBuffer** ppVertexBuffer);
	virtual effHRESULT			CreateVertexDeclaration(const EFF3DVERTEXELEMENT * pVertexElements,EFF3DVertexDeclaration** ppDecl);

	virtual effHRESULT			DrawIndexedPrimitive(EFF3DPRIMITIVETYPE Type,effINT BaseVertexIndex,effUINT MinIndex,effUINT NumVertices,
															effUINT StartIndex,effUINT PrimitiveCount);
	virtual effHRESULT			DrawIndexedPrimitiveUP(EFF3DPRIMITIVETYPE PrimitiveType,effUINT MinVertexIndex,effUINT NumVertices,effUINT PrimitiveCount,
															const void * pIndexData,EFF3DFORMAT IndexDataFormat,
															const void* pVertexStreamZeroData,effUINT VertexStreamZeroStride);
	virtual effHRESULT			DrawPrimitive(EFF3DPRIMITIVETYPE PrimitiveType,effUINT StartVertex,effUINT PrimitiveCount);
	virtual effHRESULT			DrawPrimitiveUP(EFF3DPRIMITIVETYPE PrimitiveType,effUINT PrimitiveCount,const void* pVertexStreamZeroData,
															effUINT VertexStreamZeroStride);

	virtual effHRESULT			SetTransform(EFF3DTRANSFORMSTATETYPE State,const EFFMatrix4 * pMatrix);
	virtual effHRESULT			SetFVF(effDWORD FVF);
	virtual effHRESULT			SetVertexDeclaration(EFF3DVertexDeclaration * pDecl);
	virtual effHRESULT			SetStreamSource(effUINT StreamNumber,EFF3DVertexBuffer * pStreamData,effUINT OffsetInBytes,effUINT Stride);
	virtual effHRESULT			SetIndices(EFF3DIndexBuffer * pIndexData);
	virtual effHRESULT			SetRenderState(EFF3DRENDERSTATETYPE State,effDWORD Value);
	virtual effHRESULT			SetTextureStageState(effDWORD Stage,EFF3DTEXTURESTAGESTATETYPE Type,effDWORD Value);
	virtual effHRESULT			SetRenderTarget(effDWORD RenderTargetIndex,EFF3DSurface * pRenderTarget);
	virtual effHRESULT			SetTexture(effDWORD Sampler,EFF3DBaseTexture * pTexture);
	virtual effHRESULT			SetDepthStencilSurface(EFF3DSurface * pNewZStencil);
public:
	virtual effVOID					Release();

public:
	LPDIRECT3D9 &				GetD3D() { return m_pD3D; }
	effVOID							SetD3D(LPDIRECT3D9 pD3D) { m_pD3D = pD3D; } 
	LPDIRECT3DDEVICE9 &	GetD3DDevice() { return m_pD3DDevice; }
protected:
	LPDIRECT3D9					m_pD3D;       
	LPDIRECT3DDEVICE9		m_pD3DDevice;
};

effVOID InitFullScreen(effINT nWidth,effINT nHeight,D3DPRESENT_PARAMETERS *d3dpp,HWND hWnd);
effVOID InitWindow(effINT with,effINT height,effBOOL bFSA,D3DPRESENT_PARAMETERS *d3dpp,effDWORD dwDSFormat);

effBOOL EFFD3D9_API effCreate3DDevice(EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA = effFALSE,effBOOL bMultiThread = effFALSE,effDWORD dwDSFormat = EFF3DFMT_D16);
typedef effBOOL (*effCREATE3DDEVICE)(EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA,effBOOL bMultiThread,effDWORD dwDSFormat);


#endif