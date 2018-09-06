/******************************************************************************
	created:	2008-12-1   23:24
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DDevice.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"

#include "EFF3DResourceManager.h"
#include "EFF3DSceneManager.h"
#include "EFF3DTexture.h"
#include "EFF3DMaterial.h"
#include "EFF3DShader.h"
#include "EFF3DDevice.h"
#include "EFF3DObject.h"
#include "EFF3DFont.h"
#include "EFF3DVertexBuffer.h"
#include "EFF3DVertexDeclaration.h"
#include "EFF3DUniformBuffer.h"
#include "EFF3DRenderQueue.h"
#include "EFF3DRenderQueueManager.h"
//#include "EFF3DWebGui.h"


//#include <Awesomium/WebCore.h>
//#include <Awesomium/STLHelpers.h>

#include <direct.h>

//#define new EFFNEW

EFF3D_BEGIN

static EFF3DDevice * device = NULL;

EFFINLINE EFF3DDevice * EFF3DGetDevice()
{
	return device;
}

typedef effBOOL (*effCREATE3DDEVICE)(EFF3DDevice ** eff3DDevice, effBOOL window, HWND hWnd, effINT width, effINT height);

effBOOL EFF3D_API Create3DDevice(const effString & dllName, EFF3DDevice ** eff3DDevice, effBOOL window, effBOOL host, HWND hWnd, effINT width, effINT height)
{
	HMODULE hDLL = LoadLibrary(dllName.c_str());
	if ( hDLL != NULL )
	{
		effCREATE3DDEVICE realCreate3DDevice = (effCREATE3DDEVICE)GetProcAddress((HMODULE)hDLL, "effCreate3DDevice");
		if ( realCreate3DDevice == NULL )
		{
			return effFALSE;
		}

		effBOOL succeed = realCreate3DDevice(eff3DDevice, window, hWnd, width, height);
		if ( succeed )
		{
			device = *eff3DDevice;
			device->Init(host);
		}
	}
	return effFALSE;
}


struct EFF3DFormatInfo
{
	effUINT bpp;
	effUINT rmask;
	effUINT gmask;
	effUINT bmask;
	effUINT amask;
	EFF3DTextureFormat format;
};


EFF3DFormatInfo formatInfo[] =
{
	{ 1, 0xFF, 0, 0, 0, EFF3D_TEXTURE_FORMAT_A8 },
	{ 2, 0xf800, 0x07e0, 0x001f, 0x0000, EFF3D_TEXTURE_FORMAT_R5G6B5 },
	{ 2, 0x7c00, 0x03e0, 0x001f, 0x8000, EFF3D_TEXTURE_FORMAT_RGB5A1 },
	{ 2, 0x0f00, 0x00f0, 0x000f, 0xf000, EFF3D_TEXTURE_FORMAT_RGBA4 },
	{ 3, 0xff0000, 0x00ff00, 0x0000ff, 0x000000, EFF3D_TEXTURE_FORMAT_RGB8 },
	{ 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000, EFF3D_TEXTURE_FORMAT_RGBA8 },
	{ 4, 0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000, EFF3D_TEXTURE_FORMAT_RGB10A2 },
	{ 4, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000, EFF3D_TEXTURE_FORMAT_RG16 },
};

effINT EFF3DGetPixelSizeFromFormat(EFF3DTextureFormat format)	
{
	effINT elementCount = sizeof(formatInfo) / sizeof(EFF3DFormatInfo);
	for ( effINT i = 0; i < elementCount; i++ )
	{
		if ( formatInfo[i].format == format )
		{
			return formatInfo[i].bpp;
		}
	}

	return 0;
}



EFF3DDevice::EFF3DDevice()
{
	//Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化





	width = 0;
	height = 0;
	//m_pImageManager->CreateFromFile(_effT(""),EFF3DRTYPE_TEXTURE);

    sharedRenderTarget = NULL;

    CurrentState = EFF3D_STATE_NONE;
    CurrentStencilState = EFF3D_STENCIL_NONE;
    BlendFactor = 0;
    AtocSupport = effFALSE;
}

EFF3DDevice::~EFF3DDevice()
{
	//Py_Finalize();//调用Py_Finalize，这个根Py_Initialize相对应的。


	//Awesomium::WebCore::Shutdown();
	//webCore = NULL;

	//wkeShutdown();

    SF_DELETE(sharedRenderTarget);
	SF_DELETE(textureManager);
    SF_DELETE(vertexBufferManager);
    SF_DELETE(vertexDeclManager);
    SF_DELETE(renderQueueManager);

	SF_DELETE(sceneManager);
	SF_DELETE(fontManager);
	SF_DELETE(inputManager);
    SF_DELETE(entityManager);
    SF_DELETE(staticStringManager);
}

EFFId EFF3DDevice::CreateResourceFromFile(const effString & filePath, EFF3DResourceType resourceType)
{
    if (resourceType == EFF3DResourceType_Texture2D || resourceType == EFF3DResourceType_Texture3D || resourceType == EFF3DResourceType_TextureCube)
    {
        return textureManager->CreateFromFile(filePath, resourceType);
    }

    return EFFId();
}

effBOOL	EFF3DDevice::CreateSharedTexture(effUINT width, effUINT height, effUINT levels, effUINT flag, EFF3DTextureFormat format, EFF3DSharedTexture ** texture)
{
    /*if (!_CreateSharedTexture(width, height, levels, usage, format, texture))
    {
        return effFALSE;
    }*/

    EFF3DSharedTexture * sharedTexture = EFFNEW EFF3DSharedTexture();

    //(*texture)->name = _effT("ClientSharedTexture");

    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        CreateTexture(width, height, levels, flag, format, EFF3DResourceType_RenderTarget, &sharedTexture->textureHandle[i]);
    }

    sharedTexture->clientSemaphore.Create(3, 3, _effT("ClientSharedTextureSemaphore"));
    sharedTexture->hostSemaphore.Create(0, 1, _effT("HostSharedTextureSemaphore"));


    *texture = sharedTexture;

    return effTRUE;
}

