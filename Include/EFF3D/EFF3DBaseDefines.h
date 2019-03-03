/******************************************************************************
	created:	2008-12-1   22:37
	file path:	d:\EFF\EFFEngine\Src\EFF3D.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DBaseDefines_2008_12_1__
#define __EFF3DBaseDefines_2008_12_1__



#define EFF3D_BEGIN	namespace EFF3D {
#define EFF3D_END		}
#define USE_EFF3D		using namespace EFF3D;

#define CAMERA_MAX_RENDER_LAYER 32
#define OBJECT_MAX_RENDER_LAYER 4


#ifndef EFF3D_CONFIG_MAX_COMMAND_BUFFER_SIZE
#	define EFF3D_CONFIG_MAX_COMMAND_BUFFER_SIZE (64<<10)
#endif // EFF3D_CONFIG_MAX_COMMAND_BUFFER_SIZE

#ifndef EFF3D_CONFIG_MAX_INSTANCE_DATA_COUNT
#	define EFF3D_CONFIG_MAX_INSTANCE_DATA_COUNT 5
#endif // EFF3D_CONFIG_MAX_INSTANCE_DATA_COUNT

#ifndef EFF3D_CONFIG_MAX_VERTEX_STREAMS
#	define EFF3D_CONFIG_MAX_VERTEX_STREAMS 4
#endif // EFF3D_CONFIG_MAX_VERTEX_STREAMS


#ifndef EFF3D_CONFIG_MAX_VERTEX_DECLS
#	define EFF3D_CONFIG_MAX_VERTEX_DECLS 64
#endif // EFF3D_CONFIG_MAX_VERTEX_DECLS

#ifndef EFF3D_CONFIG_MAX_TEXTURE_SAMPLERS
#	define EFF3D_CONFIG_MAX_TEXTURE_SAMPLERS 16
#endif // EFF3D_CONFIG_MAX_TEXTURE_SAMPLERS

#ifndef EFF3D_CONFIG_MAX_UNIFORM_BUFFERS
#	define EFF3D_CONFIG_MAX_UNIFORM_BUFFERS 8
#endif // EFF3D_CONFIG_MAX_UNIFORM_BUFFERS

typedef struct _EFF3DCOLORVALUE
{
	effFLOAT r;
	effFLOAT g;
	effFLOAT b;
	effFLOAT a;
} EFF3DCOLORVALUE;


typedef struct EFF3DXCOLOR
{
#ifdef __cplusplus
public:
	EFF3DXCOLOR() {}
	EFF3DXCOLOR( effUINT argb );
	EFF3DXCOLOR( const effFLOAT * );
	EFF3DXCOLOR( const EFF3DCOLORVALUE& );
	EFF3DXCOLOR( effFLOAT r, effFLOAT g, effFLOAT b, effFLOAT a );

	// casting
	operator effUINT () const;

	operator effFLOAT* ();
	operator const effFLOAT* () const;

	operator EFF3DCOLORVALUE* ();
	operator const EFF3DCOLORVALUE* () const;

	operator EFF3DCOLORVALUE& ();
	operator const EFF3DCOLORVALUE& () const;

	// assignment operators
	EFF3DXCOLOR& operator += ( const EFF3DXCOLOR& );
	EFF3DXCOLOR& operator -= ( const EFF3DXCOLOR& );
	EFF3DXCOLOR& operator *= ( effFLOAT );
	EFF3DXCOLOR& operator /= ( effFLOAT );

	// unary operators
	EFF3DXCOLOR operator + () const;
	EFF3DXCOLOR operator - () const;

	// binary operators
	EFF3DXCOLOR operator + ( const EFF3DXCOLOR& ) const;
	EFF3DXCOLOR operator - ( const EFF3DXCOLOR& ) const;
	EFF3DXCOLOR operator * ( effFLOAT ) const;
	EFF3DXCOLOR operator / ( effFLOAT ) const;

	friend EFF3DXCOLOR operator * (effFLOAT, const EFF3DXCOLOR& );

	effBOOL operator == ( const EFF3DXCOLOR& ) const;
	effBOOL operator != ( const EFF3DXCOLOR& ) const;

#endif //__cplusplus
	effFLOAT r, g, b, a;
} EFF3DXCOLOR, *LPEFF3DXCOLOR;

typedef effUINT EFF3DCOLOR;







// maps unsigned 8 bits/channel to EFF3DCOLOR
#define EFF3DCOLOR_ARGB(a,r,g,b) \
	((EFF3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define EFF3DCOLOR_RGBA(r,g,b,a) EFF3DCOLOR_ARGB(a,r,g,b)
#define EFF3DCOLOR_XRGB(r,g,b)   EFF3DCOLOR_ARGB(0xff,r,g,b)








#ifndef EFF3D_MAKEFOURCC
#define EFF3D_MAKEFOURCC(ch0, ch1, ch2, ch3) \
	((effUINT)(effBYTE)(ch0) | ((effUINT)(effBYTE)(ch1) << 8) | \
	((effUINT)(effBYTE)(ch2) << 16) | ((effUINT)(effBYTE)(ch3) << 24 ))
#endif /* defined(EFF3D_MAKEFOURCC) */




/* Multi-Sample buffer types */
typedef enum _EFF3DMULTISAMPLE_TYPE
{
	EFF3DMULTISAMPLE_NONE            =  0,
	EFF3DMULTISAMPLE_NONMASKABLE     =  1,
	EFF3DMULTISAMPLE_2_SAMPLES       =  2,
	EFF3DMULTISAMPLE_3_SAMPLES       =  3,
	EFF3DMULTISAMPLE_4_SAMPLES       =  4,
	EFF3DMULTISAMPLE_5_SAMPLES       =  5,
	EFF3DMULTISAMPLE_6_SAMPLES       =  6,
	EFF3DMULTISAMPLE_7_SAMPLES       =  7,
	EFF3DMULTISAMPLE_8_SAMPLES       =  8,
	EFF3DMULTISAMPLE_9_SAMPLES       =  9,
	EFF3DMULTISAMPLE_10_SAMPLES      = 10,
	EFF3DMULTISAMPLE_11_SAMPLES      = 11,
	EFF3DMULTISAMPLE_12_SAMPLES      = 12,
	EFF3DMULTISAMPLE_13_SAMPLES      = 13,
	EFF3DMULTISAMPLE_14_SAMPLES      = 14,
	EFF3DMULTISAMPLE_15_SAMPLES      = 15,
	EFF3DMULTISAMPLE_16_SAMPLES      = 16,

	EFF3DMULTISAMPLE_FORCE_DWORD     = 0x7fffffff
} EFF3DMULTISAMPLE_TYPE;



struct EFF3DLockedRect
{
    effINT pitch;
    effVOID * data;
};

enum EFF3DQueryType
{
    Query_TimeStamp
};

#define EFF3D_TEXTURE_INTERNAL_DEFAULT_SAMPLER  UINT32_C(0x10000000)

struct EFF3DRendererType
{
    /// Renderer types:
    enum Enum
    {
        Noop,         //!< No rendering.
        Direct3D9,    //!< Direct3D 9.0
        Direct3D11,   //!< Direct3D 11.0
        Direct3D12,   //!< Direct3D 12.0
        Gnm,          //!< GNM
        Metal,        //!< Metal
        OpenGLES,     //!< OpenGL ES 2.0+
        OpenGL,       //!< OpenGL 2.1+
        Vulkan,       //!< Vulkan

        Count
    };
};

struct EFF3DVertexAttrib
{
    enum Enum
    {
        Position,  //!< a_position
        Normal,    //!< a_normal
        Tangent,   //!< a_tangent
        Bitangent, //!< a_bitangent
        Color0,    //!< a_color0
        Color1,    //!< a_color1
        Color2,    //!< a_color2
        Color3,    //!< a_color3
        Indices,   //!< a_indices
        Weight,    //!< a_weight
        TexCoord0, //!< a_texcoord0
        TexCoord1, //!< a_texcoord1
        TexCoord2, //!< a_texcoord2
        TexCoord3, //!< a_texcoord3
        TexCoord4, //!< a_texcoord4
        TexCoord5, //!< a_texcoord5
        TexCoord6, //!< a_texcoord6
        TexCoord7, //!< a_texcoord7

        Count
    };
};





struct EFF3DVertexAttribType
{
    enum Enum
    {
        Uint8,  //!< Uint8
        Uint10, //!< Uint10, availability depends on: `EFF3D_CAPS_VERTEX_ATTRIB_UINT10`.
        Int16,  //!< Int16
        Half,   //!< Half, availability depends on: `EFF3D_CAPS_VERTEX_ATTRIB_HALF`.
        Float,  //!< Float

        Count
    };
};



#ifndef EFF3D_EXPORTS
	#define EFF3D_API		__declspec(dllimport)
#else
	#define EFF3D_API		__declspec(dllexport)
#endif

//#define from bgfx https://github.com/bkaradzic/bgfx



//EFF_HANDLE(EFF3DTextureHandle)

typedef EFFId EFF3DTextureHandle;
typedef EFFId EFF3DIndexBufferHandle;
typedef EFFId EFF3DVertexBufferHandle;
typedef EFFId EFF3DVertexDeclarationHandle;
typedef EFFId EFF3DConstantBufferHandle;
typedef EFFId EFF3DOcclusionQueryHandle;
typedef EFFId EFF3DIndirectBufferHandle;
typedef EFFId EFF3DTimeQueryHandle;

