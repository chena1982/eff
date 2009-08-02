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

class EFF3DImageManager;

class EFF3D_API EFF3DDevice
{
public:
	EFF3DDevice();
	virtual ~EFF3DDevice();
public:
	//��һ������Ⱦ�ĳ���㣬����֧��OpenGL��D3D���ӿڵ���Ʒ���ֱ��ʹ��D3D�Ľӿڣ����ܼ���һ��С���޸�
	//��ΪOpenGL��D3D�Ľӿڲ�һ������3����Ʒ���
	//1.ʹ��OpenGL�Ľӿڣ�ʹ��D3D��Ⱦʱ���Ѹ��ֽӿ�ת��D3D
	//2.ʹ��D3D�Ľӿڣ�ʹ��OpenGL��ʱ�򣬰Ѹ��ֽӿ�ת��OpenGL
	//3.ʹ���Լ��Ľӿڣ�ʹ��D3D��OpenGLʱ����ת��
	//��3�ַ����鷳��ûʲô���壬��1�ַ����͵�2�ַ����ȵĻ����Ҿ�����Ϸ�����ҵ����D3Dʹ�ø����㣬����ѡ��ʹ�õ�2�з���


	virtual effHRESULT			BeginScene() = 0;
	virtual effHRESULT			EndScene() = 0;
	virtual effHRESULT			Clear(effDWORD Count,const EFFRect * pRects,effDWORD Flags,EFF3DCOLOR Color,effFLOAT Z,effDWORD Stencil) = 0;
	virtual effHRESULT			Present(const EFFRect * pSourceRect,const EFFRect * pDestRect) = 0;

	
	virtual effHRESULT			CreateTexture(effUINT Width,effUINT Height,effUINT Levels,effDWORD Usage,EFF3DFORMAT Format,EFF3DPOOL Pool,
													EFF3DTexture** ppTexture) = 0;
	virtual effHRESULT			CreateRenderTarget(effUINT Width,effUINT Height,EFF3DFORMAT Format,EFF3DMULTISAMPLE_TYPE MultiSample,
													effDWORD MultisampleQuality,effBOOL Lockable,EFF3DSurface** ppSurface) = 0;
	virtual effHRESULT			CreateDepthStencilSurface(effUINT Width,effUINT Height,EFF3DFORMAT Format,EFF3DMULTISAMPLE_TYPE MultiSample,
													effDWORD MultisampleQuality,effBOOL Discard,EFF3DSurface** ppSurface) = 0;
	
	virtual effHRESULT			CreateIndexBuffer(effUINT Length,effDWORD Usage,EFF3DFORMAT Format,EFF3DPOOL Pool,
													EFF3DIndexBuffer** ppIndexBuffer) = 0;
	virtual effHRESULT			CreateVertexBuffer(effUINT Length,effDWORD Usage,effDWORD FVF,EFF3DPOOL Pool,EFF3DVertexBuffer** ppVertexBuffer) = 0;
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
	virtual effHRESULT			SetFVF(effDWORD FVF) = 0;
	virtual effHRESULT			SetVertexDeclaration(EFF3DVertexDeclaration * pDecl) = 0;
	virtual effHRESULT			SetStreamSource(effUINT StreamNumber,EFF3DVertexBuffer * pStreamData,effUINT OffsetInBytes,effUINT Stride) = 0;
	virtual effHRESULT			SetIndices(EFF3DIndexBuffer * pIndexData) = 0;
	virtual effHRESULT			SetRenderState(EFF3DRENDERSTATETYPE State,effDWORD Value) = 0;
	virtual effHRESULT			SetTextureStageState(effDWORD Stage,EFF3DTEXTURESTAGESTATETYPE Type,effDWORD Value) = 0;
	virtual effHRESULT			SetRenderTarget(effDWORD RenderTargetIndex,EFF3DSurface * pRenderTarget) = 0;
	virtual effHRESULT			SetTexture(effDWORD Sampler,EFF3DBaseTexture * pTexture) = 0;
	virtual effHRESULT			SetDepthStencilSurface(EFF3DSurface * pNewZStencil) = 0;

	//������һЩ������װ�ķ���ʹ�õĽӿڣ�����ʹ��������˼ȿ��Կ��ٷ������ȾһЩ������Ҳ����ʹ������Ľӿڣ��Ա��ڸ�ϸ���ȵĿ���
	//��������ĳЩ����ȫ����װ���ˣ�����밴������ģʽд���룬������һЩ����ʱ��Ҫ��ϸ���ȵĿ���ȴ���ֲ�����
public:
	virtual effVOID					Release() = 0;

public:
	EFF3DImageManager *		GetImageManager() { return m_pImageManager; }
private:

	EFF3DImageManager *		m_pImageManager;
};


typedef effBOOL (*effCREATE3DDEVICE)(EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA,effBOOL bMultiThread,effDWORD dwDSFormat);

effBOOL EFF3D_API Create3DDevice(effLPCTSTR pszDllName,EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA = effFALSE,effBOOL bMultiThread = effFALSE,effDWORD dwDSFormat = EFF3DFMT_D16);


EFF3D_END

#endif