effBOOL EFF3DDevice::CreateSharedTexture(SharedTextureInfo * sharedTextureInfo, EFF3DSharedTexture ** texture)
{
    /*if (!_CreateSharedTexture(sharedTextureInfo, texture))
    {
        return effFALSE;
    }*/


    EFF3DSharedTexture * sharedTexture = EFFNEW EFF3DSharedTexture();

    for (effINT i = 0; i < SHAREDTEXTURE_BUFFER_COUNT; i++)
    {
        effHANDLE handle = (effHANDLE)sharedTextureInfo->sharedTextureHandle[i];

        //EFF3DResource * resource = CreateEmptyResource(EFF3DResourceType_RenderTarget);
        //EFF3DTexture * texture = (EFF3DTexture *)resource;
        //texture->userData = handle;

        CreateTexture(sharedTextureInfo->width, sharedTextureInfo->height, 1, 0, 
            (EFF3DTextureFormat)sharedTextureInfo->format, EFF3DResourceType_RenderTarget, &sharedTexture->textureHandle[i]);
    }

	//(*texture)->name = _effT("HostSharedTexture");


    sharedTexture->clientSemaphore.Open(_effT("ClientSharedTextureSemaphore"));
    sharedTexture->hostSemaphore.Open(_effT("HostSharedTextureSemaphore"));

    *texture = sharedTexture;

    return effTRUE;
}

effBOOL	EFF3DDevice::CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT flag, EFF3DTextureFormat format, EFF3DResourceType resourceType, EFF3DTextureHandle * textureHandle)
{
    if (CreateTexture(width, height, levels, flag, format, resourceType, textureHandle))
    {
        return effTRUE;
    }

    return effFALSE;
}

effBOOL	EFF3DDevice::CreateTextureFromMemory(effVOID * srcData, effUINT srcDataSize, effINT width, effINT height, effINT levels, effUINT flag,
                                        EFF3DTextureFormat format, EFF3DResourceType resourceType, EFF3DTextureHandle * textureHandle)
{
    if (CreateTextureFromMemory(srcData, srcDataSize, width, height, levels, flag, format, resourceType, textureHandle))
    {
        return effTRUE;
    }

    return effFALSE;
}