#define EFF3D_TEXTURE_NONE                UINT32_C(0x00000000) //!<
#define EFF3D_TEXTURE_U_MIRROR            UINT32_C(0x00000001) //!< Wrap U mode: Mirror
#define EFF3D_TEXTURE_U_CLAMP             UINT32_C(0x00000002) //!< Wrap U mode: Clamp
#define EFF3D_TEXTURE_U_BORDER            UINT32_C(0x00000003) //!< Wrap U mode: Border
#define EFF3D_TEXTURE_U_SHIFT             0                    //!<
#define EFF3D_TEXTURE_U_MASK              UINT32_C(0x00000003) //!<
#define EFF3D_TEXTURE_V_MIRROR            UINT32_C(0x00000004) //!< Wrap V mode: Mirror
#define EFF3D_TEXTURE_V_CLAMP             UINT32_C(0x00000008) //!< Wrap V mode: Clamp
#define EFF3D_TEXTURE_V_BORDER            UINT32_C(0x0000000c) //!< Wrap V mode: Border
#define EFF3D_TEXTURE_V_SHIFT             2                    //!<
#define EFF3D_TEXTURE_V_MASK              UINT32_C(0x0000000c) //!<
#define EFF3D_TEXTURE_W_MIRROR            UINT32_C(0x00000010) //!< Wrap W mode: Mirror
#define EFF3D_TEXTURE_W_CLAMP             UINT32_C(0x00000020) //!< Wrap W mode: Clamp
#define EFF3D_TEXTURE_W_BORDER            UINT32_C(0x00000030) //!< Wrap W mode: Border
#define EFF3D_TEXTURE_W_SHIFT             4                    //!<
#define EFF3D_TEXTURE_W_MASK              UINT32_C(0x00000030) //!<
#define EFF3D_TEXTURE_MIN_POINT           UINT32_C(0x00000040) //!< Min sampling mode: Point
#define EFF3D_TEXTURE_MIN_ANISOTROPIC     UINT32_C(0x00000080) //!< Min sampling mode: Anisotropic
#define EFF3D_TEXTURE_MIN_SHIFT           6                    //!<
#define EFF3D_TEXTURE_MIN_MASK            UINT32_C(0x000000c0) //!<
#define EFF3D_TEXTURE_MAG_POINT           UINT32_C(0x00000100) //!< Mag sampling mode: Point
#define EFF3D_TEXTURE_MAG_ANISOTROPIC     UINT32_C(0x00000200) //!< Mag sampling mode: Anisotropic
#define EFF3D_TEXTURE_MAG_SHIFT           8                    //!<
#define EFF3D_TEXTURE_MAG_MASK            UINT32_C(0x00000300) //!<
#define EFF3D_TEXTURE_MIP_POINT           UINT32_C(0x00000400) //!< Mip sampling mode: Point
#define EFF3D_TEXTURE_MIP_SHIFT           10                   //!<
#define EFF3D_TEXTURE_MIP_MASK            UINT32_C(0x00000400) //!<
#define EFF3D_TEXTURE_MSAA_SAMPLE         UINT32_C(0x00000800) //!< Texture will be used for MSAA sampling.
#define EFF3D_TEXTURE_RT                  UINT32_C(0x00001000) //!<
#define EFF3D_TEXTURE_RT_MSAA_X2          UINT32_C(0x00002000) //!< Render target MSAAx2 mode.
#define EFF3D_TEXTURE_RT_MSAA_X4          UINT32_C(0x00003000) //!< Render target MSAAx4 mode.
#define EFF3D_TEXTURE_RT_MSAA_X8          UINT32_C(0x00004000) //!< Render target MSAAx8 mode.
#define EFF3D_TEXTURE_RT_MSAA_X16         UINT32_C(0x00005000) //!< Render target MSAAx16 mode.
#define EFF3D_TEXTURE_RT_MSAA_SHIFT       12                   //!<
#define EFF3D_TEXTURE_RT_MSAA_MASK        UINT32_C(0x00007000) //!<
#define EFF3D_TEXTURE_RT_WRITE_ONLY       UINT32_C(0x00008000) //!< Render target will be used for writing only.
#define EFF3D_TEXTURE_RT_MASK             UINT32_C(0x0000f000) //!<
#define EFF3D_TEXTURE_COMPARE_LESS        UINT32_C(0x00010000) //!< Compare when sampling depth texture: less.
#define EFF3D_TEXTURE_COMPARE_LEQUAL      UINT32_C(0x00020000) //!< Compare when sampling depth texture: less or equal.
#define EFF3D_TEXTURE_COMPARE_EQUAL       UINT32_C(0x00030000) //!< Compare when sampling depth texture: equal.
#define EFF3D_TEXTURE_COMPARE_GEQUAL      UINT32_C(0x00040000) //!< Compare when sampling depth texture: greater or equal.
#define EFF3D_TEXTURE_COMPARE_GREATER     UINT32_C(0x00050000) //!< Compare when sampling depth texture: greater.
#define EFF3D_TEXTURE_COMPARE_NOTEQUAL    UINT32_C(0x00060000) //!< Compare when sampling depth texture: not equal.
#define EFF3D_TEXTURE_COMPARE_NEVER       UINT32_C(0x00070000) //!< Compare when sampling depth texture: never.
#define EFF3D_TEXTURE_COMPARE_ALWAYS      UINT32_C(0x00080000) //!< Compare when sampling depth texture: always.
#define EFF3D_TEXTURE_COMPARE_SHIFT       16                   //!<
#define EFF3D_TEXTURE_COMPARE_MASK        UINT32_C(0x000f0000) //!<
#define EFF3D_TEXTURE_COMPUTE_WRITE       UINT32_C(0x00100000) //!< Texture will be used for compute write.
#define EFF3D_TEXTURE_SRGB                UINT32_C(0x00200000) //!< Sample texture as sRGB.
#define EFF3D_TEXTURE_BLIT_DST            UINT32_C(0x00400000) //!< Texture will be used as blit destination.
#define EFF3D_TEXTURE_READ_BACK           UINT32_C(0x00800000) //!< Texture will be used for read back from GPU.
#define EFF3D_TEXTURE_BORDER_COLOR_SHIFT  24                   //!<
#define EFF3D_TEXTURE_BORDER_COLOR_MASK   UINT32_C(0x0f000000) //!<
#define EFF3D_TEXTURE_RESERVED_SHIFT      28                   //!<
#define EFF3D_TEXTURE_RESERVED_MASK       UINT32_C(0xf0000000) //!<

enum EFF3DAccess
{
    Read,
    Write,
    ReadWrite,

    AccessCount
};

enum EFF3DTextureFormat
{
    EFF3D_TEXTURE_FORMAT_BC1,          //!< DXT1
    EFF3D_TEXTURE_FORMAT_BC2,          //!< DXT3
    EFF3D_TEXTURE_FORMAT_BC3,          //!< DXT5
    EFF3D_TEXTURE_FORMAT_BC4,          //!< LATC1/ATI1
    EFF3D_TEXTURE_FORMAT_BC5,          //!< LATC2/ATI2
    EFF3D_TEXTURE_FORMAT_BC6H,         //!< BC6H
    EFF3D_TEXTURE_FORMAT_BC7,          //!< BC7
    EFF3D_TEXTURE_FORMAT_ETC1,         //!< ETC1 RGB8
    EFF3D_TEXTURE_FORMAT_ETC2,         //!< ETC2 RGB8
    EFF3D_TEXTURE_FORMAT_ETC2A,        //!< ETC2 RGBA8
    EFF3D_TEXTURE_FORMAT_ETC2A1,       //!< ETC2 RGB8A1
    EFF3D_TEXTURE_FORMAT_PTC12,        //!< PVRTC1 RGB 2BPP
    EFF3D_TEXTURE_FORMAT_PTC14,        //!< PVRTC1 RGB 4BPP
    EFF3D_TEXTURE_FORMAT_PTC12A,       //!< PVRTC1 RGBA 2BPP
    EFF3D_TEXTURE_FORMAT_PTC14A,       //!< PVRTC1 RGBA 4BPP
    EFF3D_TEXTURE_FORMAT_PTC22,        //!< PVRTC2 RGBA 2BPP
    EFF3D_TEXTURE_FORMAT_PTC24,        //!< PVRTC2 RGBA 4BPP

    EFF3D_TEXTURE_FORMAT_UNKNOWN,      // Compressed formats above.

