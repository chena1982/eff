/******************************************************************************
	created:	2008-12-2   0:18
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9BaseDefines_2008_12_2__
#define __EFFD3D9BaseDefines_2008_12_2__

#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dxerr9.lib")
#pragma comment (lib, "DxErr.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dxof.lib")

#ifndef EFFD3D9_EXPORTS
    #define EFFD3D9_API		__declspec(dllimport)
#else
    #define EFFD3D9_API		__declspec(dllexport)
#endif

#	if defined(D3D_DISABLE_9EX)
#		define D3DFMT_S8_LOCKABLE D3DFORMAT( 85)
#		define D3DFMT_A1          D3DFORMAT(118)
#	endif // defined(D3D_DISABLE_9EX)

#	ifndef D3DFMT_ATI1
#		define D3DFMT_ATI1 ( (D3DFORMAT)EFF3D_MAKEFOURCC('A', 'T', 'I', '1') )
#	endif // D3DFMT_ATI1

#	ifndef D3DFMT_ATI2
#		define D3DFMT_ATI2 ( (D3DFORMAT)EFF3D_MAKEFOURCC('A', 'T', 'I', '2') )
#	endif // D3DFMT_ATI2

#	ifndef D3DFMT_ATOC
#		define D3DFMT_ATOC ( (D3DFORMAT)EFF3D_MAKEFOURCC('A', 'T', 'O', 'C') )
#	endif // D3DFMT_ATOC

#	ifndef D3DFMT_DF16
#		define D3DFMT_DF16 ( (D3DFORMAT)EFF3D_MAKEFOURCC('D', 'F', '1', '6') )
#	endif // D3DFMT_DF16

#	ifndef D3DFMT_DF24
#		define D3DFMT_DF24 ( (D3DFORMAT)EFF3D_MAKEFOURCC('D', 'F', '2', '4') )
#	endif // D3DFMT_DF24

#	ifndef D3DFMT_INST
#		define D3DFMT_INST ( (D3DFORMAT)EFF3D_MAKEFOURCC('I', 'N', 'S', 'T') )
#	endif // D3DFMT_INST

#	ifndef D3DFMT_INTZ
#		define D3DFMT_INTZ ( (D3DFORMAT)EFF3D_MAKEFOURCC('I', 'N', 'T', 'Z') )
#	endif // D3DFMT_INTZ

#	ifndef D3DFMT_NULL
#		define D3DFMT_NULL ( (D3DFORMAT)EFF3D_MAKEFOURCC('N', 'U', 'L', 'L') )
#	endif // D3DFMT_NULL

#	ifndef D3DFMT_RESZ
#		define D3DFMT_RESZ ( (D3DFORMAT)EFF3D_MAKEFOURCC('R', 'E', 'S', 'Z') )
#	endif // D3DFMT_RESZ

#	ifndef D3DFMT_RAWZ
#		define D3DFMT_RAWZ ( (D3DFORMAT)EFF3D_MAKEFOURCC('R', 'A', 'W', 'Z') )
#	endif // D3DFMT_RAWZ

#	ifndef D3DFMT_S8_LOCKABLE
#		define D3DFMT_S8_LOCKABLE ( (D3DFORMAT)85)
#	endif // D3DFMT_S8_LOCKABLE

#	ifndef D3DFMT_A1
#		define D3DFMT_A1 ( (D3DFORMAT)118)
#	endif // D3DFMT_A1

struct ExtendedFormat
{
    enum Enum
    {
        Ati1,
        Ati2,
        Df16,
        Df24,
        Inst,
        Intz,
        Null,
        Resz,
        Rawz,
        Atoc,

        Count,
    };

    D3DFORMAT m_fmt;
    DWORD m_usage;
    D3DRESOURCETYPE m_type;
    effBOOL m_supported;
};

static const D3DMULTISAMPLE_TYPE s_checkMsaa[] =
{
    D3DMULTISAMPLE_NONE,
    D3DMULTISAMPLE_2_SAMPLES,
    D3DMULTISAMPLE_4_SAMPLES,
    D3DMULTISAMPLE_8_SAMPLES,
    D3DMULTISAMPLE_16_SAMPLES,
};


struct Msaa
{
    D3DMULTISAMPLE_TYPE m_type;
    DWORD m_quality;
};

static Msaa s_msaa[] =
{
    { D3DMULTISAMPLE_NONE,       0 },
    { D3DMULTISAMPLE_2_SAMPLES,  0 },
    { D3DMULTISAMPLE_4_SAMPLES,  0 },
    { D3DMULTISAMPLE_8_SAMPLES,  0 },
    { D3DMULTISAMPLE_16_SAMPLES, 0 },
};

struct Blend
{
    D3DBLEND m_src;
    D3DBLEND m_dst;
    effBOOL m_factor;
};

static const Blend s_blendFactor[] =
{
    { (D3DBLEND)0,             (D3DBLEND)0,             false }, // ignored
    { D3DBLEND_ZERO,           D3DBLEND_ZERO,           false }, // ZERO
    { D3DBLEND_ONE,            D3DBLEND_ONE,            false }, // ONE
    { D3DBLEND_SRCCOLOR,       D3DBLEND_SRCCOLOR,       false }, // SRC_COLOR
    { D3DBLEND_INVSRCCOLOR,    D3DBLEND_INVSRCCOLOR,    false }, // INV_SRC_COLOR
    { D3DBLEND_SRCALPHA,       D3DBLEND_SRCALPHA,       false }, // SRC_ALPHA
    { D3DBLEND_INVSRCALPHA,    D3DBLEND_INVSRCALPHA,    false }, // INV_SRC_ALPHA
    { D3DBLEND_DESTALPHA,      D3DBLEND_DESTALPHA,      false }, // DST_ALPHA
    { D3DBLEND_INVDESTALPHA,   D3DBLEND_INVDESTALPHA,   false }, // INV_DST_ALPHA
    { D3DBLEND_DESTCOLOR,      D3DBLEND_DESTCOLOR,      false }, // DST_COLOR
    { D3DBLEND_INVDESTCOLOR,   D3DBLEND_INVDESTCOLOR,   false }, // INV_DST_COLOR
    { D3DBLEND_SRCALPHASAT,    D3DBLEND_ONE,            false }, // SRC_ALPHA_SAT
    { D3DBLEND_BLENDFACTOR,    D3DBLEND_BLENDFACTOR,    true }, // FACTOR
    { D3DBLEND_INVBLENDFACTOR, D3DBLEND_INVBLENDFACTOR, true }, // INV_FACTOR
};

static const D3DBLENDOP s_blendEquation[] =
{
    D3DBLENDOP_ADD,
    D3DBLENDOP_SUBTRACT,
    D3DBLENDOP_REVSUBTRACT,
    D3DBLENDOP_MIN,
    D3DBLENDOP_MAX,
};

static const D3DCMPFUNC s_cmpFunc[] =
{
    (D3DCMPFUNC)0, // ignored
    D3DCMP_LESS,
    D3DCMP_LESSEQUAL,
    D3DCMP_EQUAL,
    D3DCMP_GREATEREQUAL,
    D3DCMP_GREATER,
    D3DCMP_NOTEQUAL,
    D3DCMP_NEVER,
    D3DCMP_ALWAYS,
};

static const D3DSTENCILOP s_stencilOp[] =
{
    D3DSTENCILOP_ZERO,
    D3DSTENCILOP_KEEP,
    D3DSTENCILOP_REPLACE,
    D3DSTENCILOP_INCR,
    D3DSTENCILOP_INCRSAT,
    D3DSTENCILOP_DECR,
    D3DSTENCILOP_DECRSAT,
    D3DSTENCILOP_INVERT,
};

static const D3DRENDERSTATETYPE s_stencilFuncRs[] =
{
    D3DRS_STENCILFUNC,
    D3DRS_CCW_STENCILFUNC,
};

static const D3DRENDERSTATETYPE s_stencilFailRs[] =
{
    D3DRS_STENCILFAIL,
    D3DRS_CCW_STENCILFAIL,
};

static const D3DRENDERSTATETYPE s_stencilZFailRs[] =
{
    D3DRS_STENCILZFAIL,
    D3DRS_CCW_STENCILZFAIL,
};

static const D3DRENDERSTATETYPE s_stencilZPassRs[] =
{
    D3DRS_STENCILPASS,
    D3DRS_CCW_STENCILPASS,
};

static const D3DCULL s_cullMode[] =
{
    D3DCULL_NONE,
    D3DCULL_CW,
    D3DCULL_CCW,
};

static const D3DTEXTUREADDRESS s_textureAddress[] =
{
    D3DTADDRESS_WRAP,
    D3DTADDRESS_MIRROR,
    D3DTADDRESS_CLAMP,
    D3DTADDRESS_BORDER,
};

static const D3DTEXTUREFILTERTYPE s_textureFilter[] =
{
    D3DTEXF_LINEAR,
    D3DTEXF_POINT,
    D3DTEXF_ANISOTROPIC,
};

struct TextureFormatInfo
{
    D3DFORMAT m_fmt;
};

static TextureFormatInfo s_textureFormat[] =
{
    { D3DFMT_DXT1 }, // BC1
    { D3DFMT_DXT3 }, // BC2
    { D3DFMT_DXT5 }, // BC3
    { D3DFMT_UNKNOWN }, // BC4
    { D3DFMT_UNKNOWN }, // BC5
    { D3DFMT_UNKNOWN }, // BC6H
    { D3DFMT_UNKNOWN }, // BC7
    { D3DFMT_UNKNOWN }, // ETC1
    { D3DFMT_UNKNOWN }, // ETC2
    { D3DFMT_UNKNOWN }, // ETC2A
    { D3DFMT_UNKNOWN }, // ETC2A1
    { D3DFMT_UNKNOWN }, // PTC12
    { D3DFMT_UNKNOWN }, // PTC14
    { D3DFMT_UNKNOWN }, // PTC12A
    { D3DFMT_UNKNOWN }, // PTC14A
    { D3DFMT_UNKNOWN }, // PTC22
    { D3DFMT_UNKNOWN }, // PTC24
    { D3DFMT_UNKNOWN }, // Unknown
    { D3DFMT_A1 }, // R1
    { D3DFMT_A8 }, // A8
    { D3DFMT_L8 }, // R8
    { D3DFMT_UNKNOWN }, // R8I
    { D3DFMT_UNKNOWN }, // R8U
    { D3DFMT_UNKNOWN }, // R8S
    { D3DFMT_L16 }, // R16
    { D3DFMT_UNKNOWN }, // R16I
    { D3DFMT_UNKNOWN }, // R16U
    { D3DFMT_R16F }, // R16F
    { D3DFMT_UNKNOWN }, // R16S
    { D3DFMT_UNKNOWN }, // R32I
    { D3DFMT_UNKNOWN }, // R32U
    { D3DFMT_R32F }, // R32F
    { D3DFMT_A8L8 }, // RG8
    { D3DFMT_UNKNOWN }, // RG8I
    { D3DFMT_UNKNOWN }, // RG8U
    { D3DFMT_UNKNOWN }, // RG8S
    { D3DFMT_G16R16 }, // RG16
    { D3DFMT_UNKNOWN }, // RG16I
    { D3DFMT_UNKNOWN }, // RG16U
    { D3DFMT_G16R16F }, // RG16F
    { D3DFMT_UNKNOWN }, // RG16S
    { D3DFMT_UNKNOWN }, // RG32I
    { D3DFMT_UNKNOWN }, // RG32U
    { D3DFMT_G32R32F }, // RG32F
    { D3DFMT_UNKNOWN }, // RGB8
    { D3DFMT_UNKNOWN }, // RGB8I
    { D3DFMT_UNKNOWN }, // RGB8U
    { D3DFMT_UNKNOWN }, // RGB8S
    { D3DFMT_UNKNOWN }, // RGB9E5F
    { D3DFMT_A8R8G8B8 }, // BGRA8
    { D3DFMT_UNKNOWN }, // RGBA8
    { D3DFMT_UNKNOWN }, // RGBA8I
    { D3DFMT_UNKNOWN }, // RGBA8U
    { D3DFMT_UNKNOWN }, // RGBA8S
    { D3DFMT_A16B16G16R16 }, // RGBA16
    { D3DFMT_UNKNOWN }, // RGBA16I
    { D3DFMT_UNKNOWN }, // RGBA16U
    { D3DFMT_A16B16G16R16F }, // RGBA16F
    { D3DFMT_UNKNOWN }, // RGBA16S
    { D3DFMT_UNKNOWN }, // RGBA32I
    { D3DFMT_UNKNOWN }, // RGBA32U
    { D3DFMT_A32B32G32R32F }, // RGBA32F
    { D3DFMT_R5G6B5 }, // R5G6B5
    { D3DFMT_A4R4G4B4 }, // RGBA4
    { D3DFMT_A1R5G5B5 }, // RGB5A1
    { D3DFMT_A2B10G10R10 }, // RGB10A2
    { D3DFMT_UNKNOWN }, // RG11B10F
    { D3DFMT_UNKNOWN }, // UnknownDepth
    { D3DFMT_D16 }, // D16
    { D3DFMT_D24X8 }, // D24
    { D3DFMT_D24S8 }, // D24S8
    { D3DFMT_D32 }, // D32
    { D3DFMT_DF16 }, // D16F
    { D3DFMT_DF24 }, // D24F
    { D3DFMT_D32F_LOCKABLE }, // D32F
    { D3DFMT_S8_LOCKABLE }, // D0S8
};


static ExtendedFormat s_extendedFormats[ExtendedFormat::Count] =
{
    { D3DFMT_ATI1, 0,                     D3DRTYPE_TEXTURE, false },
    { D3DFMT_ATI2, 0,                     D3DRTYPE_TEXTURE, false },
    { D3DFMT_DF16, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, false },
    { D3DFMT_DF24, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, false },
    { D3DFMT_INST, 0,                     D3DRTYPE_SURFACE, false },
    { D3DFMT_INTZ, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, false },
    { D3DFMT_NULL, D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, false },
    { D3DFMT_RESZ, D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, false },
    { D3DFMT_RAWZ, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, false },
    { D3DFMT_ATOC, 0,                     D3DRTYPE_SURFACE, false },
};


#endif