effVOID EFF3DDevice::Init(effBOOL host)
{
	ilInit();


    QuadVertex::InitVertexDecl();

	textureManager = EFFNEW EFF3DTextureManager();
	//EFFRegisterObjectManager(EFF3DImage::GetThisClass(), imageManager);
	

    vertexBufferManager = EFFNEW EFF3DVertexBufferManager;
    vertexDeclManager = EFFNEW EFF3DVertexDeclarationManager;
    uniformBufferManager = EFFNEW EFF3DUniformBufferManager;

    renderQueueManager = EFFNEW EFF3DRenderQueueManager;

	sceneManager = EFFNEW EFF3DSceneManager();

	InitProperty();
    InitVertexDeclaration();

	//SetRenderState(EFF3DRS_LIGHTING, effFALSE);


	fontManager = EFFNEW EFF3DFontManager();
	fontManager->CreateFromFile(_effT("Font\\simsun.ttc"), 16);
	//fontManager->CreateFromFile(_effT("Font\\msyh.ttf"), 16);

	inputManager = EFFNEW EFFInputManager();

    entityManager = EFFNEW EFFEntityManager();

    staticStringManager = EFFNEW EFFStaticStringManager();



	//wkeInit();
	//jsBindFunction("sendMessageToCpp", js_SendMessageToCpp, 1);


	char buffer[MAX_PATH];
    _getcwd(buffer, MAX_PATH);

	//Awesomium::WebConfig config;
	//config.log_path = Awesomium::WSLit(buffer);
	//config.log_level = Awesomium::kLogLevel_Normal;
	//config.additional_options.Push(Awesomium::WSLit("disable-3d-apis"));

	//webCore = Awesomium::WebCore::Initialize(config);

    this->host = host;

    if (!host)
    {
        if (!CreateSharedTexture(width, height, 1, 0, EFF3D_TEXTURE_FORMAT_RGBA8, &sharedRenderTarget))
        {
            //create shared texture failed;
        }
    }
}


effVOID EFF3DDevice::InitProperty()
{
	EFF3DObjectInitProperty();
}


effBOOL EFF3DDevice::DrawQuad(EFFRect * rect, EFF3DRenderQueue * rendererQueue)
{

	static QuadVertex vertices[4];

	EFFRect quadRect;
	if ( rect == NULL )
	{
		quadRect.left = 0.0f;
		quadRect.right = (effFLOAT)width;
		quadRect.top = 0.0f;
		quadRect.bottom = (effFLOAT)height;
	}
	else
	{
		quadRect = *rect;
	}

	vertices[0].x = (effFLOAT)quadRect.left - 0.5f;
	vertices[0].y = (effFLOAT)quadRect.top - 0.5f;
	vertices[0].z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	vertices[1].x = (effFLOAT)quadRect.right - 0.5f;
	vertices[1].y = (effFLOAT)quadRect.top - 0.5f;
	vertices[1].z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	vertices[2].x = (effFLOAT)quadRect.right - 0.5f;
	vertices[2].y = (effFLOAT)quadRect.bottom - 0.5f;
	vertices[2].z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;


	vertices[3].x = (effFLOAT)quadRect.left - 0.5f;
	vertices[3].y = (effFLOAT)quadRect.bottom - 0.5f;
	vertices[3].z = 0.0f;
	vertices[3].rhw = 1.0f;
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;

    EFF3DCreateVertexDeclarationCommand * createVBDeclarationCommand = rendererQueue->GetCommand<EFF3DCreateVertexDeclarationCommand>();
    createVBDeclarationCommand->vbDeclHandle = vertexDeclManager->AddVertexDeclaration(QuadVertex::vertexDecl);
    createVBDeclarationCommand->vbDecl = QuadVertex::vertexDecl;


    EFF3DCreateVertexBufferCommand * createVBCommand = rendererQueue->GetCommand<EFF3DCreateVertexBufferCommand>();
    createVBCommand->data = vertices;
    createVBCommand->size = sizeof(vertices);
    createVBCommand->vbHandle = EFF3DVertexBufferHandle();
    createVBCommand->vbDeclHandle = createVBDeclarationCommand->vbDeclHandle;

    static effUINT16 indices[6] = { 0, 1, 2, 0, 2, 3 };

    static EFF3DIndexBufferHandle quadIB;
    RUN_ONCE([&] {
        EFF3DCreateIndexBufferCommand * createIBCommand = rendererQueue->GetCommand<EFF3DCreateIndexBufferCommand>();
        createIBCommand->data = indices;
        createIBCommand->size = sizeof(indices);
        createIBCommand->ibHandle = EFF3DIndexBufferHandle();
        createIBCommand->flags = 0;
    })




    EFF3DDrawCommand * drawCommand = rendererQueue->GetCommand<EFF3DDrawCommand>();
    drawCommand->numVertices = 4;
    drawCommand->numIndices = 6;
    drawCommand->indexBufferHandle = quadIB;
    drawCommand->stream[0].vertexBufferHandle = createVBCommand->vbHandle;
    drawCommand->stream[0].vertexDeclHandle = createVBDeclarationCommand->vbDeclHandle;



	//SetFVF(QuadVertex::fvf);
	//return DrawPrimitiveUP(TriangleList, 2, vertices, sizeof(QuadVertex));

    return effTRUE;
}