    EFF3D_TEXTURE_FORMAT_R1,
    EFF3D_TEXTURE_FORMAT_A8,
    EFF3D_TEXTURE_FORMAT_R8,
    EFF3D_TEXTURE_FORMAT_R8I,
    EFF3D_TEXTURE_FORMAT_R8U,
    EFF3D_TEXTURE_FORMAT_R8S,
    EFF3D_TEXTURE_FORMAT_R16,
    EFF3D_TEXTURE_FORMAT_R16I,
    EFF3D_TEXTURE_FORMAT_R16U,
    EFF3D_TEXTURE_FORMAT_R16F,
    EFF3D_TEXTURE_FORMAT_R16S,
    EFF3D_TEXTURE_FORMAT_R32I,
    EFF3D_TEXTURE_FORMAT_R32U,
    EFF3D_TEXTURE_FORMAT_R32F,
    EFF3D_TEXTURE_FORMAT_RG8,
    EFF3D_TEXTURE_FORMAT_RG8I,
    EFF3D_TEXTURE_FORMAT_RG8U,
    EFF3D_TEXTURE_FORMAT_RG8S,
    EFF3D_TEXTURE_FORMAT_RG16,
    EFF3D_TEXTURE_FORMAT_RG16I,
    EFF3D_TEXTURE_FORMAT_RG16U,
    EFF3D_TEXTURE_FORMAT_RG16F,
    EFF3D_TEXTURE_FORMAT_RG16S,
    EFF3D_TEXTURE_FORMAT_RG32I,
    EFF3D_TEXTURE_FORMAT_RG32U,
    EFF3D_TEXTURE_FORMAT_RG32F,
    EFF3D_TEXTURE_FORMAT_RGB8,
    EFF3D_TEXTURE_FORMAT_RGB8I,
    EFF3D_TEXTURE_FORMAT_RGB8U,
    EFF3D_TEXTURE_FORMAT_RGB8S,
    EFF3D_TEXTURE_FORMAT_RGB9E5F,
    EFF3D_TEXTURE_FORMAT_BGRA8,
    EFF3D_TEXTURE_FORMAT_RGBA8,
    EFF3D_TEXTURE_FORMAT_RGBA8I,
    EFF3D_TEXTURE_FORMAT_RGBA8U,
    EFF3D_TEXTURE_FORMAT_RGBA8S,
    EFF3D_TEXTURE_FORMAT_RGBA16,
    EFF3D_TEXTURE_FORMAT_RGBA16I,
    EFF3D_TEXTURE_FORMAT_RGBA16U,
    EFF3D_TEXTURE_FORMAT_RGBA16F,
    EFF3D_TEXTURE_FORMAT_RGBA16S,
    EFF3D_TEXTURE_FORMAT_RGBA32I,
    EFF3D_TEXTURE_FORMAT_RGBA32U,
    EFF3D_TEXTURE_FORMAT_RGBA32F,
    EFF3D_TEXTURE_FORMAT_R5G6B5,
    EFF3D_TEXTURE_FORMAT_RGBA4,
    EFF3D_TEXTURE_FORMAT_RGB5A1,
    EFF3D_TEXTURE_FORMAT_RGB10A2,
    EFF3D_TEXTURE_FORMAT_RG11B10F,

    EFF3D_TEXTURE_FORMAT_UNKNOWN_DEPTH, // Depth formats below.

    EFF3D_TEXTURE_FORMAT_D16,
    EFF3D_TEXTURE_FORMAT_D24,
    EFF3D_TEXTURE_FORMAT_D24S8,
    EFF3D_TEXTURE_FORMAT_D32,
    EFF3D_TEXTURE_FORMAT_D16F,
    EFF3D_TEXTURE_FORMAT_D24F,
    EFF3D_TEXTURE_FORMAT_D32F,
    EFF3D_TEXTURE_FORMAT_D0S8,

    EFF3D_TEXTURE_FORMAT_COUNT
};


enum EFF3DResourceType
{
    EFF3DResourceType_Texture2D,
    EFF3DResourceType_Texture3D,
    EFF3DResourceType_TextureCube,
    EFF3DResourceType_RenderTarget,
    EFF3DResourceType_DepthStencil,
    EFF3DResourceType_TextureReadBack,
    EFF3DResourceType_IndexBuffer,
	EFF3DResourceType_VertexDeclaration,
    EFF3DResourceType_VertexBuffer,
	EFF3DResourceType_TimeQuery,
};

struct EFF3DImageInfo
{
    effUINT width;
    effUINT height;
    effUINT depth;
    effUINT mipLevels;
    EFF3DTextureFormat format;
    EFF3DResourceType type;

    effUINT filter;
    effUINT mipFilter;

 
    EFF3DMULTISAMPLE_TYPE multiSample;
    effUINT multisampleQuality;
 
    //effUINT textureHandle;
    effUINT textureLevel;

    effHANDLE sharedHandle;

    effUINT flags;

    EFF3DImageInfo()
    {
        width = 0;
        height = 0;
        depth = 0;
        mipLevels = 0;
        format = EFF3D_TEXTURE_FORMAT_UNKNOWN;
        type = EFF3DResourceType_Texture2D;

        filter = 0;
        mipFilter = 0;

        multiSample = EFF3DMULTISAMPLE_NONE;
        multisampleQuality = 0;
        //textureHandle = EFF_INVALID_HANDLE;
        textureLevel = 0;

        sharedHandle = 0;
        flags = 0;
    }
};

/// Color RGB/alpha/depth write. When it's not specified write will be disabled.
#define EFF3D_STATE_WRITE_R                 UINT64_C(0x0000000000000001) //!< Enable R write.
#define EFF3D_STATE_WRITE_G                 UINT64_C(0x0000000000000002) //!< Enable G write.
#define EFF3D_STATE_WRITE_B                 UINT64_C(0x0000000000000004) //!< Enable B write.
#define EFF3D_STATE_WRITE_A                 UINT64_C(0x0000000000000008) //!< Enable alpha write.
#define EFF3D_STATE_WRITE_Z                 UINT64_C(0x0000004000000000) //!< Enable depth write.

/// Enable RGB write.
#define EFF3D_STATE_WRITE_RGB (0  \
			| EFF3D_STATE_WRITE_R \
			| EFF3D_STATE_WRITE_G \
			| EFF3D_STATE_WRITE_B \
			)

/// Write all channels mask.
#define EFF3D_STATE_WRITE_MASK (0   \
			| EFF3D_STATE_WRITE_RGB \
			| EFF3D_STATE_WRITE_A   \
			| EFF3D_STATE_WRITE_Z   \
			)

/// Depth test state. When `EFF3D_STATE_DEPTH_` is not specified depth test will be disabled.
#define EFF3D_STATE_DEPTH_TEST_LESS         UINT64_C(0x0000000000000010) //!< Enable depth test, less.
#define EFF3D_STATE_DEPTH_TEST_LEQUAL       UINT64_C(0x0000000000000020) //!< Enable depth test, less or equal.
#define EFF3D_STATE_DEPTH_TEST_EQUAL        UINT64_C(0x0000000000000030) //!< Enable depth test, equal.
#define EFF3D_STATE_DEPTH_TEST_GEQUAL       UINT64_C(0x0000000000000040) //!< Enable depth test, greater or equal.
#define EFF3D_STATE_DEPTH_TEST_GREATER      UINT64_C(0x0000000000000050) //!< Enable depth test, greater.
#define EFF3D_STATE_DEPTH_TEST_NOTEQUAL     UINT64_C(0x0000000000000060) //!< Enable depth test, not equal.
#define EFF3D_STATE_DEPTH_TEST_NEVER        UINT64_C(0x0000000000000070) //!< Enable depth test, never.
#define EFF3D_STATE_DEPTH_TEST_ALWAYS       UINT64_C(0x0000000000000080) //!< Enable depth test, always.
#define EFF3D_STATE_DEPTH_TEST_SHIFT        4                            //!< Depth test state bit shift.
#define EFF3D_STATE_DEPTH_TEST_MASK         UINT64_C(0x00000000000000f0) //!< Depth test state bit mask.

/// Use EFF3D_STATE_BLEND_FUNC(_src, _dst) or EFF3D_STATE_BLEND_FUNC_SEPARATE(_srcRGB, _dstRGB, _srcA, _dstA)
/// helper macros.
#define EFF3D_STATE_BLEND_ZERO              UINT64_C(0x0000000000001000) //!< 0, 0, 0, 0
#define EFF3D_STATE_BLEND_ONE               UINT64_C(0x0000000000002000) //!< 1, 1, 1, 1
#define EFF3D_STATE_BLEND_SRC_COLOR         UINT64_C(0x0000000000003000) //!< Rs, Gs, Bs, As
#define EFF3D_STATE_BLEND_INV_SRC_COLOR     UINT64_C(0x0000000000004000) //!< 1-Rs, 1-Gs, 1-Bs, 1-As
#define EFF3D_STATE_BLEND_SRC_ALPHA         UINT64_C(0x0000000000005000) //!< As, As, As, As
#define EFF3D_STATE_BLEND_INV_SRC_ALPHA     UINT64_C(0x0000000000006000) //!< 1-As, 1-As, 1-As, 1-As
#define EFF3D_STATE_BLEND_DST_ALPHA         UINT64_C(0x0000000000007000) //!< Ad, Ad, Ad, Ad
#define EFF3D_STATE_BLEND_INV_DST_ALPHA     UINT64_C(0x0000000000008000) //!< 1-Ad, 1-Ad, 1-Ad ,1-Ad
#define EFF3D_STATE_BLEND_DST_COLOR         UINT64_C(0x0000000000009000) //!< Rd, Gd, Bd, Ad
#define EFF3D_STATE_BLEND_INV_DST_COLOR     UINT64_C(0x000000000000a000) //!< 1-Rd, 1-Gd, 1-Bd, 1-Ad
#define EFF3D_STATE_BLEND_SRC_ALPHA_SAT     UINT64_C(0x000000000000b000) //!< f, f, f, 1; f = min(As, 1-Ad)
#define EFF3D_STATE_BLEND_FACTOR            UINT64_C(0x000000000000c000) //!< Blend factor
#define EFF3D_STATE_BLEND_INV_FACTOR        UINT64_C(0x000000000000d000) //!< 1-Blend factor
#define EFF3D_STATE_BLEND_SHIFT             12                           //!< Blend state bit shift.
#define EFF3D_STATE_BLEND_MASK              UINT64_C(0x000000000ffff000) //!< Blend state bit mask.

