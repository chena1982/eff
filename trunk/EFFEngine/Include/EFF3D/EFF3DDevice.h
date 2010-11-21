/******************************************************************************
	created:	2008-12-1   23:14
	file path:	d:\EFF\EFFEngine\Include\EFF3D.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DDevice_2008_12_1__
#define __EFF3DDevice_2008_12_1__

EFF3D_BEGIN

class EFF3DBaseTexture;
class EFF3DTexture;
class EFF3DVertexBuffer;
class EFF3DVertexDeclaration;
class EFF3DIndexBuffer;
class EFF3DSurface;
class EFF3DAsyncLoader;
class EFF3DImageManager;
class EFF3DIResource;
class EFF3DAutoParamDataSource;
class EFF3DMaterial;

class EFF3D_API EFF3DDevice
{
	friend class EFF3DResourceManager;
public:
	EFF3DDevice();
	virtual ~EFF3DDevice();
public:
	//这一层是渲染的抽象层，打算支持OpenGL和D3D，接口的设计方面直接使用D3D的接口，可能加入一点小的修改
	//因为OpenGL和D3D的接口不一样，有3种设计方案
	//1.使用OpenGL的接口，使用D3D渲染时，把各种接口转到D3D
	//2.使用D3D的接口，使用OpenGL的时候，把各种接口转到OpenGL
	//3.使用自己的接口，使用D3D和OpenGL时都做转换
	//第3种方案麻烦且没什么意义，第1种方案和第2种方案比的话，我觉得游戏这个行业还是D3D使用更方便，所以选择使用第2种方案


	virtual effHRESULT			BeginScene() = 0;
	virtual effHRESULT			EndScene() = 0;
	virtual effHRESULT			Clear(effUINT Count,const EFFRect * pRects,effUINT Flags,EFF3DCOLOR Color,effFLOAT Z,effUINT Stencil) = 0;
	virtual effHRESULT			Present(const EFFRect * pSourceRect,const EFFRect * pDestRect) = 0;

	
	virtual effHRESULT			CreateTexture(effUINT Width,effUINT Height,effUINT Levels,effUINT Usage,EFF3DFORMAT Format,EFF3DPOOL Pool,
													EFF3DTexture** ppTexture) = 0;
	virtual effHRESULT			CreateRenderTarget(effUINT Width,effUINT Height,EFF3DFORMAT Format,EFF3DMULTISAMPLE_TYPE MultiSample,
													effUINT MultisampleQuality,effBOOL Lockable,EFF3DSurface** ppSurface) = 0;
	virtual effHRESULT			CreateDepthStencilSurface(effUINT Width,effUINT Height,EFF3DFORMAT Format,EFF3DMULTISAMPLE_TYPE MultiSample,
													effUINT MultisampleQuality,effBOOL Discard,EFF3DSurface** ppSurface) = 0;
	
	virtual effHRESULT			CreateIndexBuffer(effUINT Length,effUINT Usage,EFF3DFORMAT Format,EFF3DPOOL Pool,
													EFF3DIndexBuffer** ppIndexBuffer) = 0;
	virtual effHRESULT			CreateVertexBuffer(effUINT Length,effUINT Usage,effUINT FVF,EFF3DPOOL Pool,EFF3DVertexBuffer** ppVertexBuffer) = 0;
	virtual effHRESULT			CreateVertexDeclaration(const EFF3DVERTEXELEMENT * pVertexElements,EFF3DVertexDeclaration** ppDecl) = 0;

	virtual effHRESULT			DrawIndexedPrimitive(EFF3DPRIMITIVETYPE Type,effINT BaseVertexIndex,effUINT MinIndex,effUINT NumVertices,
													effUINT StartIndex,effUINT PrimitiveCount) = 0;
	virtual effHRESULT			DrawIndexedPrimitiveUP(EFF3DPRIMITIVETYPE PrimitiveType,effUINT MinVertexIndex,effUINT NumVertices,effUINT PrimitiveCount,
													const void * pIndexData,EFF3DFORMAT IndexDataFormat,
													const void* pVertexStreamZeroData,effUINT VertexStreamZeroStride) = 0;
	virtual effHRESULT			DrawPrimitive(EFF3DPRIMITIVETYPE PrimitiveType,effUINT StartVertex,effUINT PrimitiveCount) = 0;
	virtual effHRESULT			DrawPrimitiveUP(EFF3DPRIMITIVETYPE PrimitiveType,effUINT PrimitiveCount,const void* pVertexStreamZeroData,
													effUINT VertexStreamZeroStride) = 0;

	virtual effHRESULT			SetTransform(EFF3DTRANSFORMSTATETYPE State,const EFFMatrix4 * pMatrix) = 0;
	virtual effHRESULT			SetFVF(effUINT FVF) = 0;
	virtual effHRESULT			SetVertexDeclaration(EFF3DVertexDeclaration * pDecl) = 0;
	virtual effHRESULT			SetStreamSource(effUINT StreamNumber,EFF3DVertexBuffer * pStreamData,effUINT OffsetInBytes,effUINT Stride) = 0;
	virtual effHRESULT			SetIndices(EFF3DIndexBuffer * pIndexData) = 0;
	virtual effHRESULT			SetRenderState(EFF3DRENDERSTATETYPE State,effUINT Value) = 0;
	virtual effHRESULT			SetTextureStageState(effUINT Stage,EFF3DTEXTURESTAGESTATETYPE Type,effUINT Value) = 0;
	virtual effHRESULT			SetRenderTarget(effUINT RenderTargetIndex,EFF3DSurface * pRenderTarget) = 0;
	virtual effHRESULT			SetTexture(effUINT Sampler,EFF3DBaseTexture * pTexture) = 0;
	virtual effHRESULT			SetDepthStencilSurface(EFF3DSurface * pNewZStencil) = 0;

	//下面是一些经过包装的方便使用的接口，这样使用引擎的人既可以快速方便的渲染一些东西，也可以使用上面的接口，以便于更细粒度的控制
	//不至于像某些引擎全部包装好了，你必须按照它的模式写代码，想增加一些功能时想要更细粒度的控制却发现不可能

	virtual effHRESULT			DrawQuad(EFF3DTexture * pTexture,EFF3DMaterial * pMaterial = NULL,EFFRect * pRect = NULL);
public:
	virtual effVOID					Release() = 0;

public:
	EFF3DImageManager *		GetImageManager() { return m_pImageManager; }
	EFF3DAsyncLoader *			GetAsyncLoader() { return m_pAsyncLoader; }
	Awesomium::WebCore *	GetWebCore() { return m_pWebCore; }
	effVOID							SetBackBufferSize(effINT nWidth,effINT nHeight);
	effINT								GetBackBufferWidth() { return m_nWidth; }
	effINT								GetBackBufferHeight() { return m_nHeight; }

protected:
	virtual EFF3DIResource *	CreateEmptyResource(EFF3DRESOURCETYPE resourceType) = 0;
	virtual effHRESULT			DrawQuad(EFFRect * pRect);
private:

	EFF3DImageManager *		m_pImageManager;
	EFF3DAsyncLoader *			m_pAsyncLoader;

	Awesomium::WebCore *	m_pWebCore;

	effINT								m_nWidth;
	effINT								m_nHeight;

	EFF3DAutoParamDataSource *		m_pAutoParamDataSource;
};


typedef effBOOL (*effCREATE3DDEVICE)(EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA,effBOOL bMultiThread,effUINT dwDSFormat);

effBOOL EFF3D_API Create3DDevice(effLPCTSTR pszDllName,EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA = effFALSE,effBOOL bMultiThread = effFALSE,effUINT dwDSFormat = EFF3DFMT_D16);


EFF3D_END

#endif