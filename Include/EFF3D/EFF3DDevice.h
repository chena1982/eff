/******************************************************************************
	created:	2008-12-1   23:14
	file path:	d:\EFF\EFFEngine\Include\EFF3D.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DDevice_2008_12_1__
#define __EFF3DDevice_2008_12_1__


/*namespace Awesomium {
	class WebCore;
};*/

EFF3D_BEGIN

class EFF3DImage;
class EFF3DTexture;
class EFF3DSharedTexture;
class EFF3DVertexBuffer;
class EFF3DVertexDeclaration;
class EFF3DIndexBuffer;
class EFF3DSurface;
class EFF3DImageManager;
class EFF3DResource;
class EFF3DMaterial;
class EFF3DShader;
class EFF3DSceneManager;
class EFF3DFontManager;
class EFF3DInputManager;
class EFF3DQuery;


class EFF3D_API EFF3DDevice
{
	friend class EFF3DResourceManager;
public:
	EFF3DDevice();
	virtual ~EFF3DDevice();
public:
	//��һ������Ⱦ�ĳ���㣬����֧��OpenGL��D3D���ӿڵ���Ʒ���ֱ��ʹ��D3D�Ľӿڣ����ܼ���һ��С���޸�
	//��ΪOpenGL��D3D�Ľӿڲ�һ������3����Ʒ���
	//1.ʹ��OpenGL�Ľӿڣ�ʹ��D3D��Ⱦʱ���Ѹ��ֽӿ�ת��D3D
	//2.ʹ��D3D�Ľӿڣ�ʹ��OpenGL��ʱ�򣬰Ѹ��ֽӿ�ת��OpenGL
	//3.ʹ���Լ��Ľӿڣ�ʹ��D3D��OpenGLʱ����ת��
	//��3�ַ����鷳��ûʲô���壬��1�ַ����͵�2�ַ����ȵĻ����Ҿ�����Ϸ�����ҵ����D3Dʹ�ø����㣬����ѡ��ʹ�õ�2�ַ���


	virtual effBOOL				BeginScene() = 0;
	virtual effBOOL				EndScene() = 0;
	virtual effBOOL				Clear(effUINT count, const EFFRect * rects, effUINT flags, EFF3DCOLOR color, effFLOAT z, effUINT stencil) = 0;
	virtual effBOOL				Present(const EFFRect * sourceRect, const EFFRect * destRect) = 0;
	virtual effBOOL				Reset(effBOOL window, effINT width, effINT height) = 0;
	
	virtual effBOOL				CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT usage, EFF3DFORMAT format, EFF3DPOOL pool,
													EFF3DTexture ** texture, effHANDLE * handle = NULL) = 0;

	virtual effBOOL				CreateSharedTexture(effUINT width, effUINT height, effUINT levels, effUINT usage, EFF3DFORMAT format,
                                                    EFF3DSharedTexture ** texture);

    virtual effBOOL             CreateSharedTexture(SharedTextureInfo * sharedTextureInfo, EFF3DSharedTexture ** texture);

	virtual effBOOL				CreateTextureFromFile(const effString & filePath, EFF3DTexture ** texture) = 0;

	virtual effBOOL				CreateTextureFromMemory(effVOID * srcData, effUINT srcDataSize, effUINT usage, EFF3DFORMAT format, effINT width, effINT height,
													effINT level, EFF3DTexture ** texture) = 0;

	virtual effBOOL				CreateRenderTarget(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample,
													effUINT multisampleQuality, effBOOL lockable, EFF3DSurface ** surface) = 0;

	virtual effBOOL				CreateDepthStencilSurface(effUINT width, effUINT height, EFF3DFORMAT format, EFF3DMULTISAMPLE_TYPE multiSample,
													effUINT multisampleQuality, effBOOL discard, EFF3DSurface ** surface) = 0;
	
	virtual effBOOL				CreateIndexBuffer(effUINT length, effUINT usage, EFF3DFORMAT format, EFF3DPOOL pool,
													EFF3DIndexBuffer ** indexBuffer) = 0;

	virtual effBOOL				CreateVertexBuffer(effUINT length, effUINT usage, effUINT FVF, EFF3DPOOL pool, EFF3DVertexBuffer ** vertexBuffer) = 0;

	virtual effBOOL				CreateVertexDeclaration(const EFF3DVERTEXELEMENT * vertexElements, EFF3DVertexDeclaration ** decl) = 0;

    virtual effBOOL             CreateQuery(EFF3DQUERYTYPE type, effUINT flag, EFF3DQuery ** query) = 0;

	virtual effBOOL				DrawIndexedPrimitive(EFF3DPRIMITIVETYPE type, effINT baseVertexIndex, effUINT minIndex,effUINT numVertices,
													effUINT startIndex, effUINT primitiveCount) = 0;

	virtual effBOOL				DrawIndexedPrimitiveUP(EFF3DPRIMITIVETYPE primitiveType, effUINT minVertexIndex, effUINT numVertices, effUINT primitiveCount,
													const effVOID * indexData, EFF3DFORMAT indexDataFormat,
													const effVOID * vertexStreamZeroData, effUINT vertexStreamZeroStride) = 0;

	virtual effBOOL				DrawPrimitive(EFF3DPRIMITIVETYPE primitiveType, effUINT startVertex, effUINT primitiveCount) = 0;

	virtual effBOOL				DrawPrimitiveUP(EFF3DPRIMITIVETYPE primitiveType, effUINT primitiveCount, const effVOID * vertexStreamZeroData,
								                    effUINT vertexStreamZeroStride) = 0;

	virtual effBOOL				SetTransform(EFF3DTRANSFORMSTATETYPE state, const EFFMatrix4 * matrix) = 0;
	virtual effBOOL				SetFVF(effUINT FVF) = 0;
	virtual effBOOL				SetVertexDeclaration(EFF3DVertexDeclaration * decl) = 0;
	virtual effBOOL				SetStreamSource(effUINT streamNumber, EFF3DVertexBuffer * streamData, effUINT offsetInBytes, effUINT stride) = 0;
	virtual effBOOL				SetIndices(EFF3DIndexBuffer * indexData) = 0;
	virtual effBOOL				SetRenderState(EFF3DRENDERSTATETYPE state, effUINT value) = 0;
	virtual effBOOL				SetTextureStageState(effUINT stage, EFF3DTEXTURESTAGESTATETYPE type, effUINT value) = 0;
	virtual effBOOL				SetSamplerState(effUINT Sampler, EFF3DSAMPLERSTATETYPE Type, effUINT Value) = 0;
	virtual effBOOL				SetRenderTarget(effUINT renderTargetIndex, EFF3DSurface * renderTarget) = 0;
	virtual effBOOL				SetTexture(effUINT sampler, EFF3DImage * texture) = 0;
	virtual effBOOL				SetDepthStencilSurface(EFF3DSurface * newZStencil) = 0;

	virtual effBOOL				SetShader(EFF3DShader * shader) = 0;
	virtual effBOOL				SetScissorRect(const EFFRect * rect) = 0; 


	virtual effBOOL				GetRenderTarget(effUINT index, EFF3DSurface ** surface) = 0;
	virtual effBOOL				GetViewport(EFF3DVIEWPORT9 * viewport) = 0;
	virtual effBOOL				CheckFormatSupport(effUINT * width, effUINT * height, effUINT * numMipLevels, effUINT usage, EFF3DFORMAT * format, EFF3DPOOL pool) = 0;

	//������һЩ������װ�ķ���ʹ�õĽӿڣ�����ʹ��������˼ȿ��Կ��ٷ������ȾһЩ������Ҳ����ʹ������Ľӿڣ��Ա��ڸ�ϸ���ȵĿ���
	//��������ĳЩ����ȫ����װ���ˣ�����밴������ģʽд���룬������һЩ����ʱ��Ҫ��ϸ���ȵĿ���ȴ���ֲ�����

	virtual effBOOL				DrawQuad(EFFRect * rect, effDWORD color);
	virtual effBOOL				DrawQuad(EFFRect * rect, EFF3DTexture * texture, effBOOL blend);
	virtual effBOOL				DrawQuad(EFFRect * rect, EFF3DMaterial * material, EFF3DTexture * texture);