/// Use EFF3D_STATE_BLEND_EQUATION(_equation) or EFF3D_STATE_BLEND_EQUATION_SEPARATE(_equationRGB, _equationA)
/// helper macros.
#define EFF3D_STATE_BLEND_EQUATION_ADD      UINT64_C(0x0000000000000000) //!< Blend add: src + dst.
#define EFF3D_STATE_BLEND_EQUATION_SUB      UINT64_C(0x0000000010000000) //!< Blend subtract: src - dst.
#define EFF3D_STATE_BLEND_EQUATION_REVSUB   UINT64_C(0x0000000020000000) //!< Blend reverse subtract: dst - src.
#define EFF3D_STATE_BLEND_EQUATION_MIN      UINT64_C(0x0000000030000000) //!< Blend min: min(src, dst).
#define EFF3D_STATE_BLEND_EQUATION_MAX      UINT64_C(0x0000000040000000) //!< Blend max: max(src, dst).
#define EFF3D_STATE_BLEND_EQUATION_SHIFT    28                           //!< Blend equation bit shift.
#define EFF3D_STATE_BLEND_EQUATION_MASK     UINT64_C(0x00000003f0000000) //!< Blend equation bit mask.

#define EFF3D_STATE_BLEND_INDEPENDENT       UINT64_C(0x0000000400000000) //!< Enable blend independent.
#define EFF3D_STATE_BLEND_ALPHA_TO_COVERAGE UINT64_C(0x0000000800000000) //!< Enable alpha to coverage.

/// Cull state. When `EFF3D_STATE_CULL_*` is not specified culling will be disabled.
#define EFF3D_STATE_CULL_CW                 UINT64_C(0x0000001000000000) //!< Cull clockwise triangles.
#define EFF3D_STATE_CULL_CCW                UINT64_C(0x0000002000000000) //!< Cull counter-clockwise triangles.
#define EFF3D_STATE_CULL_SHIFT              36                           //!< Culling mode bit shift.
#define EFF3D_STATE_CULL_MASK               UINT64_C(0x0000003000000000) //!< Culling mode bit mask.

/// See EFF3D_STATE_ALPHA_REF(_ref) helper macro.
#define EFF3D_STATE_ALPHA_REF_SHIFT         40                           //!< Alpha reference bit shift.
#define EFF3D_STATE_ALPHA_REF_MASK          UINT64_C(0x0000ff0000000000) //!< Alpha reference bit mask.

#define EFF3D_STATE_PT_TRISTRIP             UINT64_C(0x0001000000000000) //!< Tristrip.
#define EFF3D_STATE_PT_LINES                UINT64_C(0x0002000000000000) //!< Lines.
#define EFF3D_STATE_PT_LINESTRIP            UINT64_C(0x0003000000000000) //!< Line strip.
#define EFF3D_STATE_PT_POINTS               UINT64_C(0x0004000000000000) //!< Points.
#define EFF3D_STATE_PT_SHIFT                48                           //!< Primitive type bit shift.
#define EFF3D_STATE_PT_MASK                 UINT64_C(0x0007000000000000) //!< Primitive type bit mask.

/// See EFF3D_STATE_POINT_SIZE(_size) helper macro.
#define EFF3D_STATE_POINT_SIZE_SHIFT        52                           //!< Point size bit shift.
#define EFF3D_STATE_POINT_SIZE_MASK         UINT64_C(0x00f0000000000000) //!< Point size bit mask.

/// Enable MSAA write when writing into MSAA frame buffer.
/// This flag is ignored when not writing into MSAA frame buffer.
#define EFF3D_STATE_MSAA                    UINT64_C(0x0100000000000000) //!< Enable MSAA rasterization.
#define EFF3D_STATE_LINEAA                  UINT64_C(0x0200000000000000) //!< Enable line AA rasterization.
#define EFF3D_STATE_CONSERVATIVE_RASTER     UINT64_C(0x0400000000000000) //!< Enable conservative rasterization.

/// Do not use!
#define EFF3D_STATE_RESERVED_SHIFT          61                           //!< Internal bits shift.
#define EFF3D_STATE_RESERVED_MASK           UINT64_C(0xe000000000000000) //!< Internal bits mask.

#define EFF3D_STATE_NONE                    UINT64_C(0x0000000000000000) //!< No state.
#define EFF3D_STATE_MASK                    UINT64_C(0xffffffffffffffff) //!< State mask.

/// Default state is write to RGB, alpha, and depth with depth test less enabled, with clockwise
/// culling and MSAA (when writing into MSAA frame buffer, otherwise this flag is ignored).
#define EFF3D_STATE_DEFAULT (0            \
			| EFF3D_STATE_WRITE_RGB       \
			| EFF3D_STATE_WRITE_A         \
			| EFF3D_STATE_WRITE_Z         \
			| EFF3D_STATE_DEPTH_TEST_LESS \
			| EFF3D_STATE_CULL_CW         \
			| EFF3D_STATE_MSAA            \
			)

/// Alpha reference value.
#define EFF3D_STATE_ALPHA_REF(_ref)   ( ( (uint64_t)(_ref )<<EFF3D_STATE_ALPHA_REF_SHIFT )&EFF3D_STATE_ALPHA_REF_MASK)

/// Point size value.
#define EFF3D_STATE_POINT_SIZE(_size) ( ( (uint64_t)(_size)<<EFF3D_STATE_POINT_SIZE_SHIFT)&EFF3D_STATE_POINT_SIZE_MASK)

/// Blend function separate.
#define EFF3D_STATE_BLEND_FUNC_SEPARATE(_srcRGB, _dstRGB, _srcA, _dstA) (UINT64_C(0) \
			| ( ( (uint64_t)(_srcRGB)|( (uint64_t)(_dstRGB)<<4) )   )               \
			| ( ( (uint64_t)(_srcA  )|( (uint64_t)(_dstA  )<<4) )<<8)               \
			)

/// Blend equation separate.
#define EFF3D_STATE_BLEND_EQUATION_SEPARATE(_equationRGB, _equationA) ( (uint64_t)(_equationRGB)|( (uint64_t)(_equationA)<<3) )

/// Blend function.
#define EFF3D_STATE_BLEND_FUNC(_src, _dst)    EFF3D_STATE_BLEND_FUNC_SEPARATE(_src, _dst, _src, _dst)

/// Blend equation.
#define EFF3D_STATE_BLEND_EQUATION(_equation) EFF3D_STATE_BLEND_EQUATION_SEPARATE(_equation, _equation)

/// Utility predefined blend modes.

/// Additive blending.
#define EFF3D_STATE_BLEND_ADD (0                                         \
	| EFF3D_STATE_BLEND_FUNC(EFF3D_STATE_BLEND_ONE, EFF3D_STATE_BLEND_ONE) \
	)

/// Alpha blend.
#define EFF3D_STATE_BLEND_ALPHA (0                                                       \
	| EFF3D_STATE_BLEND_FUNC(EFF3D_STATE_BLEND_SRC_ALPHA, EFF3D_STATE_BLEND_INV_SRC_ALPHA) \
	)

/// Selects darker color of blend.
#define EFF3D_STATE_BLEND_DARKEN (0                                      \
	| EFF3D_STATE_BLEND_FUNC(EFF3D_STATE_BLEND_ONE, EFF3D_STATE_BLEND_ONE) \
	| EFF3D_STATE_BLEND_EQUATION(EFF3D_STATE_BLEND_EQUATION_MIN)          \
	)

/// Selects lighter color of blend.
#define EFF3D_STATE_BLEND_LIGHTEN (0                                     \
	| EFF3D_STATE_BLEND_FUNC(EFF3D_STATE_BLEND_ONE, EFF3D_STATE_BLEND_ONE) \
	| EFF3D_STATE_BLEND_EQUATION(EFF3D_STATE_BLEND_EQUATION_MAX)          \
	)

/// Multiplies colors.
#define EFF3D_STATE_BLEND_MULTIPLY (0                                           \
	| EFF3D_STATE_BLEND_FUNC(EFF3D_STATE_BLEND_DST_COLOR, EFF3D_STATE_BLEND_ZERO) \
	)

/// Opaque pixels will cover the pixels directly below them without any math or algorithm applied to them.
#define EFF3D_STATE_BLEND_NORMAL (0                                                \
	| EFF3D_STATE_BLEND_FUNC(EFF3D_STATE_BLEND_ONE, EFF3D_STATE_BLEND_INV_SRC_ALPHA) \
	)

/// Multiplies the inverse of the blend and base colors.
#define EFF3D_STATE_BLEND_SCREEN (0                                                \
	| EFF3D_STATE_BLEND_FUNC(EFF3D_STATE_BLEND_ONE, EFF3D_STATE_BLEND_INV_SRC_COLOR) \
	)

