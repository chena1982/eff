/******************************************************************************
	created:	2013-4-20   15:46
	file path:	d:\EFF\Src\EFF3D\EFF3DImgui.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DImgui.h"



EFF3D_BEGIN

static const effINT TEXT_POOL_SIZE = 8000;
static effWCHAR g_textPool[TEXT_POOL_SIZE];
static effINT g_textPoolSize = 0;
static const effINT OBJECT_COUNT  = 10000;	

static const effWCHAR * allocText(const effWCHAR * text)
{
	effINT len = (effINT)((wcslen(text) + 1) * sizeof(effWCHAR));

	if ( g_textPoolSize + len >= TEXT_POOL_SIZE )
	{
		return 0;
	}

	effWCHAR * dst = &g_textPool[g_textPoolSize]; 
	memcpy(dst, text, len);
	g_textPoolSize += len;
	return dst;
}

static const effINT GFXCMD_QUEUE_SIZE = 5000;
static imguiGfxCmd g_gfxCmdQueue[GFXCMD_QUEUE_SIZE];
static effINT g_gfxCmdQueueSize = 0;

static effVOID resetGfxCmdQueue()
{
	g_gfxCmdQueueSize = 0;
	g_textPoolSize = 0;
}

static effVOID addGfxCmdScissor(effINT x, effINT y, effINT w, effINT h)
{
	if ( g_gfxCmdQueueSize >= GFXCMD_QUEUE_SIZE )
	{
		return;
	}

	imguiGfxCmd & cmd = g_gfxCmdQueue[g_gfxCmdQueueSize++];
	cmd.type = IMGUI_GFXCMD_SCISSOR;
	cmd.flags = x < 0 ? 0 : 1;	// on/off flag.
	cmd.col = 0;
	cmd.rect.x = (effSHORT)x;
	cmd.rect.y = (effSHORT)y;
	cmd.rect.w = (effSHORT)w;
	cmd.rect.h = (effSHORT)h;
}

static effVOID addGfxCmdRect(effFLOAT x, effFLOAT y, effFLOAT w, effFLOAT h, effUINT color)
{
	if ( g_gfxCmdQueueSize >= GFXCMD_QUEUE_SIZE )
	{
		return;
	}

	imguiGfxCmd & cmd = g_gfxCmdQueue[g_gfxCmdQueueSize++];
	cmd.type = IMGUI_GFXCMD_RECT;
	cmd.flags = 0;
	cmd.col = color;
	cmd.rect.x = (effSHORT)(x*8.0f);
	cmd.rect.y = (effSHORT)(y*8.0f);
	cmd.rect.w = (effSHORT)(w*8.0f);
	cmd.rect.h = (effSHORT)(h*8.0f);
	cmd.rect.r = 0;
}

static effVOID addGfxCmdLine(effFLOAT x0, effFLOAT y0, effFLOAT x1, effFLOAT y1, effFLOAT r, effUINT color)
{
	if ( g_gfxCmdQueueSize >= GFXCMD_QUEUE_SIZE )
	{
		return;
	}

	imguiGfxCmd & cmd = g_gfxCmdQueue[g_gfxCmdQueueSize++];
	cmd.type = IMGUI_GFXCMD_LINE;
	cmd.flags = 0;
	cmd.col = color;
	cmd.line.x0 = (effSHORT)(x0*8.0f);
	cmd.line.y0 = (effSHORT)(y0*8.0f);
	cmd.line.x1 = (effSHORT)(x1*8.0f);
	cmd.line.y1 = (effSHORT)(y1*8.0f);
	cmd.line.r = (effSHORT)(r*8.0f);
}

static effVOID addGfxCmdRoundedRect(effFLOAT x, effFLOAT y, effFLOAT w, effFLOAT h, effFLOAT r, effUINT color)
{
	if ( g_gfxCmdQueueSize >= GFXCMD_QUEUE_SIZE )
	{
		return;
	}

	imguiGfxCmd & cmd = g_gfxCmdQueue[g_gfxCmdQueueSize++];
	cmd.type = IMGUI_GFXCMD_RECT;
	cmd.flags = 0;
	cmd.col = color;
	cmd.rect.x = (effSHORT)(x*8.0f);
	cmd.rect.y = (effSHORT)(y*8.0f);
	cmd.rect.w = (effSHORT)(w*8.0f);
	cmd.rect.h = (effSHORT)(h*8.0f);
	cmd.rect.r = (effSHORT)(r*8.0f);
}

static effVOID addGfxCmdTriangle(effINT x, effINT y, effINT w, effINT h, effINT flags, effUINT color)
{
	if ( g_gfxCmdQueueSize >= GFXCMD_QUEUE_SIZE )
	{
		return;
	}

	imguiGfxCmd & cmd = g_gfxCmdQueue[g_gfxCmdQueueSize++];
	cmd.type = IMGUI_GFXCMD_TRIANGLE;
	cmd.flags = (effCHAR)flags;
	cmd.col = color;
	cmd.rect.x = (effSHORT)(x*8.0f);
	cmd.rect.y = (effSHORT)(y*8.0f);
	cmd.rect.w = (effSHORT)(w*8.0f);
	cmd.rect.h = (effSHORT)(h*8.0f);
}

static effVOID addGfxCmdText(effINT x, effINT y, effINT align, const effWCHAR * text, effUINT color)
{
	if ( g_gfxCmdQueueSize >= GFXCMD_QUEUE_SIZE )
	{
		return;
	}

	imguiGfxCmd & cmd = g_gfxCmdQueue[g_gfxCmdQueueSize++];
	cmd.type = IMGUI_GFXCMD_TEXT;
	cmd.flags = 0;
	cmd.col = color;
	cmd.text.x = (effSHORT)x;
	cmd.text.y = (effSHORT)y;
	cmd.text.align = (effSHORT)align;
	cmd.text.text = allocText(text);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct GuiState
{
	GuiState() :
		left(effFALSE), leftPressed(effFALSE), leftReleased(effFALSE),
		mx(-1), my(-1), scroll(0),
		active(0), hot(0), hotToBe(0), isHot(effFALSE), isActive(effFALSE), wentActive(effFALSE),
		dragX(0), dragY(0), dragOrig(0), widgetX(0), widgetY(0), widgetW(100),
		insideCurrentScroll(effFALSE),  areaId(0), widgetId(0), treeDepth(-1)
	{
		memset(treeNodeChecked, 0, sizeof(effBOOL) * OBJECT_COUNT);
	}

	effBOOL left;
	effBOOL leftPressed, leftReleased;
	effINT mx,my;
	effINT scroll;
	effUINT active;
	effUINT hot;
	effUINT hotToBe;
	effBOOL isHot;
	effBOOL isActive;
	effBOOL wentActive;
	effINT dragX, dragY;
	effFLOAT dragOrig;
	effINT widgetX, widgetY, widgetW;
	effBOOL insideCurrentScroll;
	
	effUINT areaId;
	effUINT widgetId;

	effINT	treeDepth;
	effBOOL	treeNodeChecked[OBJECT_COUNT];
};

static GuiState g_state;

inline effBOOL anyActive()
{
	return g_state.active != 0;
}

inline effBOOL isActive(effUINT id)
{
	return g_state.active == id;
}

inline effBOOL isHot(effUINT id)
{
	return g_state.hot == id;
}

inline effBOOL inRect(effINT x, effINT y, effINT w, effINT h, effBOOL checkScroll = effTRUE)
{
   return (!checkScroll || g_state.insideCurrentScroll) && g_state.mx >= x && g_state.mx <= x+w && g_state.my >= y && g_state.my <= y+h;
}

inline effVOID clearInput()
{
	g_state.leftPressed = effFALSE;
	g_state.leftReleased = effFALSE;
	g_state.scroll = 0;
}

inline effVOID clearActive()
{
	g_state.active = 0;
	// mark all UI for this frame as processed
	clearInput();
}

inline effVOID setActive(effUINT id)
{
	g_state.active = id;
	g_state.wentActive = effTRUE;
}

inline effVOID setHot(effUINT id)
{
   g_state.hotToBe = id;
}


static effBOOL buttonLogic(effUINT id, effBOOL over)
{
	effBOOL res = effFALSE;
	// process down
	if ( !anyActive() )
	{
		if ( over )
		{
			setHot(id);
		}

		if ( isHot(id) && g_state.leftPressed )
		{
			setActive(id);
		}
	}

	// if button is active, then react on left up
	if ( isActive(id) )
	{
		g_state.isActive = effTRUE;

		if ( over )
		{
			setHot(id);
		}

		if ( g_state.leftReleased )
		{
			if ( isHot(id) )
			{
				res = effTRUE;
			}
			clearActive();
		}
	}

	if ( isHot(id) )
	{
		g_state.isHot = effTRUE;
	}

	return res;
}

static effVOID updateInput(effINT mx, effINT my, effBOOL leftButtonDown, effINT scroll)
{
	effBOOL left = leftButtonDown;

	g_state.mx = mx;
	g_state.my = my;
	g_state.leftPressed = !g_state.left && left;
	g_state.leftReleased = g_state.left && !left;
	g_state.left = left;

	g_state.scroll = scroll;
}

effVOID imguiBeginFrame(effINT mx, effINT my, effBOOL mbut, effINT scroll)
{
	updateInput(mx,my,mbut,scroll);

	g_state.hot = g_state.hotToBe;
	g_state.hotToBe = 0;

	g_state.wentActive = effFALSE;
	g_state.isActive = effFALSE;
	g_state.isHot = effFALSE;

	g_state.widgetX = 0;
	g_state.widgetY = 0;
	g_state.widgetW = 0;

	g_state.areaId = 1;
	g_state.widgetId = 1;

	resetGfxCmdQueue();
}

effVOID imguiEndFrame()
{
	clearInput();
}

const imguiGfxCmd* imguiGetRenderQueue()
{
	return g_gfxCmdQueue;
}

effINT imguiGetRenderQueueSize()
{
	return g_gfxCmdQueueSize;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const effINT BUTTON_HEIGHT = 20;
static const effINT SLIDER_HEIGHT = 20;
static const effINT SLIDER_MARKER_WIDTH = 10;
static const effINT CHECK_SIZE = 8;
static const effINT DEFAULT_SPACING = 0;
static const effINT TEXT_HEIGHT = 16;
static const effINT SCROLL_AREA_PADDING = 6;
static const effINT INDENT_SIZE = 16;
static const effINT AREA_HEADER = 28;
static const effINT	TREE_INDENT = 20;

static effINT g_scrollTop = 0;
static effINT g_scrollBottom = 0;
static effINT g_scrollRight = 0;
static effINT g_scrollAreaTop = 0;
static effINT* g_scrollVal = 0;
static effINT g_focusTop = 0;
static effINT g_focusBottom = 0;
static effUINT g_scrollId = 0;
static effBOOL g_insideScrollArea = effFALSE;

effBOOL imguiBeginScrollArea(const effWCHAR * name, effINT x, effINT y, effINT w, effINT h, effINT* scroll)
{
	g_state.areaId++;
	g_state.widgetId = 0;
	g_scrollId = (g_state.areaId<<16) | g_state.widgetId;

	g_state.widgetX = x + SCROLL_AREA_PADDING;
	g_state.widgetY = y + AREA_HEADER + (*scroll);
	g_state.widgetW = w - SCROLL_AREA_PADDING * 4;

	g_scrollTop = y + AREA_HEADER;
	g_scrollBottom = y + h - SCROLL_AREA_PADDING;
	g_scrollRight = x + w - SCROLL_AREA_PADDING * 3;
	g_scrollVal = scroll;

	g_scrollAreaTop = g_state.widgetY;

	g_focusTop = y + AREA_HEADER;
	g_focusBottom = y + h - AREA_HEADER;

	g_insideScrollArea = inRect(x, y, w, h, effFALSE);
	g_state.insideCurrentScroll = g_insideScrollArea;

	addGfxCmdRoundedRect((effFLOAT)x, (effFLOAT)y, (effFLOAT)w, (effFLOAT)h, 6, imguiRGBA(19,19,20,192));

	addGfxCmdText(x+AREA_HEADER/2, y+TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, name, imguiRGBA(255,255,255,128));

	addGfxCmdScissor(x+SCROLL_AREA_PADDING, y+SCROLL_AREA_PADDING, w-SCROLL_AREA_PADDING*4, h-AREA_HEADER-SCROLL_AREA_PADDING);

	return g_insideScrollArea;
}

effVOID imguiEndScrollArea()
{
	// Disable scissoring.
	addGfxCmdScissor(-1, -1, -1, -1);

	// Draw scroll bar
	effINT x = g_scrollRight+SCROLL_AREA_PADDING/2;
	effINT y = g_scrollTop;
	effINT w = SCROLL_AREA_PADDING*2;
	effINT h = g_scrollBottom - g_scrollTop;

	effINT stop = g_scrollBottom;
	effINT sbot = g_state.widgetY;
	effINT sh = stop - sbot; // The scrollable area height.

	effFLOAT barHeight = (effFLOAT)h/(effFLOAT)sh;
	
	if ( barHeight < 1 )
	{
		effFLOAT barY = (effFLOAT)(sbot - y)/(effFLOAT)sh;
		if (barY < 0) barY = 0;
		if (barY > 1) barY = 1;
		
		// Handle scroll bar logic.
		effUINT hid = g_scrollId;
		effINT hx = x;
		effINT hy = y + (effINT)(barY*h);
		effINT hw = w;
		effINT hh = (effINT)(barHeight*h);
		
		const effINT range = h - (hh-1);
		effBOOL over = inRect(hx, hy, hw, hh);
		buttonLogic(hid, over);
		if ( isActive(hid) )
		{
			effFLOAT u = (effFLOAT)(hy-y) / (effFLOAT)range;
			if ( g_state.wentActive )
			{
				g_state.dragY = g_state.my;
				g_state.dragOrig = u;
			}

			if ( g_state.dragY != g_state.my )
			{
				u = g_state.dragOrig + (g_state.my - g_state.dragY) / (effFLOAT)range;
				if (u < 0) u = 0;
				if (u > 1) u = 1;
				*g_scrollVal = (effINT)((1-u) * (sh - h));
			}
		}
		
		// BG
		addGfxCmdRoundedRect((effFLOAT)x, (effFLOAT)y, (effFLOAT)w, (effFLOAT)h, (effFLOAT)w/2-1, imguiRGBA(0,0,0,196));
		// Bar
		if (isActive(hid))
		{
			addGfxCmdRoundedRect((effFLOAT)hx, (effFLOAT)hy, (effFLOAT)hw, (effFLOAT)hh, (effFLOAT)w/2-1, imguiRGBA(255,196,0,196));
		}
		else
		{
			addGfxCmdRoundedRect((effFLOAT)hx, (effFLOAT)hy, (effFLOAT)hw, (effFLOAT)hh, (effFLOAT)w/2-1, isHot(hid) ? imguiRGBA(255,196,0,96) : imguiRGBA(255,255,255,64));
		}

		// Handle mouse scrolling.
		if ( g_insideScrollArea ) // && !anyActive())
		{
			if ( g_state.scroll )
			{
				*g_scrollVal += 20*g_state.scroll;
				if (*g_scrollVal < 0) *g_scrollVal = 0;
				if (*g_scrollVal > (sh - h)) *g_scrollVal = (sh - h);
			}
		}
	}

	g_state.insideCurrentScroll = effFALSE;
}

effBOOL imguiButton(const effWCHAR * text, effBOOL enabled)
{
	g_state.widgetId++;
	effUINT id = (g_state.areaId<<16) | g_state.widgetId;
	
	effINT x = g_state.widgetX;
	effINT y = g_state.widgetY;
	//effINT w = g_state.widgetW;
	effINT w = 100;
	effINT h = BUTTON_HEIGHT;
	g_state.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	effBOOL over = enabled && inRect(x, y, w, h);
	effBOOL res = buttonLogic(id, over);

	addGfxCmdRoundedRect((effFLOAT)x, (effFLOAT)y, (effFLOAT)w, (effFLOAT)h, (effFLOAT)BUTTON_HEIGHT/2-1, imguiRGBA(128,128,128, isActive(id)?196:96));
	//addGfxCmdRoundedRect((effFLOAT)x, (effFLOAT)y, (effFLOAT)w, (effFLOAT)h, 6.0f, imguiRGBA(60,60,60, isActive(id)?196:96));
	//addGfxCmdRect((effFLOAT)x, (effFLOAT)y, (effFLOAT)w, (effFLOAT)h, imguiRGBA(128,128,128, isActive(id)?196:96));

	if ( enabled )
	{
		addGfxCmdText(x+BUTTON_HEIGHT/2, y+BUTTON_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, isHot(id) ? imguiRGBA(255,196,0,255) : imguiRGBA(255,255,255,200));
	}
	else
	{
		addGfxCmdText(x+BUTTON_HEIGHT/2, y+BUTTON_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, imguiRGBA(128,128,128,200));
	}

	return res;
}

effBOOL imguiItem(const effWCHAR * text, effBOOL enabled)
{
	g_state.widgetId++;
	effUINT id = (g_state.areaId<<16) | g_state.widgetId;
	
	effINT x = g_state.widgetX;
	effINT y = g_state.widgetY - BUTTON_HEIGHT;
	effINT w = g_state.widgetW;
	effINT h = BUTTON_HEIGHT;
	g_state.widgetY -= BUTTON_HEIGHT + DEFAULT_SPACING;
	
	effBOOL over = enabled && inRect(x, y, w, h);
	effBOOL res = buttonLogic(id, over);
	
	if (isHot(id))
	{
		addGfxCmdRoundedRect((effFLOAT)x, (effFLOAT)y, (effFLOAT)w, (effFLOAT)h, 2.0f, imguiRGBA(255,196,0,isActive(id)?196:96));
	}

	if (enabled)
	{
		addGfxCmdText(x+BUTTON_HEIGHT/2, y+BUTTON_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, imguiRGBA(255,255,255,200));
	}
	else
	{
		addGfxCmdText(x+BUTTON_HEIGHT/2, y+BUTTON_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, imguiRGBA(128,128,128,200));
	}
	
	return res;
}

effBOOL imguiCheck(const effWCHAR * text, effBOOL checked, effBOOL enabled)
{
	g_state.widgetId++;
	effUINT id = (g_state.areaId<<16) | g_state.widgetId;
	
	effINT x = g_state.widgetX;
	effINT y = g_state.widgetY;
	effINT w = g_state.widgetW;
	effINT h = BUTTON_HEIGHT;
	g_state.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	effBOOL over = enabled && inRect(x, y, w, h);
	effBOOL res = buttonLogic(id, over);
	
	const effINT cx = x+BUTTON_HEIGHT/2-CHECK_SIZE/2;
	const effINT cy = y+BUTTON_HEIGHT/2-CHECK_SIZE/2;
	addGfxCmdRoundedRect((effFLOAT)cx-3, (effFLOAT)cy-3, (effFLOAT)CHECK_SIZE+6, (effFLOAT)CHECK_SIZE+6, 4, imguiRGBA(128,128,128, isActive(id)?196:96));
	if ( checked )
	{
		if ( enabled )
		{
			addGfxCmdRoundedRect((effFLOAT)cx, (effFLOAT)cy, (effFLOAT)CHECK_SIZE, (effFLOAT)CHECK_SIZE, (effFLOAT)CHECK_SIZE/2-1, imguiRGBA(255,255,255,isActive(id)?255:200));
		}
		else
		{
			addGfxCmdRoundedRect((effFLOAT)cx, (effFLOAT)cy, (effFLOAT)CHECK_SIZE, (effFLOAT)CHECK_SIZE, (effFLOAT)CHECK_SIZE/2-1, imguiRGBA(128,128,128,200));
		}
	}


	if ( text != NULL )
	{
		if ( enabled )
		{
			addGfxCmdText(x+BUTTON_HEIGHT, y+BUTTON_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, isHot(id) ? imguiRGBA(255,196,0,255) : imguiRGBA(255,255,255,200));
		}
		else
		{
			addGfxCmdText(x+BUTTON_HEIGHT, y+BUTTON_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, imguiRGBA(128,128,128,200));
		}
	}

	return res;
}

effBOOL imguiCollapse(const effWCHAR * text, const effWCHAR * subtext, effBOOL checked, effBOOL enabled)
{
	g_state.widgetId++;
	effUINT id = (g_state.areaId<<16) | g_state.widgetId;
	
	effINT x = g_state.widgetX;
	effINT y = g_state.widgetY - BUTTON_HEIGHT;
	effINT w = g_state.widgetW;
	effINT h = BUTTON_HEIGHT;
	g_state.widgetY -= BUTTON_HEIGHT; // + DEFAULT_SPACING;

	const effINT cx = x+BUTTON_HEIGHT/2-CHECK_SIZE/2;
	const effINT cy = y+BUTTON_HEIGHT/2-CHECK_SIZE/2;

	effBOOL over = enabled && inRect(x, y, w, h);
	effBOOL res = buttonLogic(id, over);
	
	if ( checked )
	{
		addGfxCmdTriangle(cx, cy, CHECK_SIZE, CHECK_SIZE, 2, imguiRGBA(255,255,255,isActive(id)?255:200));
	}
	else
	{
		addGfxCmdTriangle(cx, cy, CHECK_SIZE, CHECK_SIZE, 1, imguiRGBA(255,255,255,isActive(id)?255:200));
	}

	if ( enabled )
	{
		addGfxCmdText(x+BUTTON_HEIGHT, y+BUTTON_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, isHot(id) ? imguiRGBA(255,196,0,255) : imguiRGBA(255,255,255,200));
	}
	else
	{
		addGfxCmdText(x+BUTTON_HEIGHT, y+BUTTON_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, imguiRGBA(128,128,128,200));
	}

	if ( subtext )
	{
		addGfxCmdText(x+w-BUTTON_HEIGHT/2, y+BUTTON_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_RIGHT, subtext, imguiRGBA(255,255,255,128));
	}
	
	return res;
}

effVOID imguiLabel(const effWCHAR * text)
{
	effINT x = g_state.widgetX;
	effINT y = g_state.widgetY - BUTTON_HEIGHT;
	g_state.widgetY -= BUTTON_HEIGHT;
	addGfxCmdText(x, y+BUTTON_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, imguiRGBA(255,255,255,255));
}

effVOID imguiValue(const effWCHAR * text)
{
	const effINT x = g_state.widgetX;
	const effINT y = g_state.widgetY - BUTTON_HEIGHT;
	const effINT w = g_state.widgetW;
	g_state.widgetY -= BUTTON_HEIGHT;
	
	addGfxCmdText(x+w-BUTTON_HEIGHT/2, y+BUTTON_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_RIGHT, text, imguiRGBA(255,255,255,200));
}

effBOOL imguiSlider(const effWCHAR * text, effFLOAT* val, effFLOAT vmin, effFLOAT vmax, effFLOAT vinc, effBOOL enabled)
{
	g_state.widgetId++;
	effUINT id = (g_state.areaId<<16) | g_state.widgetId;
	
	effINT x = g_state.widgetX;
	effINT y = g_state.widgetY - BUTTON_HEIGHT;
	effINT w = g_state.widgetW;
	effINT h = SLIDER_HEIGHT;
	g_state.widgetY -= SLIDER_HEIGHT + DEFAULT_SPACING;

	addGfxCmdRoundedRect((effFLOAT)x, (effFLOAT)y, (effFLOAT)w, (effFLOAT)h, 4.0f, imguiRGBA(0,0,0,128));

	const effINT range = w - SLIDER_MARKER_WIDTH;

	effFLOAT u = (*val - vmin) / (vmax-vmin);
	if (u < 0) u = 0;
	if (u > 1) u = 1;
	effINT m = (effINT)(u * range);

	effBOOL over = enabled && inRect(x+m, y, SLIDER_MARKER_WIDTH, SLIDER_HEIGHT);
	effBOOL res = buttonLogic(id, over);
	effBOOL valChanged = effFALSE;

	if ( isActive(id) )
	{
		if ( g_state.wentActive )
		{
			g_state.dragX = g_state.mx;
			g_state.dragOrig = u;
		}

		if ( g_state.dragX != g_state.mx )
		{
			u = g_state.dragOrig + (effFLOAT)(g_state.mx - g_state.dragX) / (effFLOAT)range;
			if (u < 0) u = 0;
			if (u > 1) u = 1;
			*val = vmin + u*(vmax-vmin);
			*val = floorf(*val/vinc+0.5f)*vinc; // Snap to vinc
			m = (effINT)(u * range);
			valChanged = effTRUE;
		}
	}

	if ( isActive(id) )
	{
		addGfxCmdRoundedRect((effFLOAT)(x+m), (effFLOAT)y, (effFLOAT)SLIDER_MARKER_WIDTH, (effFLOAT)SLIDER_HEIGHT, 4.0f, imguiRGBA(255,255,255,255));
	}
	else
	{
		addGfxCmdRoundedRect((effFLOAT)(x+m), (effFLOAT)y, (effFLOAT)SLIDER_MARKER_WIDTH, (effFLOAT)SLIDER_HEIGHT, 4.0f, isHot(id) ? imguiRGBA(255,196,0,128) : imguiRGBA(255,255,255,64));
	}

	// TODO: fix this, take a look at 'nicenum'.
	effINT digits = (effINT)(ceilf(log10f(vinc)));
	effWCHAR fmt[16];
	swprintf_s(fmt, 16, _effT("%%.%df"), digits >= 0 ? 0 : -digits);
	effWCHAR msg[128];
	swprintf_s(msg, 128, fmt, *val);
	
	if ( enabled )
	{
		addGfxCmdText(x+SLIDER_HEIGHT/2, y+SLIDER_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, isHot(id) ? imguiRGBA(255,196,0,255) : imguiRGBA(255,255,255,200));
		addGfxCmdText(x+w-SLIDER_HEIGHT/2, y+SLIDER_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_RIGHT, msg, isHot(id) ? imguiRGBA(255,196,0,255) : imguiRGBA(255,255,255,200));
	}
	else
	{
		addGfxCmdText(x+SLIDER_HEIGHT/2, y+SLIDER_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, imguiRGBA(128,128,128,200));
		addGfxCmdText(x+w-SLIDER_HEIGHT/2, y+SLIDER_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_RIGHT, msg, imguiRGBA(128,128,128,200));
	}

	return res || valChanged;
}

effBOOL imguiTree(EFF3DObject * object, effBOOL enabled)
{
	g_state.widgetId++;
	effUINT id = (g_state.areaId<<16) | g_state.widgetId;

	g_state.treeDepth++;

	effString name;
	object->GetPropertyValue(_effT("name"), name);
	
	effUINT childrenCount = (effUINT)object->GetChildrenCount();
	effINT indent = TREE_INDENT * g_state.treeDepth;

	if ( childrenCount != 0 )
	{
		indent += 20;
		effBOOL checked = g_state.treeNodeChecked[object->GetID()];
		if ( imguiCheck(name.c_str(), checked, enabled) )
		{
			g_state.treeNodeChecked[object->GetID()] = !g_state.treeNodeChecked[object->GetID()];
		}
	}
	else
	{
		if ( enabled )
		{
			addGfxCmdText(g_state.widgetX + indent, g_state.widgetY, IMGUI_ALIGN_LEFT, name.c_str(), isHot(id) ? imguiRGBA(255,196,0,255) : imguiRGBA(255,255,255,200));
		}
		else
		{
			addGfxCmdText(g_state.widgetX + indent, g_state.widgetY, IMGUI_ALIGN_LEFT, name.c_str(), imguiRGBA(128,128,128,200));
		}


		g_state.widgetY += 20;
	}

	if ( g_state.treeNodeChecked[object->GetID()] )
	{

		for ( effUINT i = 0; i < childrenCount; i++ )
		{
			EFF3DObject * child = (EFF3DObject *)object->GetChild(i);
			imguiTree(child, enabled);
		}
	}

	g_state.treeDepth--;


	return effTRUE;
}

effVOID imguiIndent()
{
	g_state.widgetX += INDENT_SIZE;
	g_state.widgetW -= INDENT_SIZE;
}

effVOID imguiUnindent()
{
	g_state.widgetX -= INDENT_SIZE;
	g_state.widgetW += INDENT_SIZE;
}

effVOID imguiSeparator()
{
	g_state.widgetY -= DEFAULT_SPACING*3;
}

effVOID imguiSeparatorLine()
{
	effINT x = g_state.widgetX;
	effINT y = g_state.widgetY - DEFAULT_SPACING*2;
	effINT w = g_state.widgetW;
	effINT h = 1;
	g_state.widgetY -= DEFAULT_SPACING*4;

	addGfxCmdRect((effFLOAT)x, (effFLOAT)y, (effFLOAT)w, (effFLOAT)h, imguiRGBA(255,255,255,32));
}

effVOID imguiDrawText(effINT x, effINT y, effINT align, const effWCHAR * text, effUINT color)
{
	addGfxCmdText(x, y, align, text, color);
}

effVOID imguiDrawLine(effFLOAT x0, effFLOAT y0, effFLOAT x1, effFLOAT y1, effFLOAT r, effUINT color)
{
	addGfxCmdLine(x0, y0, x1, y1, r, color);
}

effVOID imguiDrawRect(effFLOAT x, effFLOAT y, effFLOAT w, effFLOAT h, effUINT color)
{
	addGfxCmdRect(x, y, w, h, color);
}

effVOID imguiDrawRoundedRect(effFLOAT x, effFLOAT y, effFLOAT w, effFLOAT h, effFLOAT r, effUINT color)
{
	addGfxCmdRoundedRect(x, y, w, h, r, color);
}



EFF3D_END