public:
	virtual effVOID				Release() = 0;

public:
	inline EFF3DImageManager *	GetImageManager() { return imageManager; }
	inline EFF3DSceneManager *	GetSceneManager() { return sceneManager; }
	inline EFF3DFontManager *	GetFontManager() { return fontManager; }
	inline EFFInputManager *	GetInputManager() { return inputManager; }
    inline EFF3DSharedTexture * GetSharedRenderTarget() { return sharedRenderTarget; }
	//inline Awesomium::WebCore * GetWebCore() { return webCore; }
    inline EFFBase::EFFEntityManager *          GetEntityManager() { return entityManager; }
    inline EFFBase::EFFStaticStringManager *    GetStaticStringManager() { return staticStringManager; }

	effVOID						SetBackBufferSize(effINT width,effINT weight);
	effINT						GetBackBufferWidth() { return width; }
	effINT						GetBackBufferHeight() { return height; }


	effVOID						Update();

    effVOID                     InitSharedTexture(SharedTextureInfo * sharedTextureInfo);
    effBOOL                     IsHost() { return host; }

protected:
    /*virtual effBOOL				_CreateSharedTexture(effUINT width, effUINT height, effUINT levels, effUINT usage, EFF3DFORMAT format,
                                                    EFF3DSharedTexture ** texture) = 0;

    virtual effBOOL             _CreateSharedTexture(SharedTextureInfo * sharedTextureInfo, EFF3DSharedTexture ** texture) = 0;*/

	virtual EFF3DResource *		CreateEmptyResource(EFF3DRESOURCETYPE resourceType) = 0;
	virtual effBOOL				DrawQuad(EFFRect * rect);
protected:
	effVOID						Init(effBOOL host);
	effVOID						InitProperty();
	friend effBOOL EFF3D_API	Create3DDevice(const effString & dllName, EFF3DDevice ** eff3DDevice, effBOOL window, effBOOL host, HWND hWnd, effINT width, effINT height);


public:
    EFFEvent                    OnNotifyHostStartRendering;
private:

	EFF3DImageManager *			imageManager;
	EFF3DSceneManager *			sceneManager;
	EFF3DFontManager *			fontManager;
	EFFInputManager *			inputManager;
    EFFEntityManager *          entityManager;
    EFFStaticStringManager *    staticStringManager;
	//Awesomium::WebCore *		webCore;

	effINT						width;
	effINT						height;

    //process share render target
    EFF3DSharedTexture *        sharedRenderTarget;
    effBOOL                     host;



};

struct QuadVertex
{
	effFLOAT			x,y,z,rhw;
	effFLOAT			u,v;
	const static effUINT fvf = EFF3DFVF_XYZRHW | EFF3DFVF_TEX1;
};

struct QuadColoredVertex
{
	effFLOAT			x,y,z,rhw;
	effUINT				color;

	const static effUINT fvf = EFF3DFVF_XYZRHW | EFF3DFVF_DIFFUSE;
};


EFF3D_API EFFINLINE EFF3DDevice * EFF3DGetDevice();
EFF3D_API effINT EFF3DGetPixelSizeFromFormat(EFF3DFORMAT format);


EFF3D_END

#endif