/// Decreases the brightness of the base color based on the value of the blend color.
#define EFF3D_STATE_BLEND_LINEAR_BURN (0                                                 \
	| EFF3D_STATE_BLEND_FUNC(EFF3D_STATE_BLEND_DST_COLOR, EFF3D_STATE_BLEND_INV_DST_COLOR) \
	| EFF3D_STATE_BLEND_EQUATION(EFF3D_STATE_BLEND_EQUATION_SUB)                          \
	)

///
#define EFF3D_STATE_BLEND_FUNC_RT_x(_src, _dst) (0               \
			| ( (uint32_t)( (_src)>>EFF3D_STATE_BLEND_SHIFT)       \
			| ( (uint32_t)( (_dst)>>EFF3D_STATE_BLEND_SHIFT)<<4) ) \
			)

#define EFF3D_STATE_BLEND_FUNC_RT_xE(_src, _dst, _equation) (0               \
			| EFF3D_STATE_BLEND_FUNC_RT_x(_src, _dst)                        \
			| ( (uint32_t)( (_equation)>>EFF3D_STATE_BLEND_EQUATION_SHIFT)<<8) \
			)

#define EFF3D_STATE_BLEND_FUNC_RT_1(_src, _dst)  (EFF3D_STATE_BLEND_FUNC_RT_x(_src, _dst)<< 0)
#define EFF3D_STATE_BLEND_FUNC_RT_2(_src, _dst)  (EFF3D_STATE_BLEND_FUNC_RT_x(_src, _dst)<<11)
#define EFF3D_STATE_BLEND_FUNC_RT_3(_src, _dst)  (EFF3D_STATE_BLEND_FUNC_RT_x(_src, _dst)<<22)

#define EFF3D_STATE_BLEND_FUNC_RT_1E(_src, _dst, _equation) (EFF3D_STATE_BLEND_FUNC_RT_xE(_src, _dst, _equation)<< 0)
#define EFF3D_STATE_BLEND_FUNC_RT_2E(_src, _dst, _equation) (EFF3D_STATE_BLEND_FUNC_RT_xE(_src, _dst, _equation)<<11)
#define EFF3D_STATE_BLEND_FUNC_RT_3E(_src, _dst, _equation) (EFF3D_STATE_BLEND_FUNC_RT_xE(_src, _dst, _equation)<<22)

///
#define EFF3D_STENCIL_FUNC_REF_SHIFT      0                    //!<
#define EFF3D_STENCIL_FUNC_REF_MASK       UINT32_C(0x000000ff) //!<
#define EFF3D_STENCIL_FUNC_RMASK_SHIFT    8                    //!<
#define EFF3D_STENCIL_FUNC_RMASK_MASK     UINT32_C(0x0000ff00) //!<

#define EFF3D_STENCIL_TEST_LESS           UINT32_C(0x00010000) //!< Enable stencil test, less.
#define EFF3D_STENCIL_TEST_LEQUAL         UINT32_C(0x00020000) //!< Enable stencil test, less or equal.
#define EFF3D_STENCIL_TEST_EQUAL          UINT32_C(0x00030000) //!< Enable stencil test, equal.
#define EFF3D_STENCIL_TEST_GEQUAL         UINT32_C(0x00040000) //!< Enable stencil test, greater or equal.
#define EFF3D_STENCIL_TEST_GREATER        UINT32_C(0x00050000) //!< Enable stencil test, greater.
#define EFF3D_STENCIL_TEST_NOTEQUAL       UINT32_C(0x00060000) //!< Enable stencil test, not equal.
#define EFF3D_STENCIL_TEST_NEVER          UINT32_C(0x00070000) //!< Enable stencil test, never.
#define EFF3D_STENCIL_TEST_ALWAYS         UINT32_C(0x00080000) //!< Enable stencil test, always.
#define EFF3D_STENCIL_TEST_SHIFT          16                   //!< Stencil test bit shift.
#define EFF3D_STENCIL_TEST_MASK           UINT32_C(0x000f0000) //!< Stencil test bit mask.

#define EFF3D_STENCIL_OP_FAIL_S_ZERO      UINT32_C(0x00000000) //!< Zero.
#define EFF3D_STENCIL_OP_FAIL_S_KEEP      UINT32_C(0x00100000) //!< Keep.
#define EFF3D_STENCIL_OP_FAIL_S_REPLACE   UINT32_C(0x00200000) //!< Replace.
#define EFF3D_STENCIL_OP_FAIL_S_INCR      UINT32_C(0x00300000) //!< Increment and wrap.
#define EFF3D_STENCIL_OP_FAIL_S_INCRSAT   UINT32_C(0x00400000) //!< Increment and clamp.
#define EFF3D_STENCIL_OP_FAIL_S_DECR      UINT32_C(0x00500000) //!< Decrement and wrap.
#define EFF3D_STENCIL_OP_FAIL_S_DECRSAT   UINT32_C(0x00600000) //!< Decrement and clamp.
#define EFF3D_STENCIL_OP_FAIL_S_INVERT    UINT32_C(0x00700000) //!< Invert.
#define EFF3D_STENCIL_OP_FAIL_S_SHIFT     20                   //!< Stencil operation fail bit shift.
#define EFF3D_STENCIL_OP_FAIL_S_MASK      UINT32_C(0x00f00000) //!< Stencil operation fail bit mask.

#define EFF3D_STENCIL_OP_FAIL_Z_ZERO      UINT32_C(0x00000000) //!< Zero.
#define EFF3D_STENCIL_OP_FAIL_Z_KEEP      UINT32_C(0x01000000) //!< Keep.
#define EFF3D_STENCIL_OP_FAIL_Z_REPLACE   UINT32_C(0x02000000) //!< Replace.
#define EFF3D_STENCIL_OP_FAIL_Z_INCR      UINT32_C(0x03000000) //!< Increment and wrap.
#define EFF3D_STENCIL_OP_FAIL_Z_INCRSAT   UINT32_C(0x04000000) //!< Increment and clamp.
#define EFF3D_STENCIL_OP_FAIL_Z_DECR      UINT32_C(0x05000000) //!< Decrement and wrap.
#define EFF3D_STENCIL_OP_FAIL_Z_DECRSAT   UINT32_C(0x06000000) //!< Decrement and clamp.
#define EFF3D_STENCIL_OP_FAIL_Z_INVERT    UINT32_C(0x07000000) //!< Invert.
#define EFF3D_STENCIL_OP_FAIL_Z_SHIFT     24                   //!< Stencil operation depth fail bit shift
#define EFF3D_STENCIL_OP_FAIL_Z_MASK      UINT32_C(0x0f000000) //!< Stencil operation depth fail bit mask.

#define EFF3D_STENCIL_OP_PASS_Z_ZERO      UINT32_C(0x00000000) //!< Zero.
#define EFF3D_STENCIL_OP_PASS_Z_KEEP      UINT32_C(0x10000000) //!< Keep.
#define EFF3D_STENCIL_OP_PASS_Z_REPLACE   UINT32_C(0x20000000) //!< Replace.
#define EFF3D_STENCIL_OP_PASS_Z_INCR      UINT32_C(0x30000000) //!< Increment and wrap.
#define EFF3D_STENCIL_OP_PASS_Z_INCRSAT   UINT32_C(0x40000000) //!< Increment and clamp.
#define EFF3D_STENCIL_OP_PASS_Z_DECR      UINT32_C(0x50000000) //!< Decrement and wrap.
#define EFF3D_STENCIL_OP_PASS_Z_DECRSAT   UINT32_C(0x60000000) //!< Decrement and clamp.
#define EFF3D_STENCIL_OP_PASS_Z_INVERT    UINT32_C(0x70000000) //!< Invert.
#define EFF3D_STENCIL_OP_PASS_Z_SHIFT     28                   //!< Stencil operation depth pass bit shift
#define EFF3D_STENCIL_OP_PASS_Z_MASK      UINT32_C(0xf0000000) //!< Stencil operation depth pass bit mask.

#define EFF3D_STENCIL_NONE                UINT32_C(0x00000000) //!<
#define EFF3D_STENCIL_MASK                UINT32_C(0xffffffff) //!<
#define EFF3D_STENCIL_DEFAULT             UINT32_C(0x00000000) //!<

/// Set stencil ref value.
#define EFF3D_STENCIL_FUNC_REF(_ref) ( ((uint32_t)(_ref)<<EFF3D_STENCIL_FUNC_REF_SHIFT)&EFF3D_STENCIL_FUNC_REF_MASK)

/// Set stencil rmask value.
#define EFF3D_STENCIL_FUNC_RMASK(_mask) ( ((uint32_t)(_mask)<<EFF3D_STENCIL_FUNC_RMASK_SHIFT)&EFF3D_STENCIL_FUNC_RMASK_MASK)