effBOOL EFF3DDevice::DrawQuad(EFFRect * rect, effDWORD color)
{
	/*SetRenderState(EFF3DRS_ALPHABLENDENABLE, effFALSE);
	SetRenderState(EFF3DRS_TEXTUREFACTOR, color);

	SetTextureStageState(0, EFF3DTSS_COLOROP, EFF3DTOP_SELECTARG1);
	SetTextureStageState(0, EFF3DTSS_COLORARG1, EFF3DTA_TFACTOR);*/

    EFF3DRenderQueue * rendererQueue = renderQueueManager->CreateRenderQueue(0, EFF3DRenderQueueManager::Solid, _effT("GBuffer"));
    DrawQuad(rect, rendererQueue);

    return effTRUE;
}

effBOOL EFF3DDevice::DrawQuad(EFFRect * rect, EFF3DTextureHandle textureHandle, effBOOL blend)
{
	if ( blend )
	{
		/*SetRenderState(EFF3DRS_ALPHABLENDENABLE, effTRUE);
		SetRenderState(EFF3DRS_SRCBLEND, EFF3DBLEND_SRCALPHA);
		SetRenderState(EFF3DRS_DESTBLEND, EFF3DBLEND_INVSRCALPHA);

		SetTextureStageState(0, EFF3DTSS_COLOROP, EFF3DTOP_SELECTARG1);
		SetTextureStageState(0, EFF3DTSS_COLORARG1, EFF3DTA_TEXTURE);

		SetTextureStageState(0, EFF3DTSS_ALPHAOP, EFF3DTOP_SELECTARG1);
		SetTextureStageState(0, EFF3DTSS_ALPHAARG1, EFF3DTA_TEXTURE);*/
	}
	else
	{
		//SetRenderState(EFF3DRS_ALPHABLENDENABLE, effFALSE);
	}



    EFF3DRenderQueue * rendererQueue = renderQueueManager->CreateRenderQueue(0, EFF3DRenderQueueManager::Solid, _effT("GBuffer"));
    DrawQuad(rect, rendererQueue);

	return effTRUE;
}

effBOOL EFF3DDevice::DrawQuad(EFFRect * rect, EFF3DMaterial * material, EFF3DTextureHandle textureHandle)
{
	if ( material == NULL )
	{
		return effFALSE;
	}

	EFF3DShader * shader = material->GetShader();

	if ( shader == NULL )
	{
		return effFALSE;
	}


    EFF3DRenderQueue * rendererQueue = renderQueueManager->CreateRenderQueue(0, EFF3DRenderQueueManager::Solid, _effT("GBuffer"));
    DrawQuad(rect, rendererQueue);

    return effTRUE;
}

effVOID EFF3DDevice::SetBackBufferSize(effINT width, effINT height)
{
	this->width = width;
	this->height = height;
}


effVOID EFF3DDevice::Update()
{
	if ( inputManager != NULL )
	{
		inputManager->Update();
	}

	/*if ( webCore != NULL )
	{
		webCore->Update();
	}*/
}

effVOID EFF3DDevice::InitSharedTexture(SharedTextureInfo * sharedTextureInfo)
{
    SF_DELETE(sharedRenderTarget);

    CreateSharedTexture(sharedTextureInfo, &sharedRenderTarget);
}


EFF3DResource *	EFF3DDevice::CreateEmptyResource(EFF3DResourceType resourceType)
{
    EFF3DResource * resource = CreateEmptyResourceImpl(resourceType);
    if (resourceType == EFF3DResourceType_Texture2D || resourceType == EFF3DResourceType_Texture3D || resourceType == EFF3DResourceType_TextureCube)
    {
        resource->id = textureManager->Create();
        textureManager->AddResource(resource);
    }

    return resource;
}


effVOID EFF3DDevice::InitVertexDeclaration()
{
    QuadVertex::InitVertexDecl();
}

EFF3D_END