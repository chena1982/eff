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



typedef effLPCSTR EFF3DXHANDLE;
typedef EFF3DXHANDLE *LPEFF3DXHANDLE;




// maps unsigned 8 bits/channel to EFF3DCOLOR
#define EFF3DCOLOR_ARGB(a,r,g,b) \
	((EFF3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define EFF3DCOLOR_RGBA(r,g,b,a) EFF3DCOLOR_ARGB(a,r,g,b)
#define EFF3DCOLOR_XRGB(r,g,b)   EFF3DCOLOR_ARGB(0xff,r,g,b)




#define EFF3D_DEFAULT            ((effUINT) -1)
#define EFF3D_DEFAULT_NONPOW2    ((effUINT) -2)
#define EFF3D_FROM_FILE          ((effUINT) -3)
#define EFF3DFMT_FROM_FILE        ((EFF3DFORMAT) -3)



// Flexible vertex format bits
//
#define EFF3DFVF_RESERVED0        0x001
#define EFF3DFVF_POSITION_MASK    0x00E
#define EFF3DFVF_XYZ              0x002
#define EFF3DFVF_XYZRHW           0x004
#define EFF3DFVF_XYZB1            0x006
#define EFF3DFVF_XYZB2            0x008
#define EFF3DFVF_XYZB3            0x00a
#define EFF3DFVF_XYZB4            0x00c
#define EFF3DFVF_XYZB5            0x00e

#define EFF3DFVF_NORMAL           0x010
#define EFF3DFVF_PSIZE            0x020
#define EFF3DFVF_DIFFUSE          0x040
#define EFF3DFVF_SPECULAR         0x080

#define EFF3DFVF_TEXCOUNT_MASK    0xf00
#define EFF3DFVF_TEXCOUNT_SHIFT   8
#define EFF3DFVF_TEX0             0x000
#define EFF3DFVF_TEX1             0x100
#define EFF3DFVF_TEX2             0x200
#define EFF3DFVF_TEX3             0x300
#define EFF3DFVF_TEX4             0x400
#define EFF3DFVF_TEX5             0x500
#define EFF3DFVF_TEX6             0x600
#define EFF3DFVF_TEX7             0x700
#define EFF3DFVF_TEX8             0x800

#define EFF3DFVF_LASTBETA_UBYTE4  0x1000

#define EFF3DFVF_RESERVED2        0xE000  // 4 reserved bits

#define EFF3DFVF_TEXTUREFORMAT2 0         // Two floating point values
#define EFF3DFVF_TEXTUREFORMAT1 3         // One floating point value
#define EFF3DFVF_TEXTUREFORMAT3 1         // Three floating point values
#define D3DFVF_TEXTUREFORMAT4 2         // Four floating point values

#define EFF3DFVF_TEXCOORDSIZE3(CoordIndex) (EFF3DFVF_TEXTUREFORMAT3 << (CoordIndex*2 + 16))
#define EFF3DFVF_TEXCOORDSIZE2(CoordIndex) (EFF3DFVF_TEXTUREFORMAT2)
#define EFF3DFVF_TEXCOORDSIZE4(CoordIndex) (EFF3DFVF_TEXTUREFORMAT4 << (CoordIndex*2 + 16))
#define EFF3DFVF_TEXCOORDSIZE1(CoordIndex) (EFF3DFVF_TEXTUREFORMAT1 << (CoordIndex*2 + 16))


/* Pool types */
typedef enum _EFF3DPOOL
{
	EFF3DPOOL_DEFAULT                 = 0,
	EFF3DPOOL_MANAGED                 = 1,
	EFF3DPOOL_SYSTEMMEM               = 2,
	EFF3DPOOL_SCRATCH                 = 3,

	EFF3DPOOL_FORCE_DWORD             = 0x7fffffff
} EFF3DPOOL;

/* Usages */
#define EFF3DUSAGE_RENDERTARGET       (0x00000001L)
#define EFF3DUSAGE_DEPTHSTENCIL       (0x00000002L)
#define EFF3DUSAGE_DYNAMIC            (0x00000200L)

// When passed to CheckDeviceFormat, EFF3DUSAGE_AUTOGENMIPMAP may return
// EFF3DOK_NOAUTOGEN if the device doesn't support autogeneration for that format.
// EFF3DOK_NOAUTOGEN is a success code, not a failure code... the SUCCEEDED and FAILED macros
// will return true and false respectively for this code.
#define EFF3DUSAGE_AUTOGENMIPMAP      (0x00000400L)
#define EFF3DUSAGE_DMAP               (0x00004000L)

/* Usages for Vertex/Index buffers */
#define EFF3DUSAGE_WRITEONLY          (0x00000008L)
#define EFF3DUSAGE_SOFTWAREPROCESSING (0x00000010L)
#define EFF3DUSAGE_DONOTCLIP          (0x00000020L)
#define EFF3DUSAGE_POINTS             (0x00000040L)
#define EFF3DUSAGE_RTPATCHES          (0x00000080L)
#define EFF3DUSAGE_NPATCHES           (0x00000100L)
#define EFF3DUSAGE_DYNAMIC            (0x00000200L)

#ifndef EFF3D_MAKEFOURCC
#define EFF3D_MAKEFOURCC(ch0, ch1, ch2, ch3) \
	((effUINT)(effBYTE)(ch0) | ((effUINT)(effBYTE)(ch1) << 8) | \
	((effUINT)(effBYTE)(ch2) << 16) | ((effUINT)(effBYTE)(ch3) << 24 ))
#endif /* defined(EFF3D_MAKEFOURCC) */

typedef enum _EFF3DFORMAT
{
	EFF3DFMT_UNKNOWN              =  0,

	EFF3DFMT_R8G8B8               = 20,
	EFF3DFMT_A8R8G8B8             = 21,
	EFF3DFMT_X8R8G8B8             = 22,
	EFF3DFMT_R5G6B5               = 23,
	EFF3DFMT_X1R5G5B5             = 24,
	EFF3DFMT_A1R5G5B5             = 25,
	EFF3DFMT_A4R4G4B4             = 26,
	EFF3DFMT_R3G3B2               = 27,
	EFF3DFMT_A8                   = 28,
	EFF3DFMT_A8R3G3B2             = 29,
	EFF3DFMT_X4R4G4B4             = 30,
	EFF3DFMT_A2B10G10R10          = 31,
	EFF3DFMT_A8B8G8R8             = 32,
	EFF3DFMT_X8B8G8R8             = 33,
	EFF3DFMT_G16R16               = 34,
	EFF3DFMT_A2R10G10B10          = 35,
	EFF3DFMT_A16B16G16R16         = 36,

	EFF3DFMT_A8P8                 = 40,
	EFF3DFMT_P8                   = 41,

	EFF3DFMT_L8                   = 50,
	EFF3DFMT_A8L8                 = 51,
	EFF3DFMT_A4L4                 = 52,

	EFF3DFMT_V8U8                 = 60,
	EFF3DFMT_L6V5U5               = 61,
	EFF3DFMT_X8L8V8U8             = 62,
	EFF3DFMT_Q8W8V8U8             = 63,
	EFF3DFMT_V16U16               = 64,
	EFF3DFMT_A2W10V10U10          = 67,

	EFF3DFMT_UYVY                 = EFF3D_MAKEFOURCC('U', 'Y', 'V', 'Y'),
	EFF3DFMT_R8G8_B8G8            = EFF3D_MAKEFOURCC('R', 'G', 'B', 'G'),
	EFF3DFMT_YUY2                 = EFF3D_MAKEFOURCC('Y', 'U', 'Y', '2'),
	EFF3DFMT_G8R8_G8B8            = EFF3D_MAKEFOURCC('G', 'R', 'G', 'B'),
	EFF3DFMT_DXT1                 = EFF3D_MAKEFOURCC('D', 'X', 'T', '1'),
	EFF3DFMT_DXT2                 = EFF3D_MAKEFOURCC('D', 'X', 'T', '2'),
	EFF3DFMT_DXT3                 = EFF3D_MAKEFOURCC('D', 'X', 'T', '3'),
	EFF3DFMT_DXT4                 = EFF3D_MAKEFOURCC('D', 'X', 'T', '4'),
	EFF3DFMT_DXT5                 = EFF3D_MAKEFOURCC('D', 'X', 'T', '5'),

	EFF3DFMT_D16_LOCKABLE         = 70,
	EFF3DFMT_D32                  = 71,
	EFF3DFMT_D15S1                = 73,
	EFF3DFMT_D24S8                = 75,
	EFF3DFMT_D24X8                = 77,
	EFF3DFMT_D24X4S4              = 79,
	EFF3DFMT_D16                  = 80,

	EFF3DFMT_D32F_LOCKABLE        = 82,
	EFF3DFMT_D24FS8               = 83,


	EFF3DFMT_L16                  = 81,

	EFF3DFMT_VERTEXDATA           =100,
	EFF3DFMT_INDEX16              =101,
	EFF3DFMT_INDEX32              =102,

	EFF3DFMT_Q16W16V16U16         =110,

	EFF3DFMT_MULTI2_ARGB8         = EFF3D_MAKEFOURCC('M','E','T','1'),

	// Floating point surface formats

	// s10e5 formats (16-bits per channel)
	EFF3DFMT_R16F                 = 111,
	EFF3DFMT_G16R16F              = 112,
	EFF3DFMT_A16B16G16R16F        = 113,

	// IEEE s23e8 formats (32-bits per channel)
	EFF3DFMT_R32F                 = 114,
	EFF3DFMT_G32R32F              = 115,
	EFF3DFMT_A32B32G32R32F        = 116,

	EFF3DFMT_CxV8U8               = 117,


	EFF3DFMT_FORCE_DWORD          =0x7fffffff
} EFF3DFORMAT;

// Primitives supported by draw-primitive API
typedef enum _EFF3DPRIMITIVETYPE {
	EFF3DPT_POINTLIST             = 1,
	EFF3DPT_LINELIST              = 2,
	EFF3DPT_LINESTRIP             = 3,
	EFF3DPT_TRIANGLELIST          = 4,
	EFF3DPT_TRIANGLESTRIP         = 5,
	EFF3DPT_TRIANGLEFAN           = 6,
	EFF3DPT_FORCE_DWORD           = 0x7fffffff, /* force 32-bit size enum */
} EFF3DPRIMITIVETYPE;

typedef enum _EFF3DRENDERSTATETYPE {
	EFF3DRS_ZENABLE                   = 7,    /* EFF3DZBUFFERTYPE (or TRUE/FALSE for legacy) */
	EFF3DRS_FILLMODE                  = 8,    /* EFF3DFILLMODE */
	EFF3DRS_SHADEMODE                 = 9,    /* EFF3DSHADEMODE */
	EFF3DRS_ZWRITEENABLE              = 14,   /* TRUE to enable z writes */
	EFF3DRS_ALPHATESTENABLE           = 15,   /* TRUE to enable alpha tests */
	EFF3DRS_LASTPIXEL                 = 16,   /* TRUE for last-pixel on lines */
	EFF3DRS_SRCBLEND                  = 19,   /* EFF3DBLEND */
	EFF3DRS_DESTBLEND                 = 20,   /* EFF3DBLEND */
	EFF3DRS_CULLMODE                  = 22,   /* EFF3DCULL */
	EFF3DRS_ZFUNC                     = 23,   /* EFF3DCMPFUNC */
	EFF3DRS_ALPHAREF                  = 24,   /* EFF3DFIXED */
	EFF3DRS_ALPHAFUNC                 = 25,   /* EFF3DCMPFUNC */
	EFF3DRS_DITHERENABLE              = 26,   /* TRUE to enable dithering */
	EFF3DRS_ALPHABLENDENABLE          = 27,   /* TRUE to enable alpha blending */
	EFF3DRS_FOGENABLE                 = 28,   /* TRUE to enable fog blending */
	EFF3DRS_SPECULARENABLE            = 29,   /* TRUE to enable specular */
	EFF3DRS_FOGCOLOR                  = 34,   /* EFF3DCOLOR */
	EFF3DRS_FOGTABLEMODE              = 35,   /* EFF3DFOGMODE */
	EFF3DRS_FOGSTART                  = 36,   /* Fog start (for both vertex and pixel fog) */
	EFF3DRS_FOGEND                    = 37,   /* Fog end      */
	EFF3DRS_FOGDENSITY                = 38,   /* Fog density  */
	EFF3DRS_RANGEFOGENABLE            = 48,   /* Enables range-based fog */
	EFF3DRS_STENCILENABLE             = 52,   /* effBOOL enable/disable stenciling */
	EFF3DRS_STENCILFAIL               = 53,   /* EFF3DSTENCILOP to do if stencil test fails */
	EFF3DRS_STENCILZFAIL              = 54,   /* EFF3DSTENCILOP to do if stencil test passes and Z test fails */
	EFF3DRS_STENCILPASS               = 55,   /* EFF3DSTENCILOP to do if both stencil and Z tests pass */
	EFF3DRS_STENCILFUNC               = 56,   /* EFF3DCMPFUNC fn.  Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	EFF3DRS_STENCILREF                = 57,   /* Reference value used in stencil test */
	EFF3DRS_STENCILMASK               = 58,   /* Mask value used in stencil test */
	EFF3DRS_STENCILWRITEMASK          = 59,   /* Write mask applied to values written to stencil buffer */
	EFF3DRS_TEXTUREFACTOR             = 60,   /* EFF3DCOLOR used for multi-texture blend */
	EFF3DRS_WRAP0                     = 128,  /* wrap for 1st texture coord. set */
	EFF3DRS_WRAP1                     = 129,  /* wrap for 2nd texture coord. set */
	EFF3DRS_WRAP2                     = 130,  /* wrap for 3rd texture coord. set */
	EFF3DRS_WRAP3                     = 131,  /* wrap for 4th texture coord. set */
	EFF3DRS_WRAP4                     = 132,  /* wrap for 5th texture coord. set */
	EFF3DRS_WRAP5                     = 133,  /* wrap for 6th texture coord. set */
	EFF3DRS_WRAP6                     = 134,  /* wrap for 7th texture coord. set */
	EFF3DRS_WRAP7                     = 135,  /* wrap for 8th texture coord. set */
	EFF3DRS_CLIPPING                  = 136,
	EFF3DRS_LIGHTING                  = 137,
	EFF3DRS_AMBIENT                   = 139,
	EFF3DRS_FOGVERTEXMODE             = 140,
	EFF3DRS_COLORVERTEX               = 141,
	EFF3DRS_LOCALVIEWER               = 142,
	EFF3DRS_NORMALIZENORMALS          = 143,
	EFF3DRS_DIFFUSEMATERIALSOURCE     = 145,
	EFF3DRS_SPECULARMATERIALSOURCE    = 146,
	EFF3DRS_AMBIENTMATERIALSOURCE     = 147,
	EFF3DRS_EMISSIVEMATERIALSOURCE    = 148,
	EFF3DRS_VERTEXBLEND               = 151,
	EFF3DRS_CLIPPLANEENABLE           = 152,
	EFF3DRS_POINTSIZE                 = 154,   /* float point size */
	EFF3DRS_POINTSIZE_MIN             = 155,   /* float point size min threshold */
	EFF3DRS_POINTSPRITEENABLE         = 156,   /* effBOOL point texture coord control */
	EFF3DRS_POINTSCALEENABLE          = 157,   /* effBOOL point size scale enable */
	EFF3DRS_POINTSCALE_A              = 158,   /* float point attenuation A value */
	EFF3DRS_POINTSCALE_B              = 159,   /* float point attenuation B value */
	EFF3DRS_POINTSCALE_C              = 160,   /* float point attenuation C value */
	EFF3DRS_MULTISAMPLEANTIALIAS      = 161,  // effBOOL - set to do FSAA with multisample buffer
	EFF3DRS_MULTISAMPLEMASK           = 162,  // effUINT - per-sample enable/disable
	EFF3DRS_PATCHEDGESTYLE            = 163,  // Sets whether patch edges will use float style tessellation
	EFF3DRS_DEBUGMONITORTOKEN         = 165,  // DEBUG ONLY - token to debug monitor
	EFF3DRS_POINTSIZE_MAX             = 166,   /* float point size max threshold */
	EFF3DRS_INDEXEDVERTEXBLENDENABLE  = 167,
	EFF3DRS_COLORWRITEENABLE          = 168,  // per-channel write enable
	EFF3DRS_TWEENFACTOR               = 170,   // float tween factor
	EFF3DRS_BLENDOP                   = 171,   // EFF3DBLENDOP setting
	EFF3DRS_POSITIONDEGREE            = 172,   // NPatch position interpolation degree. EFF3DDEGREE_LINEAR or EFF3DDEGREE_CUBIC (default)
	EFF3DRS_NORMALDEGREE              = 173,   // NPatch normal interpolation degree. EFF3DDEGREE_LINEAR (default) or EFF3DDEGREE_QUADRATIC
	EFF3DRS_SCISSORTESTENABLE         = 174,
	EFF3DRS_SLOPESCALEDEPTHBIAS       = 175,
	EFF3DRS_ANTIALIASEDLINEENABLE     = 176,
	EFF3DRS_MINTESSELLATIONLEVEL      = 178,
	EFF3DRS_MAXTESSELLATIONLEVEL      = 179,
	EFF3DRS_ADAPTIVETESS_X            = 180,
	EFF3DRS_ADAPTIVETESS_Y            = 181,
	EFF3DRS_ADAPTIVETESS_Z            = 182,
	EFF3DRS_ADAPTIVETESS_W            = 183,
	EFF3DRS_ENABLEADAPTIVETESSELLATION = 184,
	EFF3DRS_TWOSIDEDSTENCILMODE       = 185,   /* effBOOL enable/disable 2 sided stenciling */
	EFF3DRS_CCW_STENCILFAIL           = 186,   /* EFF3DSTENCILOP to do if ccw stencil test fails */
	EFF3DRS_CCW_STENCILZFAIL          = 187,   /* EFF3DSTENCILOP to do if ccw stencil test passes and Z test fails */
	EFF3DRS_CCW_STENCILPASS           = 188,   /* EFF3DSTENCILOP to do if both ccw stencil and Z tests pass */
	EFF3DRS_CCW_STENCILFUNC           = 189,   /* EFF3DCMPFUNC fn.  ccw Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	EFF3DRS_COLORWRITEENABLE1         = 190,   /* Additional ColorWriteEnables for the devices that support EFF3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	EFF3DRS_COLORWRITEENABLE2         = 191,   /* Additional ColorWriteEnables for the devices that support EFF3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	EFF3DRS_COLORWRITEENABLE3         = 192,   /* Additional ColorWriteEnables for the devices that support EFF3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	EFF3DRS_BLENDFACTOR               = 193,   /* EFF3DCOLOR used for a constant blend factor during alpha blending for devices that support EFF3DPBLENDCAPS_BLENDFACTOR */
	EFF3DRS_SRGBWRITEENABLE           = 194,   /* Enable rendertarget writes to be DE-linearized to SRGB (for formats that expose EFF3DUSAGE_QUERY_SRGBWRITE) */
	EFF3DRS_DEPTHBIAS                 = 195,
	EFF3DRS_WRAP8                     = 198,   /* Additional wrap states for vs_3_0+ attributes with EFF3DDECLUSAGE_TEXCOORD */
	EFF3DRS_WRAP9                     = 199,
	EFF3DRS_WRAP10                    = 200,
	EFF3DRS_WRAP11                    = 201,
	EFF3DRS_WRAP12                    = 202,
	EFF3DRS_WRAP13                    = 203,
	EFF3DRS_WRAP14                    = 204,
	EFF3DRS_WRAP15                    = 205,
	EFF3DRS_SEPARATEALPHABLENDENABLE  = 206,  /* TRUE to enable a separate blending function for the alpha channel */
	EFF3DRS_SRCBLENDALPHA             = 207,  /* SRC blend factor for the alpha channel when EFF3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	EFF3DRS_DESTBLENDALPHA            = 208,  /* DST blend factor for the alpha channel when EFF3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	EFF3DRS_BLENDOPALPHA              = 209,  /* Blending operation for the alpha channel when EFF3DRS_SEPARATEDESTALPHAENABLE is TRUE */


	EFF3DRS_FORCE_DWORD               = 0x7fffffff, /* force 32-bit size enum */
} EFF3DRENDERSTATETYPE;

typedef enum _EFF3DTRANSFORMSTATETYPE {
	EFF3DTS_VIEW          = 2,
	EFF3DTS_PROJECTION    = 3,
	EFF3DTS_TEXTURE0      = 16,
	EFF3DTS_TEXTURE1      = 17,
	EFF3DTS_TEXTURE2      = 18,
	EFF3DTS_TEXTURE3      = 19,
	EFF3DTS_TEXTURE4      = 20,
	EFF3DTS_TEXTURE5      = 21,
	EFF3DTS_TEXTURE6      = 22,
	EFF3DTS_TEXTURE7      = 23,
	EFF3DTS_FORCE_DWORD     = 0x7fffffff, /* force 32-bit size enum */
} EFF3DTRANSFORMSTATETYPE;

#define EFF3DTS_WORLDMATRIX(index) (EFF3DTRANSFORMSTATETYPE)(index + 256)
#define EFF3DTS_WORLD  EFF3DTS_WORLDMATRIX(0)
#define EFF3DTS_WORLD1 EFF3DTS_WORLDMATRIX(1)
#define EFF3DTS_WORLD2 EFF3DTS_WORLDMATRIX(2)
#define EFF3DTS_WORLD3 EFF3DTS_WORLDMATRIX(3)

/*
* State enumerants for per-stage processing of fixed function pixel processing
* Two of these affect fixed function vertex processing as well: TEXTURETRANSFORMFLAGS and TEXCOORDINDEX.
*/
typedef enum _EFF3DTEXTURESTAGESTATETYPE
{
	EFF3DTSS_COLOROP        =  1, /* EFF3DTEXTUREOP - per-stage blending controls for color channels */
	EFF3DTSS_COLORARG1      =  2, /* EFF3DTA_* (texture arg) */
	EFF3DTSS_COLORARG2      =  3, /* EFF3DTA_* (texture arg) */
	EFF3DTSS_ALPHAOP        =  4, /* EFF3DTEXTUREOP - per-stage blending controls for alpha channel */
	EFF3DTSS_ALPHAARG1      =  5, /* EFF3DTA_* (texture arg) */
	EFF3DTSS_ALPHAARG2      =  6, /* EFF3DTA_* (texture arg) */
	EFF3DTSS_BUMPENVMAT00   =  7, /* float (bump mapping matrix) */
	EFF3DTSS_BUMPENVMAT01   =  8, /* float (bump mapping matrix) */
	EFF3DTSS_BUMPENVMAT10   =  9, /* float (bump mapping matrix) */
	EFF3DTSS_BUMPENVMAT11   = 10, /* float (bump mapping matrix) */
	EFF3DTSS_TEXCOORDINDEX  = 11, /* identifies which set of texture coordinates index this texture */
	EFF3DTSS_BUMPENVLSCALE  = 22, /* float scale for bump map luminance */
	EFF3DTSS_BUMPENVLOFFSET = 23, /* float offset for bump map luminance */
	EFF3DTSS_TEXTURETRANSFORMFLAGS = 24, /* EFF3DTEXTURETRANSFORMFLAGS controls texture transform */
	EFF3DTSS_COLORARG0      = 26, /* EFF3DTA_* third arg for triadic ops */
	EFF3DTSS_ALPHAARG0      = 27, /* EFF3DTA_* third arg for triadic ops */
	EFF3DTSS_RESULTARG      = 28, /* EFF3DTA_* arg for result (CURRENT or TEMP) */
	EFF3DTSS_CONSTANT       = 32, /* Per-stage constant EFF3DTA_CONSTANT */


	EFF3DTSS_FORCE_DWORD   = 0x7fffffff, /* force 32-bit size enum */
} EFF3DTEXTURESTAGESTATETYPE;

typedef struct _EFF3DVIEWPORT9 {
	effUINT       X;
	effUINT       Y;            /* Viewport Top left */
	effUINT       Width;
	effUINT       Height;       /* Viewport Dimensions */
	float       MinZ;         /* Min/max of clip Volume */
	float       MaxZ;
} EFF3DVIEWPORT9;

typedef enum _EFF3DBLEND {
	EFF3DBLEND_ZERO               = 1,
	EFF3DBLEND_ONE                = 2,
	EFF3DBLEND_SRCCOLOR           = 3,
	EFF3DBLEND_INVSRCCOLOR        = 4,
	EFF3DBLEND_SRCALPHA           = 5,
	EFF3DBLEND_INVSRCALPHA        = 6,
	EFF3DBLEND_DESTALPHA          = 7,
	EFF3DBLEND_INVDESTALPHA       = 8,
	EFF3DBLEND_DESTCOLOR          = 9,
	EFF3DBLEND_INVDESTCOLOR       = 10,
	EFF3DBLEND_SRCALPHASAT        = 11,
	EFF3DBLEND_BOTHSRCALPHA       = 12,
	EFF3DBLEND_BOTHINVSRCALPHA    = 13,
	EFF3DBLEND_BLENDFACTOR        = 14, /* Only supported if D3DPBLENDCAPS_BLENDFACTOR is on */
	EFF3DBLEND_INVBLENDFACTOR     = 15, /* Only supported if D3DPBLENDCAPS_BLENDFACTOR is on */
	EFF3DBLEND_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
} EFF3DBLEND;

/*
* Options for clearing
*/
#define EFF3DCLEAR_TARGET            0x00000001l  /* Clear target surface */
#define EFF3DCLEAR_ZBUFFER           0x00000002l  /* Clear target z buffer */
#define EFF3DCLEAR_STENCIL           0x00000004l  /* Clear stencil planes */

typedef struct _EFF3DMATERIAL
{
	_EFF3DMATERIAL()
	{
		Diffuse.r = Diffuse.g = Diffuse.b = 0.5f;
		Diffuse.a = 1.0f;
		Ambient.r = Ambient.g = Ambient.b = 0.2f;
		Diffuse.a = 1.0f;
		Specular.r = Specular.g = Specular.b = 1.0f;
		Diffuse.a = 1.0f;
		Emissive.r = Emissive.g = Emissive.b = 1.0f;
		Emissive.a = 1.0f;
		Power = 1.0f;
	}
	EFF3DCOLORVALUE   Diffuse;        /* Diffuse color RGBA */
	EFF3DCOLORVALUE   Ambient;        /* Ambient color RGB */
	EFF3DCOLORVALUE   Specular;       /* Specular 'shininess' */
	EFF3DCOLORVALUE   Emissive;       /* Emissive color RGB */
	float            Power;          /* Sharpness if specular highlight */
} EFF3DMATERIAL;



typedef enum _EFF3DLIGHTTYPE 
{
	EFF3DLIGHT_POINT          = 1,
	EFF3DLIGHT_SPOT           = 2,
	EFF3DLIGHT_DIRECTIONAL    = 3,
	EFF3DLIGHT_FORCE_DWORD    = 0x7fffffff, /* force 32-bit size enum */
} EFF3DLIGHTTYPE;

typedef struct _EFF3DLIGHT
{
	EFF3DLIGHTTYPE    Type;            /* Type of light source */
	EFF3DCOLORVALUE   Diffuse;         /* Diffuse color of light */
	EFF3DCOLORVALUE   Specular;        /* Specular color of light */
	EFF3DCOLORVALUE   Ambient;         /* Ambient color of light */
	//EFF3DxVector3       Position;         /* Position in world space */
	//EFF3DxVector3       Direction;        /* Direction in world space */
	effFLOAT           Range;            /* Cutoff range */
	effFLOAT           Falloff;          /* Falloff */
	effFLOAT           Attenuation0;     /* Constant attenuation */
	effFLOAT           Attenuation1;     /* Linear attenuation */
	effFLOAT           Attenuation2;     /* Quadratic attenuation */
	effFLOAT           Theta;            /* Inner angle of spotlight cone */
	effFLOAT           Phi;              /* Outer angle of spotlight cone */
} EFF3DLIGHT;



typedef enum _EFF3DTEXTUREOP
{
	// Control
	EFF3DTOP_DISABLE              = 1,      // disables stage
	EFF3DTOP_SELECTARG1           = 2,      // the default
	EFF3DTOP_SELECTARG2           = 3,

	// Modulate
	EFF3DTOP_MODULATE             = 4,      // multiply args together
	EFF3DTOP_MODULATE2X           = 5,      // multiply and  1 bit
	EFF3DTOP_MODULATE4X           = 6,      // multiply and  2 bits

	// Add
	EFF3DTOP_ADD                  =  7,   // add arguments together
	EFF3DTOP_ADDSIGNED            =  8,   // add with -0.5 bias
	EFF3DTOP_ADDSIGNED2X          =  9,   // as above but left  1 bit
	EFF3DTOP_SUBTRACT             = 10,   // Arg1 - Arg2, with no saturation
	EFF3DTOP_ADDSMOOTH            = 11,   // add 2 args, subtract product
	// Arg1 + Arg2 - Arg1*Arg2
	// = Arg1 + (1-Arg1)*Arg2

	// Linear alpha blend: Arg1*(Alpha) + Arg2*(1-Alpha)
	EFF3DTOP_BLENDDIFFUSEALPHA    = 12, // iterated alpha
	EFF3DTOP_BLENDTEXTUREALPHA    = 13, // texture alpha
	EFF3DTOP_BLENDFACTORALPHA     = 14, // alpha from EFF3DRS_TEXTUREFACTOR

	// Linear alpha blend with pre-multiplied arg1 input: Arg1 + Arg2*(1-Alpha)
	EFF3DTOP_BLENDTEXTUREALPHAPM  = 15, // texture alpha
	EFF3DTOP_BLENDCURRENTALPHA    = 16, // by alpha of current color

	// Specular mapping
	EFF3DTOP_PREMODULATE            = 17,     // modulate with next texture before use
	EFF3DTOP_MODULATEALPHA_ADDCOLOR = 18,     // Arg1.RGB + Arg1.A*Arg2.RGB
	// COLOROP only
	EFF3DTOP_MODULATECOLOR_ADDALPHA = 19,     // Arg1.RGB*Arg2.RGB + Arg1.A
	// COLOROP only
	EFF3DTOP_MODULATEINVALPHA_ADDCOLOR = 20,  // (1-Arg1.A)*Arg2.RGB + Arg1.RGB
	// COLOROP only
	EFF3DTOP_MODULATEINVCOLOR_ADDALPHA = 21,  // (1-Arg1.RGB)*Arg2.RGB + Arg1.A
	// COLOROP only

	// Bump mapping
	EFF3DTOP_BUMPENVMAP           = 22, // per pixel env map perturbation
	EFF3DTOP_BUMPENVMAPLUMINANCE  = 23, // with luminance channel

	// This can do either diffuse or specular bump mapping with correct input.
	// Performs the function (Arg1.R*Arg2.R + Arg1.G*Arg2.G + Arg1.B*Arg2.B)
	// where each component has been scaled and offset to make it signed.
	// The result is replicated into all four (including alpha) channels.
	// This is a valid COLOROP only.
	EFF3DTOP_DOTPRODUCT3          = 24,

	// Triadic ops
	EFF3DTOP_MULTIPLYADD          = 25, // Arg0 + Arg1*Arg2
	EFF3DTOP_LERP                 = 26, // (Arg0)*Arg1 + (1-Arg0)*Arg2

	EFF3DTOP_FORCE_DWORD = 0x7fffffff,
} EFF3DTEXTUREOP;

/*
* Values for COLORARG0,1,2, ALPHAARG0,1,2, and RESULTARG texture blending
* operations set in texture processing stage controls in EFF3DRENDERSTATE.
*/
#define EFF3DTA_SELECTMASK        0x0000000f  // mask for arg selector
#define EFF3DTA_DIFFUSE           0x00000000  // select diffuse color (read only)
#define EFF3DTA_CURRENT           0x00000001  // select stage destination register (read/write)
#define EFF3DTA_TEXTURE           0x00000002  // select texture color (read only)
#define EFF3DTA_TFACTOR           0x00000003  // select EFF3DRS_TEXTUREFACTOR (read only)
#define EFF3DTA_SPECULAR          0x00000004  // select specular color (read only)
#define EFF3DTA_TEMP              0x00000005  // select temporary register color (read/write)
#define EFF3DTA_CONSTANT		  0x00000006  // select texture stage constant
#define EFF3DTA_COMPLEMENT        0x00000010  // take 1.0 - x (read modifier)
#define EFF3DTA_ALPHAREPLICATE    0x00000020  // replicate alpha to color components (read modifier)

typedef enum _EFF3DCULL {
	EFF3DCULL_NONE                = 1,
	EFF3DCULL_CW                  = 2,
	EFF3DCULL_CCW                 = 3,
	EFF3DCULL_FORCE_DWORD         = 0x7fffffff, /* force 32-bit size enum */
} EFF3DCULL;

typedef enum _EFF3DZBUFFERTYPE {
	EFF3DZB_FALSE                 = 0,
	EFF3DZB_TRUE                  = 1, // Z buffering
	EFF3DZB_USEW                  = 2, // W buffering
	EFF3DZB_FORCE_DWORD           = 0x7fffffff, /* force 32-bit size enum */
} EFF3DZBUFFERTYPE;

/* Usages for Vertex/Index buffers */
#define EFF3DUSAGE_WRITEONLY          (0x00000008L)
#define EFF3DUSAGE_SOFTWAREPROCESSING (0x00000010L)
#define EFF3DUSAGE_DONOTCLIP          (0x00000020L)
#define EFF3DUSAGE_POINTS             (0x00000040L)
#define EFF3DUSAGE_RTPATCHES          (0x00000080L)
#define EFF3DUSAGE_NPATCHES           (0x00000100L)
#define EFF3DUSAGE_DYNAMIC            (0x00000200L)


typedef enum _EFF3DVSD_TOKENTYPE
{
	EFF3DVSD_TOKEN_NOP        = 0,    // NOP or extension
	EFF3DVSD_TOKEN_STREAM,            // stream selector
	EFF3DVSD_TOKEN_STREAMDATA,        // stream data definition (map to vertex input memory)
	EFF3DVSD_TOKEN_TESSELLATOR,       // vertex input memory from tessellator
	EFF3DVSD_TOKEN_CONSTMEM,          // constant memory from shader
	EFF3DVSD_TOKEN_EXT,               // extension
	EFF3DVSD_TOKEN_END = 7,           // end-of-array (requires all effUINT bits to be 1)
	EFF3DVSD_FORCE_DWORD = 0x7fffffff,// force 32-bit size enum
} EFF3DVSD_TOKENTYPE;

#define EFF3DVSD_TOKENTYPESHIFT   29
#define EFF3DVSD_TOKENTYPEMASK    (7 << EFF3DVSD_TOKENTYPESHIFT)

#define EFF3DVSD_STREAMNUMBERSHIFT 0
#define EFF3DVSD_STREAMNUMBERMASK (0xF << EFF3DVSD_STREAMNUMBERSHIFT)

#define EFF3DVSD_DATALOADTYPESHIFT 28
#define EFF3DVSD_DATALOADTYPEMASK (0x1 << EFF3DVSD_DATALOADTYPESHIFT)

#define EFF3DVSD_DATATYPESHIFT 16
#define EFF3DVSD_DATATYPEMASK (0xF << EFF3DVSD_DATATYPESHIFT)

#define EFF3DVSD_SKIPCOUNTSHIFT 16
#define EFF3DVSD_SKIPCOUNTMASK (0xF << EFF3DVSD_SKIPCOUNTSHIFT)

#define EFF3DVSD_VERTEXREGSHIFT 0
#define EFF3DVSD_VERTEXREGMASK (0x1F << EFF3DVSD_VERTEXREGSHIFT)

#define EFF3DVSD_VERTEXREGINSHIFT 20
#define EFF3DVSD_VERTEXREGINMASK (0xF << EFF3DVSD_VERTEXREGINSHIFT)

#define EFF3DVSD_CONSTCOUNTSHIFT 25
#define EFF3DVSD_CONSTCOUNTMASK (0xF << EFF3DVSD_CONSTCOUNTSHIFT)

#define EFF3DVSD_CONSTADDRESSSHIFT 0
#define EFF3DVSD_CONSTADDRESSMASK (0x7F << EFF3DVSD_CONSTADDRESSSHIFT)

#define EFF3DVSD_CONSTRSSHIFT 16
#define EFF3DVSD_CONSTRSMASK (0x1FFF << EFF3DVSD_CONSTRSSHIFT)

#define EFF3DVSD_EXTCOUNTSHIFT 24
#define EFF3DVSD_EXTCOUNTMASK (0x1F << EFF3DVSD_EXTCOUNTSHIFT)

#define EFF3DVSD_EXTINFOSHIFT 0
#define EFF3DVSD_EXTINFOMASK (0xFFFFFF << EFF3DVSD_EXTINFOSHIFT)

#define EFF3DVSD_MAKETOKENTYPE(tokenType) ((tokenType << EFF3DVSD_TOKENTYPESHIFT) & EFF3DVSD_TOKENTYPEMASK)

// macros for generation of CreateVertexShader Declaration token array

// Set current stream
// _StreamNumber [0..(MaxStreams-1)] stream to get data from
//
#define EFF3DVSD_STREAM( _StreamNumber ) \
	(EFF3DVSD_MAKETOKENTYPE(EFF3DVSD_TOKEN_STREAM) | (_StreamNumber))

// Set tessellator stream
//
#define EFF3DVSD_STREAMTESSSHIFT 28
#define EFF3DVSD_STREAMTESSMASK (1 << EFF3DVSD_STREAMTESSSHIFT)
#define EFF3DVSD_STREAM_TESS( ) \
	(EFF3DVSD_MAKETOKENTYPE(EFF3DVSD_TOKEN_STREAM) | (EFF3DVSD_STREAMTESSMASK))

// bind single vertex register to vertex element from vertex stream
//
// _VertexRegister [0..15] address of the vertex register
// _Type [EFF3DVSDT_*] dimensionality and arithmetic data type

#define EFF3DVSD_REG( _VertexRegister, _Type ) \
	(EFF3DVSD_MAKETOKENTYPE(EFF3DVSD_TOKEN_STREAMDATA) |            \
	((_Type) << EFF3DVSD_DATATYPESHIFT) | (_VertexRegister))

// Skip _DWORDCount DWORDs in vertex
//
#define EFF3DVSD_SKIP( _DWORDCount ) \
	(EFF3DVSD_MAKETOKENTYPE(EFF3DVSD_TOKEN_STREAMDATA) | 0x10000000 | \
	((_DWORDCount) << EFF3DVSD_SKIPCOUNTSHIFT))

// load data into vertex shader constant memory
//
// _ConstantAddress [0..95] - address of constant array to begin filling data
// _Count [0..15] - number of constant vectors to load (4 DWORDs each)
// followed by 4*_Count DWORDS of data
//
#define EFF3DVSD_CONST( _ConstantAddress, _Count ) \
	(EFF3DVSD_MAKETOKENTYPE(EFF3DVSD_TOKEN_CONSTMEM) | \
	((_Count) << EFF3DVSD_CONSTCOUNTSHIFT) | (_ConstantAddress))

// enable tessellator generated normals
//
// _VertexRegisterIn  [0..15] address of vertex register whose input stream
//                            will be used in normal computation
// _VertexRegisterOut [0..15] address of vertex register to output the normal to
//
#define EFF3DVSD_TESSNORMAL( _VertexRegisterIn, _VertexRegisterOut ) \
	(EFF3DVSD_MAKETOKENTYPE(EFF3DVSD_TOKEN_TESSELLATOR) | \
	((_VertexRegisterIn) << EFF3DVSD_VERTEXREGINSHIFT) | \
	((0x02) << EFF3DVSD_DATATYPESHIFT) | (_VertexRegisterOut))

// enable tessellator generated surface parameters
//
// _VertexRegister [0..15] address of vertex register to output parameters
//
#define EFF3DVSD_TESSUV( _VertexRegister ) \
	(EFF3DVSD_MAKETOKENTYPE(EFF3DVSD_TOKEN_TESSELLATOR) | 0x10000000 | \
	((0x01) << EFF3DVSD_DATATYPESHIFT) | (_VertexRegister))

// Generates END token
//
#define EFF3DVSD_END() 0xFFFFFFFF

// Generates NOP token
#define EFF3DVSD_NOP() 0x00000000

// bit declarations for _Type fields
#define EFF3DVSDT_FLOAT1      0x00    // 1D float expanded to (value, 0., 0., 1.)
#define EFF3DVSDT_FLOAT2      0x01    // 2D float expanded to (value, value, 0., 1.)
#define EFF3DVSDT_FLOAT3      0x02    // 3D float expanded to (value, value, value, 1.)
#define EFF3DVSDT_FLOAT4      0x03    // 4D float
#define EFF3DVSDT_EFF3DCOLOR    0x04    // 4D packed unsigned bytes mapped to 0. to 1. range
// Input is in EFF3DCOLOR format (ARGB) expanded to (R, G, B, A)
#define EFF3DVSDT_UBYTE4      0x05    // 4D unsigned byte
#define EFF3DVSDT_SHORT2      0x06    // 2D signed short expanded to (value, value, 0., 1.)
#define EFF3DVSDT_SHORT4      0x07    // 4D signed short


/* Lock flags */

#define EFF3DLOCK_READONLY           0x00000010L
#define EFF3DLOCK_DISCARD            0x00002000L
#define EFF3DLOCK_NOOVERWRITE        0x00001000L
#define EFF3DLOCK_NOSYSLOCK          0x00000800L
#define EFF3DLOCK_DONOTWAIT          0x00004000L                  

#define EFF3DLOCK_NO_DIRTY_UPDATE     0x00008000L


typedef enum _EFF3DFILLMODE {
	EFF3DFILL_POINT               = 1,
	EFF3DFILL_WIREFRAME           = 2,
	EFF3DFILL_SOLID               = 3,
	EFF3DFILL_FORCE_DWORD         = 0x7fffffff, /* force 32-bit size enum */
} EFF3DFILLMODE;

typedef enum _EFF3DBACKBUFFER_TYPE
{
	EFF3DBACKBUFFER_TYPE_MONO         = 0,
	EFF3DBACKBUFFER_TYPE_LEFT         = 1,
	EFF3DBACKBUFFER_TYPE_RIGHT        = 2,

	EFF3DBACKBUFFER_TYPE_FORCE_DWORD  = 0x7fffffff
} EFF3DBACKBUFFER_TYPE;


typedef enum _EFF3DSAMPLERSTATETYPE
{
	EFF3DSAMP_ADDRESSU       = 1,  /* EFF3DTEXTUREADDRESS for U coordinate */
	EFF3DSAMP_ADDRESSV       = 2,  /* EFF3DTEXTUREADDRESS for V coordinate */
	EFF3DSAMP_ADDRESSW       = 3,  /* EFF3DTEXTUREADDRESS for W coordinate */
	EFF3DSAMP_BORDERCOLOR    = 4,  /* EFF3DCOLOR */
	EFF3DSAMP_MAGFILTER      = 5,  /* EFF3DTEXTUREFILTER filter to use for magnification */
	EFF3DSAMP_MINFILTER      = 6,  /* EFF3DTEXTUREFILTER filter to use for minification */
	EFF3DSAMP_MIPFILTER      = 7,  /* EFF3DTEXTUREFILTER filter to use between mipmaps during minification */
	EFF3DSAMP_MIPMAPLODBIAS  = 8,  /* float Mipmap LOD bias */
	EFF3DSAMP_MAXMIPLEVEL    = 9,  /* effUINT 0..(n-1) LOD index of largest map to use (0 == largest) */
	EFF3DSAMP_MAXANISOTROPY  = 10, /* effUINT maximum anisotropy */
	EFF3DSAMP_SRGBTEXTURE    = 11, /* Default = 0 (which means Gamma 1.0,no correction required.) else correct for Gamma = 2.2 */
	EFF3DSAMP_ELEMENTINDEX   = 12, /* When multi-element texture is assigned to sampler, this indicates which element index to use.  Default = 0.  */
	EFF3DSAMP_DMAPOFFSET     = 13, /* Offset in vertices in the pre-sampled displacement map. Only valid for EFF3DDMAPSAMPLER sampler  */
	EFF3DSAMP_FORCE_DWORD   = 0x7fffffff, /* force 32-bit size enum */
} EFF3DSAMPLERSTATETYPE;

/* Special sampler which is used in the tesselator */
#define EFF3DDMAPSAMPLER 256

// Samplers used in vertex shaders
#define EFF3DVERTEXTEXTURESAMPLER0 (EFF3DDMAPSAMPLER+1)
#define EFF3DVERTEXTEXTURESAMPLER1 (EFF3DDMAPSAMPLER+2)
#define EFF3DVERTEXTEXTURESAMPLER2 (EFF3DDMAPSAMPLER+3)
#define EFF3DVERTEXTEXTURESAMPLER3 (EFF3DDMAPSAMPLER+4)

// Values, used with EFF3DTSS_TEXCOORDINDEX, to specify that the vertex data(position
// and normal in the camera space) should be taken as texture coordinates
// Low 16 bits are used to specify texture coordinate index, to take the WRAP mode from
//
#define EFF3DTSS_TCI_PASSTHRU                             0x00000000
#define EFF3DTSS_TCI_CAMERASPACENORMAL                    0x00010000
#define EFF3DTSS_TCI_CAMERASPACEPOSITION                  0x00020000
#define EFF3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR          0x00030000
#define EFF3DTSS_TCI_SPHEREMAP                            0x00040000


typedef enum _EFF3DTEXTUREADDRESS {
	EFF3DTADDRESS_WRAP            = 1,
	EFF3DTADDRESS_MIRROR          = 2,
	EFF3DTADDRESS_CLAMP           = 3,
	EFF3DTADDRESS_BORDER          = 4,
	EFF3DTADDRESS_MIRRORONCE      = 5,
	EFF3DTADDRESS_FORCE_DWORD     = 0x7fffffff, /* force 32-bit size enum */
} EFF3DTEXTUREADDRESS;

//
// Values for EFF3DSAMP_***FILTER texture stage states
//
typedef enum _EFF3DTEXTUREFILTERTYPE
{
	EFF3DTEXF_NONE            = 0,    // filtering disabled (valid for mip filter only)
	EFF3DTEXF_POINT           = 1,    // nearest
	EFF3DTEXF_LINEAR          = 2,    // linear interpolation
	EFF3DTEXF_ANISOTROPIC     = 3,    // anisotropic
	EFF3DTEXF_PYRAMIDALQUAD   = 6,    // 4-sample tent
	EFF3DTEXF_GAUSSIANQUAD    = 7,    // 4-sample gaussian
	EFF3DTEXF_FORCE_DWORD     = 0x7fffffff,   // force 32-bit size enum
} EFF3DTEXTUREFILTERTYPE;

/* Bits for Flags in ProcessVertices call */

#define EFF3DPV_DONOTCOPYDATA     (1 << 0)

//---------------------------------------------------------------------
// Vertex Shaders
//

// Vertex shader declaration



// Maximum supported number of texture coordinate sets
#define EFF3DDP_MAXTEXCOORD   8

typedef enum _EFF3DFOGMODE {
	EFF3DFOG_NONE                 = 0,
	EFF3DFOG_EXP                  = 1,
	EFF3DFOG_EXP2                 = 2,
	EFF3DFOG_LINEAR               = 3,
	EFF3DFOG_FORCE_DWORD          = 0x7fffffff, /* force 32-bit size enum */
} EFF3DFOGMODE;

//----------------------------------------------------------------------------
// EFF3DMACRO:
// ----------
// Preprocessor macro definition.  The application pass in a NULL-terminated
// array of this structure to various EFF3D APIs.  This enables the application
// to #define tokens at runtime, before the file is parsed.
//----------------------------------------------------------------------------

typedef struct _EFF3DXMACRO
{
	effLPCSTR Name;
	effLPCSTR Definition;

} EFF3DXMACRO, *LPEFF3DXMACRO;


typedef enum _EFF3DXINCLUDE_TYPE
{
	EFF3DXINC_LOCAL,
	EFF3DXINC_SYSTEM,

	// force 32-bit size enum
	EFF3DXINC_FORCE_DWORD = 0x7fffffff

} EFF3DXINCLUDE_TYPE, *LPEFF3DXINCLUDE_TYPE;

typedef enum _EFF3DRESOURCETYPE 
{
	EFF3DRTYPE_SURFACE = 1,
	EFF3DRTYPE_VOLUME = 2,
	EFF3DRTYPE_TEXTURE = 3,
	EFF3DRTYPE_VOLUMETEXTURE = 4,
	EFF3DRTYPE_CUBETEXTURE = 5,
	EFF3DRTYPE_VERTEXBUFFER = 6,
	EFF3DRTYPE_INDEXBUFFER = 7,
	EFF3DRTYPE_FORCE_DWORD = 0x7fffffff
} EFF3DRESOURCETYPE;

typedef enum _EFF3DIMAGE_FILEFORMAT {
	EFF3DXIFF_BMP = 0,
	EFF3DXIFF_JPG = 1,
	EFF3DXIFF_TGA = 2,
	EFF3DXIFF_PNG = 3,
	EFF3DXIFF_DDS = 4,
	EFF3DXIFF_PPM = 5,
	EFF3DXIFF_DIB = 6,
	EFF3DXIFF_HDR = 7,
	EFF3DXIFF_PFM = 8,
	EFF3DXIFF_FORCE_DWORD = 0x7fffffff
} EFF3DIMAGE_FILEFORMAT;


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





/* Structure for LockRect */
typedef struct _EFF3DLOCKED_RECT
{
	effINT					Pitch;
	effVOID*               pBits;
} EFF3DLOCKED_RECT;

typedef enum _EFF3DCMPFUNC {
	EFF3DCMP_NEVER                = 1,
	EFF3DCMP_LESS                 = 2,
	EFF3DCMP_EQUAL                = 3,
	EFF3DCMP_LESSEQUAL            = 4,
	EFF3DCMP_GREATER              = 5,
	EFF3DCMP_NOTEQUAL             = 6,
	EFF3DCMP_GREATEREQUAL         = 7,
	EFF3DCMP_ALWAYS               = 8,
	EFF3DCMP_FORCE_DWORD          = 0x7fffffff, /* force 32-bit size enum */
} EFF3DCMPFUNC;


//----------------------------------------------------------------------------
// EFF3D_FILTER flags:
// ------------------
//
// A valid filter must contain one of these values:
//
//  EFF3D_FILTER_NONE
//      No scaling or filtering will take place.  Pixels outside the bounds
//      of the source image are assumed to be transparent black.
//  EFF3D_FILTER_POINT
//      Each destination pixel is computed by sampling the nearest pixel
//      from the source image.
//  EFF3D_FILTER_LINEAR
//      Each destination pixel is computed by linearly interpolating between
//      the nearest pixels in the source image.  This filter works best 
//      when the scale on each axis is less than 2.
//  EFF3D_FILTER_TRIANGLE
//      Every pixel in the source image contributes equally to the
//      destination image.  This is the slowest of all the filters.
//  EFF3D_FILTER_BOX
//      Each pixel is computed by averaging a 2x2(x2) box pixels from 
//      the source image. Only works when the dimensions of the 
//      destination are half those of the source. (as with mip maps)
//
// And can be OR'd with any of these optional flags:
//
//  EFF3D_FILTER_MIRROR_U
//      Indicates that pixels off the edge of the texture on the U-axis
//      should be mirrored, not wraped.
//  EFF3D_FILTER_MIRROR_V
//      Indicates that pixels off the edge of the texture on the V-axis
//      should be mirrored, not wraped.
//  EFF3D_FILTER_MIRROR_W
//      Indicates that pixels off the edge of the texture on the W-axis
//      should be mirrored, not wraped.
//  EFF3D_FILTER_MIRROR
//      Same as specifying EFF3D_FILTER_MIRROR_U | EFF3D_FILTER_MIRROR_V |
//      EFF3D_FILTER_MIRROR_V
//  EFF3D_FILTER_DITHER
//      Dithers the resulting image using a 4x4 order dither pattern.
//  EFF3D_FILTER_SRGB_IN
//      Denotes that the input data is in sRGB (gamma 2.2) colorspace.
//  EFF3D_FILTER_SRGB_OUT
//      Denotes that the output data is in sRGB (gamma 2.2) colorspace.
//  EFF3D_FILTER_SRGB
//      Same as specifying EFF3D_FILTER_SRGB_IN | EFF3D_FILTER_SRGB_OUT
//
//----------------------------------------------------------------------------

#define EFF3D_FILTER_NONE             (1 << 0)
#define EFF3D_FILTER_POINT            (2 << 0)
#define EFF3D_FILTER_LINEAR           (3 << 0)
#define EFF3D_FILTER_TRIANGLE         (4 << 0)
#define EFF3D_FILTER_BOX              (5 << 0)

#define EFF3D_FILTER_MIRROR_U         (1 << 16)
#define EFF3D_FILTER_MIRROR_V         (2 << 16)
#define EFF3D_FILTER_MIRROR_W         (4 << 16)
#define EFF3D_FILTER_MIRROR           (7 << 16)

#define EFF3D_FILTER_DITHER           (1 << 19)
#define EFF3D_FILTER_DITHER_DIFFUSION (2 << 19)

#define EFF3D_FILTER_SRGB_IN          (1 << 21)
#define EFF3D_FILTER_SRGB_OUT         (2 << 21)
#define EFF3D_FILTER_SRGB             (3 << 21)

typedef enum _EFF3DTEXTURETRANSFORMFLAGS
{
	EFF3DTTFF_DISABLE         = 0,    // texture coordinates are passed directly
	EFF3DTTFF_COUNT1          = 1,    // rasterizer should expect 1-D texture coords
	EFF3DTTFF_COUNT2          = 2,    // rasterizer should expect 2-D texture coords
	EFF3DTTFF_COUNT3          = 3,    // rasterizer should expect 3-D texture coords
	EFF3DTTFF_COUNT4          = 4,    // rasterizer should expect 4-D texture coords
	EFF3DTTFF_PROJECTED       = 256,  // texcoords to be divided by COUNTth element
	EFF3DTTFF_FORCE_DWORD     = 0x7fffffff,
} EFF3DTEXTURETRANSFORMFLAGS;

typedef struct _EFF3DVIEWPORT
{
	effUINT X;
	effUINT Y;
	effUINT Width;
	effUINT Height;
	effFLOAT MinZ;
	effFLOAT MaxZ;
} EFF3DVIEWPORT;

typedef struct EFF3DMACRO
{
	effLPCSTR Name;
	effLPCSTR Definition;
} EFFD3DMACRO;

typedef enum _EFF3DINCLUDE_TYPE
{
	EFF3DINC_LOCAL = 0,
	EFF3DINC_SYSTEM = 1,
	EFF3DINC_FORCE_DWORD = 0x7fffffff
} EFF3DINCLUDE_TYPE;


#define EFF3DCLIPPLANE0 (1 << 0)
#define EFF3DCLIPPLANE1 (1 << 1)
#define EFF3DCLIPPLANE2 (1 << 2)
#define EFF3DCLIPPLANE3 (1 << 3)
#define EFF3DCLIPPLANE4 (1 << 4)
#define EFF3DCLIPPLANE5 (1 << 5)


/* CubeMap Face identifiers */
typedef enum _EFF3DCUBEMAP_FACES
{
	EFF3DCUBEMAP_FACE_POSITIVE_X     = 0,
	EFF3DCUBEMAP_FACE_NEGATIVE_X     = 1,
	EFF3DCUBEMAP_FACE_POSITIVE_Y     = 2,
	EFF3DCUBEMAP_FACE_NEGATIVE_Y     = 3,
	EFF3DCUBEMAP_FACE_POSITIVE_Z     = 4,
	EFF3DCUBEMAP_FACE_NEGATIVE_Z     = 5,

	EFF3DCUBEMAP_FACE_FORCE_DWORD    = 0x7fffffff
} EFF3DCUBEMAP_FACES;

// Values for material source
typedef enum _EFF3DMATERIALCOLORSOURCE
{
	EFF3DMCS_MATERIAL         = 0,            // Color from material is used
	EFF3DMCS_COLOR1           = 1,            // Diffuse vertex color is used
	EFF3DMCS_COLOR2           = 2,            // Specular vertex color is used
	EFF3DMCS_FORCE_DWORD      = 0x7fffffff,   // force 32-bit size enum
} EFF3DMATERIALCOLORSOURCE;


typedef enum EFF3DQUERYTYPE
{
    EFF3DQUERYTYPE_VCACHE = 4,
    EFF3DQUERYTYPE_ResourceManager = 5,
    EFF3DQUERYTYPE_VERTEXSTATS = 6,
    EFF3DQUERYTYPE_EVENT = 8,
    EFF3DQUERYTYPE_OCCLUSION = 9,
    EFF3DQUERYTYPE_TIMESTAMP = 10,
    EFF3DQUERYTYPE_TIMESTAMPDISJOINT = 11,
    EFF3DQUERYTYPE_TIMESTAMPFREQ = 12,
    EFF3DQUERYTYPE_PIPELINETIMINGS = 13,
    EFF3DQUERYTYPE_INTERFACETIMINGS = 14,
    EFF3DQUERYTYPE_VERTEXTIMINGS = 15,
    EFF3DQUERYTYPE_PIXELTIMINGS = 16,
    EFF3DQUERYTYPE_BANDWIDTHTIMINGS = 17,
    EFF3DQUERYTYPE_CACHEUTILIZATION = 18,
    EFF3DQUERYTYPE_MEMORYPRESSURE = 19
} EFF3DQUERYTYPE, *LPEFF3DQUERYTYPE;

#define EFF3DISSUE_END (1 << 0) // Tells the runtime to issue the end of a query, changing it's state to "non-signaled".
#define EFF3DISSUE_BEGIN (1 << 1) // Tells the runtime to issue the beginng of a query.

/* Surface Description */
struct EFF3DSURFACE_DESC
{
	EFF3DFORMAT           Format;
	EFF3DRESOURCETYPE     Type;
	effUINT               Usage;
	EFF3DPOOL             Pool;

	EFF3DMULTISAMPLE_TYPE MultiSampleType;
	effUINT               MultiSampleQuality;
	effUINT                Width;
	effUINT                Height;
};

typedef struct EFF3DVERTEXELEMENT
{
	effWORD Stream;
	effWORD Offset;
	effBYTE Type;
	effBYTE Method;
	effBYTE Usage;
	effBYTE UsageIndex;
} EFF3DVERTEXELEMENT, *LPEFF3DVERTEXELEMENT;

/*struct EFF3DFont
{
	std::string	Name;
	//UCSize		Size;
	effINT		Weight;
	effBOOL		Italic;
	effUINT		CharSet;
	effUINT		OutputPrecision;
	effUINT		Quality;
	effUINT		PitchAndFamily;
};*/

/* Flags to construct D3DRS_COLORWRITEENABLE */
#define EFF3DCOLORWRITEENABLE_RED     (1L<<0)
#define EFF3DCOLORWRITEENABLE_GREEN   (1L<<1)
#define EFF3DCOLORWRITEENABLE_BLUE    (1L<<2)
#define EFF3DCOLORWRITEENABLE_ALPHA   (1L<<3)



#define FOURCC_DF16 (MAKEFOURCC('D','F','1','6')) 
#define FOURCC_DF24 (MAKEFOURCC('D','F','2','4')) 



#define _FACEFF3D  0x876
#define MAKE_EFF3DHRESULT( code )  MAKE_HRESULT( 1, _FACEFF3D, code )

#define EFF3DERR_DEVICELOST						MAKE_EFF3DHRESULT(2152)
#define EFF3DERR_DEVICENOTRESET					MAKE_EFF3DHRESULT(2153) 
#define EFF3DERR_INVALIDCALL                    MAKE_EFF3DHRESULT(2156)

typedef struct EFF3DINDEXBUFFER_DESC
{
	EFF3DFORMAT Format;
	EFF3DRESOURCETYPE Type;
	effUINT Usage;
	EFF3DPOOL Pool;
	effUINT Size;
} EFF3DINDEXBUFFER_DESC, *LPEFF3DINDEXBUFFER_DESC;


typedef struct EFF3DVERTEXBUFFER_DESC {
	EFF3DFORMAT Format;
	EFF3DRESOURCETYPE Type;
	effUINT Usage;
	EFF3DPOOL Pool;
	effUINT Size;
	effUINT FVF;
} EFF3DVERTEXBUFFER_DESC, *LPEFF3DVERTEXBUFFER_DESC;


#ifndef EFF3D_EXPORTS
	#define EFF3D_API		__declspec(dllimport)
#else
	#define EFF3D_API		__declspec(dllexport)
#endif

//#define from bgfx https://github.com/bkaradzic/bgfx



//EFF_HANDLE(EFF3DTextureHandle)

typedef EFFId EFF3DTextureHandle;



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
    BC1,          //!< DXT1
    BC2,          //!< DXT3
    BC3,          //!< DXT5
    BC4,          //!< LATC1/ATI1
    BC5,          //!< LATC2/ATI2
    BC6H,         //!< BC6H
    BC7,          //!< BC7
    ETC1,         //!< ETC1 RGB8
    ETC2,         //!< ETC2 RGB8
    ETC2A,        //!< ETC2 RGBA8
    ETC2A1,       //!< ETC2 RGB8A1
    PTC12,        //!< PVRTC1 RGB 2BPP
    PTC14,        //!< PVRTC1 RGB 4BPP
    PTC12A,       //!< PVRTC1 RGBA 2BPP
    PTC14A,       //!< PVRTC1 RGBA 4BPP
    PTC22,        //!< PVRTC2 RGBA 2BPP
    PTC24,        //!< PVRTC2 RGBA 4BPP

    EFF3DTextureFormatUnknown,      // Compressed formats above.

    R1,
    A8,
    R8,
    R8I,
    R8U,
    R8S,
    R16,
    R16I,
    R16U,
    R16F,
    R16S,
    R32I,
    R32U,
    R32F,
    RG8,
    RG8I,
    RG8U,
    RG8S,
    RG16,
    RG16I,
    RG16U,
    RG16F,
    RG16S,
    RG32I,
    RG32U,
    RG32F,
    RGB8,
    RGB8I,
    RGB8U,
    RGB8S,
    RGB9E5F,
    BGRA8,
    RGBA8,
    RGBA8I,
    RGBA8U,
    RGBA8S,
    RGBA16,
    RGBA16I,
    RGBA16U,
    RGBA16F,
    RGBA16S,
    RGBA32I,
    RGBA32U,
    RGBA32F,
    R5G6B5,
    RGBA4,
    RGB5A1,
    RGB10A2,
    RG11B10F,

    EFF3DTextureFormatUnknownDepth, // Depth formats below.

    D16,
    D24,
    D24S8,
    D32,
    D16F,
    D24F,
    D32F,
    D0S8,

    EFF3DTextureFormatCount
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
    EFF3DResourceType_VertexBuffer,
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
 
    effULONG textureHandle;
    effUINT textureLevel;

    EFF3DImageInfo()
    {
        width = 0;
        height = 0;
        depth = 0;
        mipLevels = 0;
        format = EFF3DTextureFormatUnknown;
        type = EFF3DResourceType_Texture2D;

        filter = 0;
        mipFilter = 0;

        multiSample = EFF3DMULTISAMPLE_NONE;
        multisampleQuality = 0;
        textureHandle = EFF_INVALID_HANDLE;
        textureLevel = 0;
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