///
#define EFF3D_CLEAR_NONE                  UINT16_C(0x0000) //!< No clear flags.
#define EFF3D_CLEAR_COLOR                 UINT16_C(0x0001) //!< Clear color.
#define EFF3D_CLEAR_DEPTH                 UINT16_C(0x0002) //!< Clear depth.
#define EFF3D_CLEAR_STENCIL               UINT16_C(0x0004) //!< Clear stencil.
#define EFF3D_CLEAR_DISCARD_COLOR_0       UINT16_C(0x0008) //!< Discard frame buffer attachment 0.
#define EFF3D_CLEAR_DISCARD_COLOR_1       UINT16_C(0x0010) //!< Discard frame buffer attachment 1.
#define EFF3D_CLEAR_DISCARD_COLOR_2       UINT16_C(0x0020) //!< Discard frame buffer attachment 2.
#define EFF3D_CLEAR_DISCARD_COLOR_3       UINT16_C(0x0040) //!< Discard frame buffer attachment 3.
#define EFF3D_CLEAR_DISCARD_COLOR_4       UINT16_C(0x0080) //!< Discard frame buffer attachment 4.
#define EFF3D_CLEAR_DISCARD_COLOR_5       UINT16_C(0x0100) //!< Discard frame buffer attachment 5.
#define EFF3D_CLEAR_DISCARD_COLOR_6       UINT16_C(0x0200) //!< Discard frame buffer attachment 6.
#define EFF3D_CLEAR_DISCARD_COLOR_7       UINT16_C(0x0400) //!< Discard frame buffer attachment 7.
#define EFF3D_CLEAR_DISCARD_DEPTH         UINT16_C(0x0800) //!< Discard frame buffer depth attachment.
#define EFF3D_CLEAR_DISCARD_STENCIL       UINT16_C(0x1000) //!< Discard frame buffer stencil attachment.

#define EFF3D_CLEAR_DISCARD_COLOR_MASK (0    \
			| EFF3D_CLEAR_DISCARD_COLOR_0    \
			| EFF3D_CLEAR_DISCARD_COLOR_1    \
			| EFF3D_CLEAR_DISCARD_COLOR_2    \
			| EFF3D_CLEAR_DISCARD_COLOR_3    \
			| EFF3D_CLEAR_DISCARD_COLOR_4    \
			| EFF3D_CLEAR_DISCARD_COLOR_5    \
			| EFF3D_CLEAR_DISCARD_COLOR_6    \
			| EFF3D_CLEAR_DISCARD_COLOR_7    \
			)
#define EFF3D_CLEAR_DISCARD_MASK (0          \
			| EFF3D_CLEAR_DISCARD_COLOR_MASK \
			| EFF3D_CLEAR_DISCARD_DEPTH      \
			| EFF3D_CLEAR_DISCARD_STENCIL    \
			)

///
#define EFF3D_DEBUG_NONE                  UINT32_C(0x00000000) //!< No debug.
#define EFF3D_DEBUG_WIREFRAME             UINT32_C(0x00000001) //!< Enable wireframe for all primitives.
#define EFF3D_DEBUG_IFH                   UINT32_C(0x00000002) //!< Enable infinitely fast hardware test. No draw calls will be submitted to driver. It¡¯s useful when profiling to quickly assess bottleneck between CPU and GPU.
#define EFF3D_DEBUG_STATS                 UINT32_C(0x00000004) //!< Enable statistics display.
#define EFF3D_DEBUG_TEXT                  UINT32_C(0x00000008) //!< Enable debug text display.
#define EFF3D_DEBUG_PROFILER              UINT32_C(0x00000010) //!< Enable profiler.

///
#define EFF3D_BUFFER_NONE                 UINT16_C(0x0000) //!<

#define EFF3D_BUFFER_COMPUTE_FORMAT_8x1   UINT16_C(0x0001) //!< 1 8-bit value
#define EFF3D_BUFFER_COMPUTE_FORMAT_8x2   UINT16_C(0x0002) //!< 2 8-bit values
#define EFF3D_BUFFER_COMPUTE_FORMAT_8x4   UINT16_C(0x0003) //!< 4 8-bit values
#define EFF3D_BUFFER_COMPUTE_FORMAT_16x1  UINT16_C(0x0004) //!< 1 16-bit value
#define EFF3D_BUFFER_COMPUTE_FORMAT_16x2  UINT16_C(0x0005) //!< 2 16-bit values
#define EFF3D_BUFFER_COMPUTE_FORMAT_16x4  UINT16_C(0x0006) //!< 4 16-bit values
#define EFF3D_BUFFER_COMPUTE_FORMAT_32x1  UINT16_C(0x0007) //!< 1 32-bit value
#define EFF3D_BUFFER_COMPUTE_FORMAT_32x2  UINT16_C(0x0008) //!< 2 32-bit values
#define EFF3D_BUFFER_COMPUTE_FORMAT_32x4  UINT16_C(0x0009) //!< 4 32-bit values
#define EFF3D_BUFFER_COMPUTE_FORMAT_SHIFT 0                //!<
#define EFF3D_BUFFER_COMPUTE_FORMAT_MASK  UINT16_C(0x000f) //!<

#define EFF3D_BUFFER_COMPUTE_TYPE_INT     UINT16_C(0x0010) //!< Type `int`.
#define EFF3D_BUFFER_COMPUTE_TYPE_UINT    UINT16_C(0x0020) //!< Type `uint`.
#define EFF3D_BUFFER_COMPUTE_TYPE_FLOAT   UINT16_C(0x0030) //!< Type `float`.
#define EFF3D_BUFFER_COMPUTE_TYPE_SHIFT   4                //!<
#define EFF3D_BUFFER_COMPUTE_TYPE_MASK    UINT16_C(0x0030) //!<

#define EFF3D_BUFFER_COMPUTE_READ         UINT16_C(0x0100) //!< Buffer will be read by shader.
#define EFF3D_BUFFER_COMPUTE_WRITE        UINT16_C(0x0200) //!< Buffer will be used for writing.
#define EFF3D_BUFFER_DRAW_INDIRECT        UINT16_C(0x0400) //!< Buffer will be used for storing draw indirect commands.
#define EFF3D_BUFFER_ALLOW_RESIZE         UINT16_C(0x0800) //!< Allow dynamic index/vertex buffer resize during update.
#define EFF3D_BUFFER_INDEX32              UINT16_C(0x1000) //!< Index buffer contains 32-bit indices.

#define EFF3D_BUFFER_COMPUTE_READ_WRITE (0 \
			| EFF3D_BUFFER_COMPUTE_READ    \
			| EFF3D_BUFFER_COMPUTE_WRITE   \
			)

