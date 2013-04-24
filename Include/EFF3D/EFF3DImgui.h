/******************************************************************************
	created:	2013-4-20   15:44
	file path:	d:\EFF\Include\EFF3D\EFF3DImgui.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DImgui_2013_4_20__
#define __EFF3DImgui_2013_4_20__

EFF3D_BEGIN

enum imguiMouseButton
{
	IMGUI_MBUT_LEFT = 0x01, 
	IMGUI_MBUT_RIGHT = 0x02, 
};

enum imguiTextAlign
{
	IMGUI_ALIGN_LEFT,
	IMGUI_ALIGN_CENTER,
	IMGUI_ALIGN_RIGHT,
};

inline effUINT imguiRGBA(effBYTE r, effBYTE g, effBYTE b, effBYTE a=255)
{
	return (r) | (g << 8) | (b << 16) | (a << 24);
}

EFF3D_API effVOID imguiBeginFrame(effINT mx, effINT my, effBYTE mbut, effINT scroll);
EFF3D_API effVOID imguiEndFrame();

EFF3D_API effBOOL imguiBeginScrollArea(const effWCHAR * name, effINT x, effINT y, effINT w, effINT h, effINT* scroll);
EFF3D_API effVOID imguiEndScrollArea();

effVOID imguiIndent();
effVOID imguiUnindent();
effVOID imguiSeparator();
effVOID imguiSeparatorLine();

EFF3D_API effBOOL imguiButton(const effWCHAR * text, effBOOL enabled = effTRUE);
effBOOL imguiItem(const effWCHAR * text, effBOOL enabled = effTRUE);
effBOOL imguiCheck(const effWCHAR * text, effBOOL checked, effBOOL enabled = effTRUE);
effBOOL imguiCollapse(const effWCHAR * text, const effCHAR * subtext, effBOOL checked, effBOOL enabled = effTRUE);
effVOID imguiLabel(const effWCHAR * text);
effVOID imguiValue(const effWCHAR * text);
effBOOL imguiSlider(const effWCHAR * text, effFLOAT* val, effFLOAT vmin, effFLOAT vmax, effFLOAT vinc, effBOOL enabled = effTRUE);

effVOID imguiDrawText(effINT x, effINT y, effINT align, const effWCHAR * text, effUINT color);
effVOID imguiDrawLine(effFLOAT x0, effFLOAT y0, effFLOAT x1, effFLOAT y1, effFLOAT r, effUINT color);
effVOID imguiDrawRoundedRect(effFLOAT x, effFLOAT y, effFLOAT w, effFLOAT h, effFLOAT r, effUINT color);
effVOID imguiDrawRect(effFLOAT x, effFLOAT y, effFLOAT w, effFLOAT h, effUINT color);

// Pull render interface.
enum imguiGfxCmdType
{
	IMGUI_GFXCMD_RECT,
	IMGUI_GFXCMD_TRIANGLE,
	IMGUI_GFXCMD_LINE,
	IMGUI_GFXCMD_TEXT,
	IMGUI_GFXCMD_SCISSOR,
};

struct imguiGfxRect
{
	effSHORT x,y,w,h,r;
};

struct imguiGfxText
{
	effSHORT x,y,align;
	const effWCHAR * text;
};

struct imguiGfxLine
{
	effSHORT x0,y0,x1,y1,r;
};

struct imguiGfxCmd
{
	effCHAR type;
	effCHAR flags;
	effCHAR pad[2];
	effUINT col;
	union
	{
		imguiGfxLine line;
		imguiGfxRect rect;
		imguiGfxText text;
	};
};

const imguiGfxCmd* imguiGetRenderQueue();
effINT imguiGetRenderQueueSize();



EFF3D_END

#endif __EFF3DImgui_2013_4_20__