///
#define EFF3D_TEXTURE_NONE                UINT32_C(0x00000000) //!<
#define EFF3D_TEXTURE_U_MIRROR            UINT32_C(0x00000001) //!< Wrap U mode: Mirror
#define EFF3D_TEXTURE_U_CLAMP             UINT32_C(0x00000002) //!< Wrap U mode: Clamp
#define EFF3D_TEXTURE_U_BORDER            UINT32_C(0x00000003) //!< Wrap U mode: Border
#define EFF3D_TEXTURE_U_SHIFT             0                    //!<
#define EFF3D_TEXTURE_U_MASK              UINT32_C(0x00000003) //!<
#define EFF3D_TEXTURE_V_MIRROR            UINT32_C(0x00000004) //!< Wrap V mode: Mirror
#define EFF3D_TEXTURE_V_CLAMP             UINT32_C(0x00000008) //!< Wrap V mode: Clamp
#define EFF3D_TEXTURE_V_BORDER            UINT32_C(0x0000000c) //!< Wrap V mode: Border
#define EFF3D_TEXTURE_V_SHIFT             2                    //!<
#define EFF3D_TEXTURE_V_MASK              UINT32_C(0x0000000c) //!<
#define EFF3D_TEXTURE_W_MIRROR            UINT32_C(0x00000010) //!< Wrap W mode: Mirror
#define EFF3D_TEXTURE_W_CLAMP             UINT32_C(0x00000020) //!< Wrap W mode: Clamp
#define EFF3D_TEXTURE_W_BORDER            UINT32_C(0x00000030) //!< Wrap W mode: Border
#define EFF3D_TEXTURE_W_SHIFT             4                    //!<
#define EFF3D_TEXTURE_W_MASK              UINT32_C(0x00000030) //!<
#define EFF3D_TEXTURE_MIN_POINT           UINT32_C(0x00000040) //!< Min sampling mode: Point
#define EFF3D_TEXTURE_MIN_ANISOTROPIC     UINT32_C(0x00000080) //!< Min sampling mode: Anisotropic
#define EFF3D_TEXTURE_MIN_SHIFT           6                    //!<
#define EFF3D_TEXTURE_MIN_MASK            UINT32_C(0x000000c0) //!<
#define EFF3D_TEXTURE_MAG_POINT           UINT32_C(0x00000100) //!< Mag sampling mode: Point
#define EFF3D_TEXTURE_MAG_ANISOTROPIC     UINT32_C(0x00000200) //!< Mag sampling mode: Anisotropic
#define EFF3D_TEXTURE_MAG_SHIFT           8                    //!<
#define EFF3D_TEXTURE_MAG_MASK            UINT32_C(0x00000300) //!<
#define EFF3D_TEXTURE_MIP_POINT           UINT32_C(0x00000400) //!< Mip sampling mode: Point
#define EFF3D_TEXTURE_MIP_SHIFT           10                   //!<
#define EFF3D_TEXTURE_MIP_MASK            UINT32_C(0x00000400) //!<
#define EFF3D_TEXTURE_MSAA_SAMPLE         UINT32_C(0x00000800) //!< Texture will be used for MSAA sampling.
#define EFF3D_TEXTURE_RT                  UINT32_C(0x00001000) //!<
#define EFF3D_TEXTURE_RT_MSAA_X2          UINT32_C(0x00002000) //!< Render target MSAAx2 mode.
#define EFF3D_TEXTURE_RT_MSAA_X4          UINT32_C(0x00003000) //!< Render target MSAAx4 mode.
#define EFF3D_TEXTURE_RT_MSAA_X8          UINT32_C(0x00004000) //!< Render target MSAAx8 mode.
#define EFF3D_TEXTURE_RT_MSAA_X16         UINT32_C(0x00005000) //!< Render target MSAAx16 mode.
#define EFF3D_TEXTURE_RT_MSAA_SHIFT       12                   //!<
#define EFF3D_TEXTURE_RT_MSAA_MASK        UINT32_C(0x00007000) //!<
#define EFF3D_TEXTURE_RT_WRITE_ONLY       UINT32_C(0x00008000) //!< Render target will be used for writing only.
#define EFF3D_TEXTURE_RT_MASK             UINT32_C(0x0000f000) //!<
#define EFF3D_TEXTURE_COMPARE_LESS        UINT32_C(0x00010000) //!< Compare when sampling depth texture: less.
#define EFF3D_TEXTURE_COMPARE_LEQUAL      UINT32_C(0x00020000) //!< Compare when sampling depth texture: less or equal.
#define EFF3D_TEXTURE_COMPARE_EQUAL       UINT32_C(0x00030000) //!< Compare when sampling depth texture: equal.
#define EFF3D_TEXTURE_COMPARE_GEQUAL      UINT32_C(0x00040000) //!< Compare when sampling depth texture: greater or equal.
#define EFF3D_TEXTURE_COMPARE_GREATER     UINT32_C(0x00050000) //!< Compare when sampling depth texture: greater.
#define EFF3D_TEXTURE_COMPARE_NOTEQUAL    UINT32_C(0x00060000) //!< Compare when sampling depth texture: not equal.
#define EFF3D_TEXTURE_COMPARE_NEVER       UINT32_C(0x00070000) //!< Compare when sampling depth texture: never.
#define EFF3D_TEXTURE_COMPARE_ALWAYS      UINT32_C(0x00080000) //!< Compare when sampling depth texture: always.
#define EFF3D_TEXTURE_COMPARE_SHIFT       16                   //!<
#define EFF3D_TEXTURE_COMPARE_MASK        UINT32_C(0x000f0000) //!<
#define EFF3D_TEXTURE_COMPUTE_WRITE       UINT32_C(0x00100000) //!< Texture will be used for compute write.
#define EFF3D_TEXTURE_SRGB                UINT32_C(0x00200000) //!< Sample texture as sRGB.
#define EFF3D_TEXTURE_BLIT_DST            UINT32_C(0x00400000) //!< Texture will be used as blit destination.
#define EFF3D_TEXTURE_READ_BACK           UINT32_C(0x00800000) //!< Texture will be used for read back from GPU.
#define EFF3D_TEXTURE_BORDER_COLOR_SHIFT  24                   //!<
#define EFF3D_TEXTURE_BORDER_COLOR_MASK   UINT32_C(0x0f000000) //!<
#define EFF3D_TEXTURE_RESERVED_SHIFT      28                   //!<
#define EFF3D_TEXTURE_RESERVED_MASK       UINT32_C(0xf0000000) //!<

#define EFF3D_TEXTURE_BORDER_COLOR(_index) ( (_index << EFF3D_TEXTURE_BORDER_COLOR_SHIFT) & EFF3D_TEXTURE_BORDER_COLOR_MASK)

#define EFF3D_TEXTURE_SAMPLER_BITS_MASK (0 \
			| EFF3D_TEXTURE_U_MASK         \
			| EFF3D_TEXTURE_V_MASK         \
			| EFF3D_TEXTURE_W_MASK         \
			| EFF3D_TEXTURE_MIN_MASK       \
			| EFF3D_TEXTURE_MAG_MASK       \
			| EFF3D_TEXTURE_MIP_MASK       \
			| EFF3D_TEXTURE_COMPARE_MASK   \
			)

///
#define EFF3D_RESET_NONE                  UINT32_C(0x00000000) //!< No reset flags.
#define EFF3D_RESET_FULLSCREEN            UINT32_C(0x00000001) //!< Not supported yet.
#define EFF3D_RESET_FULLSCREEN_SHIFT      0                    //!< Fullscreen bit shift.
#define EFF3D_RESET_FULLSCREEN_MASK       UINT32_C(0x00000001) //!< Fullscreen bit mask.
#define EFF3D_RESET_MSAA_X2               UINT32_C(0x00000010) //!< Enable 2x MSAA.
#define EFF3D_RESET_MSAA_X4               UINT32_C(0x00000020) //!< Enable 4x MSAA.
#define EFF3D_RESET_MSAA_X8               UINT32_C(0x00000030) //!< Enable 8x MSAA.
#define EFF3D_RESET_MSAA_X16              UINT32_C(0x00000040) //!< Enable 16x MSAA.
#define EFF3D_RESET_MSAA_SHIFT            4                    //!< MSAA mode bit shift.
#define EFF3D_RESET_MSAA_MASK             UINT32_C(0x00000070) //!< MSAA mode bit mask.
#define EFF3D_RESET_VSYNC                 UINT32_C(0x00000080) //!< Enable V-Sync.
#define EFF3D_RESET_MAXANISOTROPY         UINT32_C(0x00000100) //!< Turn on/off max anisotropy.
#define EFF3D_RESET_CAPTURE               UINT32_C(0x00000200) //!< Begin screen capture.
#define EFF3D_RESET_HMD                   UINT32_C(0x00000400) //!< HMD stereo rendering.
#define EFF3D_RESET_HMD_DEBUG             UINT32_C(0x00000800) //!< HMD stereo rendering debug mode.
#define EFF3D_RESET_HMD_RECENTER          UINT32_C(0x00001000) //!< HMD calibration.
#define EFF3D_RESET_FLUSH_AFTER_RENDER    UINT32_C(0x00002000) //!< Flush rendering after submitting to GPU.
#define EFF3D_RESET_FLIP_AFTER_RENDER     UINT32_C(0x00004000) //!< This flag  specifies where flip occurs. Default behavior is that flip occurs before rendering new frame. This flag only has effect when `EFF3D_CONFIG_MULTITHREADED=0`.
#define EFF3D_RESET_SRGB_BACKBUFFER       UINT32_C(0x00008000) //!< Enable sRGB backbuffer.
#define EFF3D_RESET_HIDPI                 UINT32_C(0x00010000) //!< Enable HiDPI rendering.
#define EFF3D_RESET_DEPTH_CLAMP           UINT32_C(0x00020000) //!< Enable depth clamp.
#define EFF3D_RESET_SUSPEND               UINT32_C(0x00040000) //!< Suspend rendering.

#define EFF3D_RESET_RESERVED_SHIFT        31                   //!< Internal bits shift.
#define EFF3D_RESET_RESERVED_MASK         UINT32_C(0x80000000) //!< Internal bits mask.

///
#define EFF3D_CAPS_ALPHA_TO_COVERAGE      UINT64_C(0x0000000000000001) //!< Alpha to coverage is supported.
#define EFF3D_CAPS_BLEND_INDEPENDENT      UINT64_C(0x0000000000000002) //!< Blend independent is supported.
#define EFF3D_CAPS_COMPUTE                UINT64_C(0x0000000000000004) //!< Compute shaders are supported.
#define EFF3D_CAPS_CONSERVATIVE_RASTER    UINT64_C(0x0000000000000008) //!< Conservative rasterization is supported.
#define EFF3D_CAPS_DRAW_INDIRECT          UINT64_C(0x0000000000000010) //!< Draw indirect is supported.
#define EFF3D_CAPS_FRAGMENT_DEPTH         UINT64_C(0x0000000000000020) //!< Fragment depth is accessible in fragment shader.
#define EFF3D_CAPS_FRAGMENT_ORDERING      UINT64_C(0x0000000000000040) //!< Fragment ordering is available in fragment shader.
#define EFF3D_CAPS_GRAPHICS_DEBUGGER      UINT64_C(0x0000000000000080) //!< Graphics debugger is present.
#define EFF3D_CAPS_HIDPI                  UINT64_C(0x0000000000000100) //!< HiDPI rendering is supported.
#define EFF3D_CAPS_HMD                    UINT64_C(0x0000000000000200) //!< Head Mounted Display is available.
#define EFF3D_CAPS_INDEX32                UINT64_C(0x0000000000000400) //!< 32-bit indices are supported.
#define EFF3D_CAPS_INSTANCING             UINT64_C(0x0000000000000800) //!< Instancing is supported.
#define EFF3D_CAPS_OCCLUSION_QUERY        UINT64_C(0x0000000000001000) //!< Occlusion query is supported.
#define EFF3D_CAPS_RENDERER_MULTITHREADED UINT64_C(0x0000000000002000) //!< Renderer is on separate thread.
#define EFF3D_CAPS_SWAP_CHAIN             UINT64_C(0x0000000000004000) //!< Multiple windows are supported.
#define EFF3D_CAPS_TEXTURE_2D_ARRAY       UINT64_C(0x0000000000008000) //!< 2D texture array is supported.
#define EFF3D_CAPS_TEXTURE_3D             UINT64_C(0x0000000000010000) //!< 3D textures are supported.
#define EFF3D_CAPS_TEXTURE_BLIT           UINT64_C(0x0000000000020000) //!< Texture blit is supported.
#define EFF3D_CAPS_TEXTURE_COMPARE_ALL    UINT64_C(0x00000000000c0000) //!< All texture compare modes are supported.
#define EFF3D_CAPS_TEXTURE_COMPARE_LEQUAL UINT64_C(0x0000000000080000) //!< Texture compare less equal mode is supported.
#define EFF3D_CAPS_TEXTURE_CUBE_ARRAY     UINT64_C(0x0000000000100000) //!< Cubemap texture array is supported.
#define EFF3D_CAPS_TEXTURE_DIRECT_ACCESS  UINT64_C(0x0000000000200000) //!< CPU direct access to GPU texture memory.
#define EFF3D_CAPS_TEXTURE_READ_BACK      UINT64_C(0x0000000000400000) //!< Read-back texture is supported.
#define EFF3D_CAPS_VERTEX_ATTRIB_HALF     UINT64_C(0x0000000000800000) //!< Vertex attribute half-float is supported.
#define EFF3D_CAPS_VERTEX_ATTRIB_UINT10   UINT64_C(0x0000000000800000) //!< Vertex attribute 10_10_10_2 is supported.

///
#define EFF3D_CAPS_FORMAT_TEXTURE_NONE             UINT16_C(0x0000) //!< Texture format is not supported.
#define EFF3D_CAPS_FORMAT_TEXTURE_2D               UINT16_C(0x0001) //!< Texture format is supported.
#define EFF3D_CAPS_FORMAT_TEXTURE_2D_SRGB          UINT16_C(0x0002) //!< Texture as sRGB format is supported.
#define EFF3D_CAPS_FORMAT_TEXTURE_2D_EMULATED      UINT16_C(0x0004) //!< Texture format is emulated.
#define EFF3D_CAPS_FORMAT_TEXTURE_3D               UINT16_C(0x0008) //!< Texture format is supported.
#define EFF3D_CAPS_FORMAT_TEXTURE_3D_SRGB          UINT16_C(0x0010) //!< Texture as sRGB format is supported.
#define EFF3D_CAPS_FORMAT_TEXTURE_3D_EMULATED      UINT16_C(0x0020) //!< Texture format is emulated.
#define EFF3D_CAPS_FORMAT_TEXTURE_CUBE             UINT16_C(0x0040) //!< Texture format is supported.
#define EFF3D_CAPS_FORMAT_TEXTURE_CUBE_SRGB        UINT16_C(0x0080) //!< Texture as sRGB format is supported.
#define EFF3D_CAPS_FORMAT_TEXTURE_CUBE_EMULATED    UINT16_C(0x0100) //!< Texture format is emulated.
#define EFF3D_CAPS_FORMAT_TEXTURE_VERTEX           UINT16_C(0x0200) //!< Texture format can be used from vertex shader.
#define EFF3D_CAPS_FORMAT_TEXTURE_IMAGE            UINT16_C(0x0400) //!< Texture format can be used as image from compute shader.
#define EFF3D_CAPS_FORMAT_TEXTURE_FRAMEBUFFER      UINT16_C(0x0800) //!< Texture format can be used as frame buffer.
#define EFF3D_CAPS_FORMAT_TEXTURE_FRAMEBUFFER_MSAA UINT16_C(0x1000) //!< Texture format can be used as MSAA frame buffer.
#define EFF3D_CAPS_FORMAT_TEXTURE_MSAA             UINT16_C(0x2000) //!< Texture can be sampled as MSAA.
#define EFF3D_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN      UINT16_C(0x4000) //!< Texture format supports auto-generated mips.

///
#define EFF3D_VIEW_NONE   UINT8_C(0x00) //!<
#define EFF3D_VIEW_STEREO UINT8_C(0x01) //!< View will be rendered in stereo mode.

///
#define EFF3D_SUBMIT_EYE_LEFT       UINT8_C(0x01) //!< Submit to left eye.
#define EFF3D_SUBMIT_EYE_RIGHT      UINT8_C(0x02) //!< Submit to right eye.
#define EFF3D_SUBMIT_EYE_MASK       UINT8_C(0x03) //!<
#define EFF3D_SUBMIT_EYE_FIRST      EFF3D_SUBMIT_EYE_LEFT

#define EFF3D_SUBMIT_RESERVED_SHIFT 7             //!< Internal bits shift.
#define EFF3D_SUBMIT_RESERVED_MASK  UINT8_C(0x80) //!< Internal bits mask.

///
#define EFF3D_PCI_ID_NONE                UINT16_C(0x0000) //!< Autoselect adapter.
#define EFF3D_PCI_ID_SOFTWARE_RASTERIZER UINT16_C(0x0001) //!< Software rasterizer.
#define EFF3D_PCI_ID_AMD                 UINT16_C(0x1002) //!< AMD adapter.
#define EFF3D_PCI_ID_INTEL               UINT16_C(0x8086) //!< Intel adapter.
#define EFF3D_PCI_ID_NVIDIA              UINT16_C(0x10de) //!< nVidia adapter.

///
#define EFF3D_HMD_NONE              UINT8_C(0x00) //!< None.
#define EFF3D_HMD_DEVICE_RESOLUTION UINT8_C(0x01) //!< Has HMD native resolution.
#define EFF3D_HMD_RENDERING         UINT8_C(0x02) //!< Rendering to HMD.

///
#define EFF3D_CUBE_MAP_POSITIVE_X UINT8_C(0x00) //!< Cubemap +x.
#define EFF3D_CUBE_MAP_NEGATIVE_X UINT8_C(0x01) //!< Cubemap -x.
#define EFF3D_CUBE_MAP_POSITIVE_Y UINT8_C(0x02) //!< Cubemap +y.
#define EFF3D_CUBE_MAP_NEGATIVE_Y UINT8_C(0x03) //!< Cubemap -y.
#define EFF3D_CUBE_MAP_POSITIVE_Z UINT8_C(0x04) //!< Cubemap +z.
#define EFF3D_CUBE_MAP_NEGATIVE_Z UINT8_C(0x05) //!< Cubemap -z.


///
#define EFF3D_CLEAR_NONE                  UINT16_C(0x0000) //!< No clear flags.
#define EFF3D_CLEAR_COLOR                 UINT16_C(0x0001) //!< Clear color.
#define EFF3D_CLEAR_DEPTH                 UINT16_C(0x0002) //!< Clear depth.
#define EFF3D_CLEAR_STENCIL               UINT16_C(0x0004) //!< Clear stencil.
#define EFF3D_CLEAR_DISCARD_COLOR_0       UINT16_C(0x0008) //!< Discard frame buffer attachment 0.
#define EFF3D_CLEAR_DISCARD_COLOR_1       UINT16_C(0x0010) //!< Discard frame buffer attachment 1.
#define EFF3D_CLEAR_DISCARD_COLOR_2       UINT16_C(0x0020) //!< Discard frame buffer attachment 2.
#define EFF3D_CLEAR_DISCARD_COLOR_3       UINT16_C(0x0040) //!< Discard frame buffer attachment 3.
#define EFF3D_CLEAR_DISCARD_COLOR_4       UINT16_C(0x0080) //!< Discard frame buffer attachment 4.
#define EFF3D_CLEAR_DISCARD_COLOR_5       UINT16_C(0x0100) //!< Discard frame buffer attachment 5.
#define EFF3D_CLEAR_DISCARD_COLOR_6       UINT16_C(0x0200) //!< Discard frame buffer attachment 6.
#define EFF3D_CLEAR_DISCARD_COLOR_7       UINT16_C(0x0400) //!< Discard frame buffer attachment 7.
#define EFF3D_CLEAR_DISCARD_DEPTH         UINT16_C(0x0800) //!< Discard frame buffer depth attachment.
#define EFF3D_CLEAR_DISCARD_STENCIL       UINT16_C(0x1000) //!< Discard frame buffer stencil attachment.


#define EFF_ALIGN_DECL_16(_decl) EFF_ALIGN_DECL(16, _decl)
#define EFF_ALIGN_DECL_256(_decl) EFF_ALIGN_DECL(256, _decl)
#define EFF_ALIGN_DECL_CACHE_LINE(_decl) EFF_ALIGN_DECL(EFF_CACHE_LINE_SIZE, _decl)

enum EFF3DPrimitiveType
{
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip,
    TriangleFan,
    EFF3DPrimitiveTypeCount
};

inline effUINT64 PackStencil(effUINT frontStencil, effUINT backStencil)
{
    return (effUINT64(backStencil) << 32) | effUINT64(frontStencil);
}

inline effUINT UnpackStencil(effBYTE zeroOrOne, effUINT64 stencil)
{
    return effUINT((stencil >> (32 * zeroOrOne)));
}

#if defined(_DEBUG) || defined(DEBUG)

    #define DX_CHECK(call) \
        {\
            HRESULT hr = call; \
            if (!SUCCEEDED(hr)) \
            {\
            }\
        }
#else
    #define DX_CHECK(call) call
#endif

#endif