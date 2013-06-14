/******************************************************************************
	created:	2013-4-20   15:46
	file path:	d:\EFF\Src\EFF3D\EFF3DImguiRender.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DImguiRender.h"
#include "EFF3DImgui.h"
#include "EFF3DDevice.h"
#include "EFF3DTexture.h"
#include "EFF3DFont.h"

EFF3D_BEGIN




effVOID imguifree(effVOID * ptr, effVOID * userptr);
effVOID * imguimalloc(effUINT size, effVOID * userptr);

#define STBTT_malloc(x,y)    imguimalloc(x,y)
#define STBTT_free(x,y)      imguifree(x,y)
#define STB_TRUETYPE_IMPLEMENTATION

#include "stb_truetype.h"

effVOID imguifree(effVOID* ptr, effVOID* /*userptr*/)
{
	free(ptr);
}

effVOID* imguimalloc(effUINT size, effVOID* /*userptr*/)
{
	return malloc(size);
}

static const unsigned TEMP_COORD_COUNT = 100;
static effFLOAT g_tempCoords[TEMP_COORD_COUNT*4];
static effFLOAT g_tempNormals[TEMP_COORD_COUNT*4];

static const effINT CIRCLE_VERTS = 8*4;
static effFLOAT g_circleVerts[CIRCLE_VERTS*4];

static stbtt_bakedchar g_cdata[96]; // ASCII 32..126 is 95 glyphs
static effUINT g_ftex = 0;

static EFF3DTexture * fontTexture = NULL;

inline effUINT RGBA(effBYTE r, effBYTE g, effBYTE b, effBYTE a)
{
	return (r) | (g << 8) | (b << 16) | (a << 24);
}

static effVOID drawPolygon(const effFLOAT* coords, effINT numCoords, effFLOAT r, effUINT col)
{
	if (numCoords > TEMP_COORD_COUNT) numCoords = TEMP_COORD_COUNT;
	
	for ( effINT i = 0, j = numCoords - 1; i < numCoords; j = i++ )
	{
		const effFLOAT * v0 = &coords[j*4];
		const effFLOAT * v1 = &coords[i*4];
		effFLOAT dx = v1[0] - v0[0];
		effFLOAT dy = v1[1] - v0[1];
		effFLOAT d = sqrtf(dx*dx+dy*dy);
		if ( d > 0 )
		{
			d = 1.0f/d;
			dx *= d;
			dy *= d;
		}
		g_tempNormals[j*4+0] = dy;
		g_tempNormals[j*4+1] = -dx;
	}
	
	for ( effINT i = 0, j = numCoords-1; i < numCoords; j = i++ )
	{
		effFLOAT dlx0 = g_tempNormals[j*4+0];
		effFLOAT dly0 = g_tempNormals[j*4+1];
		effFLOAT dlx1 = g_tempNormals[i*4+0];
		effFLOAT dly1 = g_tempNormals[i*4+1];
		effFLOAT dmx = (dlx0 + dlx1) * 0.5f;
		effFLOAT dmy = (dly0 + dly1) * 0.5f;
		effFLOAT	dmr2 = dmx*dmx + dmy*dmy;
		if ( dmr2 > 0.000001f )
		{
			effFLOAT	scale = 1.0f / dmr2;
			if (scale > 10.0f) scale = 10.0f;
			dmx *= scale;
			dmy *= scale;
		}
		g_tempCoords[i*4+0] = coords[i*4+0]+dmx*r;
		g_tempCoords[i*4+1] = coords[i*4+1]+dmy*r;
	}
	
	effUINT colTrans = RGBA(col&0xff, (col>>8)&0xff, (col>>16)&0xff, 0);
	

	QuadColoredVertex * temp = EFFNEW QuadColoredVertex[numCoords * 6];
	memset(temp, 0, sizeof(QuadColoredVertex) * numCoords * 6);

	for ( effINT i = 0, j = numCoords - 1; i < numCoords; j = i++ )
	{
		memcpy(&temp[i*6], &coords[i*4], sizeof(effFLOAT) * 2);
		temp[i*6].color = col;
		//glVertex2fv(&coords[i*2]);
		memcpy(&temp[i*6 + 1], &coords[j*4], sizeof(effFLOAT) * 2);
		temp[i*6 + 1].color = col;
		//glVertex2fv(&coords[j*2]);
		//glColor4ubv((GLubyte*)&colTrans);
		memcpy(&temp[i*6 + 2], &g_tempCoords[j*4], sizeof(effFLOAT) * 2);
		temp[i*6 + 2].color = colTrans;
		//glVertex2fv(&g_tempCoords[j*2]);
		
		memcpy(&temp[i*6 + 3], &g_tempCoords[j*4], sizeof(effFLOAT) * 2);
		temp[i*6 + 3].color = colTrans;
		//glVertex2fv(&g_tempCoords[j*2]);
		memcpy(&temp[i*6 + 4], &g_tempCoords[i*4], sizeof(effFLOAT) * 2);
		temp[i*6 + 4].color = colTrans;
		//glVertex2fv(&g_tempCoords[i*2]);
		
		//glColor4ubv((GLubyte*)&col);
		memcpy(&temp[i*6 + 5], &coords[i*4], sizeof(effFLOAT) * 2);
		temp[i*6 + 5].color = col;
		//glVertex2fv(&coords[i*2]);
	}


	EFF3DDevice * device = GetDevice();

	device->SetRenderState(EFF3DRS_ALPHABLENDENABLE, effTRUE);
	device->SetRenderState(EFF3DRS_SRCBLEND, EFF3DBLEND_SRCALPHA);
	device->SetRenderState(EFF3DRS_DESTBLEND, EFF3DBLEND_INVSRCALPHA);

	device->SetTextureStageState(0, EFF3DTSS_COLOROP, EFF3DTOP_SELECTARG1);
	device->SetTextureStageState(0, EFF3DTSS_COLORARG1, EFF3DTA_DIFFUSE);
	device->SetTextureStageState(0, EFF3DTSS_ALPHAOP, EFF3DTOP_SELECTARG1);
	device->SetTextureStageState(0, EFF3DTSS_ALPHAARG1, EFF3DTA_DIFFUSE);
	device->SetFVF(QuadColoredVertex::fvf);
	device->DrawPrimitiveUP(EFF3DPT_TRIANGLELIST, numCoords * 2, temp, sizeof(QuadColoredVertex));

	device->SetTextureStageState(0, EFF3DTSS_COLOROP, EFF3DTOP_SELECTARG1);
	device->SetTextureStageState(0, EFF3DTSS_COLORARG1, EFF3DTA_CONSTANT);
	device->SetTextureStageState(0, EFF3DTSS_CONSTANT, col);
	device->SetFVF(EFF3DFVF_XYZRHW);
	device->DrawPrimitiveUP(EFF3DPT_TRIANGLEFAN, numCoords - 2, coords, sizeof(effFLOAT) * 4);


	SFT_DELETE(temp);

}

static effVOID drawRect(effFLOAT x, effFLOAT y, effFLOAT w, effFLOAT h, effFLOAT fth, effUINT col)
{


	/*effFLOAT verts[4*4] =
	{
		x+0.5f, y+0.5f, 0.0f, 0.0f,
		x+w-0.5f, y+0.5f, 0.0f, 0.0f,
		x+w-0.5f, y+h-0.5f, 0.0f, 0.0f,
		x+0.5f, y+h-0.5f, 0.0f, 0.0f,
	};*/

	EFF3DDevice * device = GetDevice();

	EFFRect rect(x, y, w, h);
	device->DrawQuad(&rect, col, NULL);
}

/*
static effVOID drawEllipse(effFLOAT x, effFLOAT y, effFLOAT w, effFLOAT h, effFLOAT fth, unsigned effINT col)
{
	effFLOAT verts[CIRCLE_VERTS*2];
	const effFLOAT* cverts = g_circleVerts;
	effFLOAT* v = verts;
	
	for (effINT i = 0; i < CIRCLE_VERTS; ++i)
	{
		*v++ = x + cverts[i*2]*w;
		*v++ = y + cverts[i*2+1]*h;
	}
	
	drawPolygon(verts, CIRCLE_VERTS, fth, col);
}
*/

static effVOID drawRoundedRect(effFLOAT x, effFLOAT y, effFLOAT w, effFLOAT h, effFLOAT r, effFLOAT fth, effUINT col)
{
	const effINT n = CIRCLE_VERTS/4;
	effFLOAT verts[(n+1)*4*4];
	memset(verts, 0, sizeof(verts));

	const effFLOAT * cverts = g_circleVerts;
	effFLOAT* v = verts;
	

	for (unsigned i = 0; i <= n; ++i)
	{
		*v++ = x+w-r + cverts[i*2]*r;
		*v++ = y+h-r + cverts[i*2+1]*r;

		v += 2;
	}
	
	for (unsigned i = n; i <= n*2; ++i)
	{
		*v++ = x+r + cverts[i*2]*r;
		*v++ = y+h-r + cverts[i*2+1]*r;

		v += 2;
	}
	
	for (unsigned i = n*2; i <= n*3; ++i)
	{
		*v++ = x+r + cverts[i*2]*r;
		*v++ = y+r + cverts[i*2+1]*r;

		v += 2;
	}
	
	for (unsigned i = n*3; i < n*4; ++i)
	{
		*v++ = x+w-r + cverts[i*2]*r;
		*v++ = y+r + cverts[i*2+1]*r;

		v += 2;
	}
	*v++ = x+w-r + cverts[0]*r;
	*v++ = y+r + cverts[1]*r;

	drawPolygon(verts, (n+1)*4, fth, col);
}


static effVOID drawLine(effFLOAT x0, effFLOAT y0, effFLOAT x1, effFLOAT y1, effFLOAT r, effFLOAT fth, effUINT col)
{
	effFLOAT dx = x1-x0;
	effFLOAT dy = y1-y0;
	effFLOAT d = sqrtf(dx*dx+dy*dy);
	if (d > 0.0001f)
	{
		d = 1.0f/d;
		dx *= d;
		dy *= d;
	}
	effFLOAT nx = dy;
	effFLOAT ny = -dx;
	effFLOAT verts[4*2];
	r -= fth;
	r *= 0.5f;
	if (r < 0.01f) r = 0.01f;
	dx *= r;
	dy *= r;
	nx *= r;
	ny *= r;
	
	verts[0] = x0-dx-nx;
	verts[1] = y0-dy-ny;
	
	verts[2] = x0-dx+nx;
	verts[3] = y0-dy+ny;
	
	verts[4] = x1+dx+nx;
	verts[5] = y1+dy+ny;
	
	verts[6] = x1+dx-nx;
	verts[7] = y1+dy-ny;
	
	drawPolygon(verts, 4, fth, col);
}


effBOOL imguiRenderInit(const effTCHAR * fontpath)
{
	for (effINT i = 0; i < CIRCLE_VERTS; ++i)
	{
		effFLOAT a = (effFLOAT)i / (effFLOAT)CIRCLE_VERTS * PI * 2;
		g_circleVerts[i*2 + 0] = cosf(a);
		g_circleVerts[i*2 + 1] = sinf(a);
	}

	// Load font.
	FILE * fp;
	_wfopen_s(&fp, fontpath, _effT("rb"));

	if ( !fp )
	{
		return effFALSE;
	}

	fseek(fp, 0, SEEK_END);
	effINT size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	effBYTE* ttfBuffer = (effBYTE*)malloc(size); 
	if (!ttfBuffer)
	{
		fclose(fp);
		return effFALSE;
	}
	
	fread(ttfBuffer, 1, size, fp);
	fclose(fp);
	fp = 0;
	
	effBYTE* bmap = (effBYTE*)malloc(512*512);
	if (!bmap)
	{
		free(ttfBuffer);
		return effFALSE;
	}
	
	stbtt_fontinfo font;
	stbtt_InitFont(&font, ttfBuffer, stbtt_GetFontOffsetForIndex(ttfBuffer, 0));
	effFLOAT scale = stbtt_ScaleForPixelHeight(&font, 15.0f);

	//effINT codepoint = 'a';
	//effINT width, height, xoffset, yoffset;
	//effBYTE * bitmap = stbtt_GetCodepointBitmap(&font, 0, scale, codepoint, &width, &height, &xoffset, &yoffset);

	stbtt_BakeFontBitmap(ttfBuffer, 0, 15.0f, bmap, 512, 512, 32, 96, g_cdata);
	
	EFF3DDevice * device = GetDevice();
	device->CreateTextureFromMemory(bmap, 512 * 512, EFF3DFMT_A8, 512, 512, 1, &fontTexture);
	//device->CreateTextureFromMemory(bitmap, width * height, EFF3DFMT_A8, width, height, 1, &fontTexture);



	free(ttfBuffer);
	free(bmap);

	return true;
}

effVOID imguiRenderDestroy()
{
	if (g_ftex)
	{
		//glDeleteTextures(1, &g_ftex);
		g_ftex = 0;
	}
}

static effVOID getBakedQuad(stbtt_bakedchar *chardata, effINT pw, effINT ph, effINT char_index,
						 effFLOAT *xpos, effFLOAT *ypos, stbtt_aligned_quad *q)
{
	stbtt_bakedchar *b = chardata + char_index;
	effINT round_x = STBTT_ifloor(*xpos + b->xoff);
	effINT round_y = STBTT_ifloor(*ypos + b->yoff);
	
	q->x0 = (effFLOAT)round_x;
	q->y0 = (effFLOAT)round_y;
	q->x1 = (effFLOAT)round_x + b->x1 - b->x0;
	q->y1 = (effFLOAT)round_y + b->y1 - b->y0;
	
	q->s0 = b->x0 / (effFLOAT)pw;
	q->t0 = b->y0 / (effFLOAT)pw;
	q->s1 = b->x1 / (effFLOAT)ph;
	q->t1 = b->y1 / (effFLOAT)ph;
	
	*xpos += b->xadvance;
}

static const effFLOAT g_tabStops[4] = {150, 210, 270, 330};

static effFLOAT getTextLength(stbtt_bakedchar *chardata, const WCHAR* text)
{
	effFLOAT xpos = 0;
	effFLOAT len = 0;
	while (*text)
	{
		effWCHAR c = *text;
		if ( c == _effT('\t') )
		{
			for (effINT i = 0; i < 4; ++i)
			{
				if (xpos < g_tabStops[i])
				{
					xpos = g_tabStops[i];
					break;
				}
			}
		}
		else if (c >= 32 && c < 128)
		{
			stbtt_bakedchar *b = chardata + c-32;
			effINT round_x = STBTT_ifloor((xpos + b->xoff) + 0.5);
			len = round_x + b->x1 - b->x0 + 0.5f;
			xpos += b->xadvance;
		}
		++text;
	}
	return len;
}

static effVOID drawText(effFLOAT x, effFLOAT y, const effWCHAR * text, effINT align, effUINT col)
{
	if (!text) return;
	
	if (align == IMGUI_ALIGN_CENTER)
		x -= getTextLength(g_cdata, text)/2;
	else if (align == IMGUI_ALIGN_RIGHT)
		x -= getTextLength(g_cdata, text);
	
	//glColor4ub(col&0xff, (col>>8)&0xff, (col>>16)&0xff, (col>>24)&0xff);

	//imguiRGBA(col, col, col, col);

	EFF3DDevice * device = GetDevice();
	//EFF3DFont * font = device->GetFontManager()->GetFont(L"Î¢ÈíÑÅºÚ");
	EFF3DFont * font = device->GetFontManager()->GetFont(L"ËÎÌå");

	if ( font == NULL )
	{
		return;
	}
	
	effINT posX = (effINT)x;
	effINT posY = (effINT)y;
	font->DrawText(text, posX, posY, col);
}


effVOID imguiRenderDraw()
{
	const imguiGfxCmd* q = imguiGetRenderQueue();
	effINT nq = imguiGetRenderQueueSize();

	const effFLOAT s = 1.0f/8.0f;

	//glDisable(GL_SCISSOR_TEST);
	for (effINT i = 0; i < nq; ++i)
	{
		const imguiGfxCmd& cmd = q[i];
		if (cmd.type == IMGUI_GFXCMD_RECT)
		{
			if (cmd.rect.r == 0)
			{
				drawRect((effFLOAT)cmd.rect.x*s+0.5f, (effFLOAT)cmd.rect.y*s+0.5f,
						 (effFLOAT)cmd.rect.w*s-1, (effFLOAT)cmd.rect.h*s-1,
						 1.0f, cmd.col);
			}
			else
			{
				drawRoundedRect((effFLOAT)cmd.rect.x*s+0.5f, (effFLOAT)cmd.rect.y*s+0.5f,
								(effFLOAT)cmd.rect.w*s-1, (effFLOAT)cmd.rect.h*s-1,
								(effFLOAT)cmd.rect.r*s, 1.0f, cmd.col);
			}
		}
		else if (cmd.type == IMGUI_GFXCMD_LINE)
		{
			drawLine(cmd.line.x0*s, cmd.line.y0*s, cmd.line.x1*s, cmd.line.y1*s, cmd.line.r*s, 1.0f, cmd.col);
		}
		else if (cmd.type == IMGUI_GFXCMD_TRIANGLE)
		{
			if (cmd.flags == 1)
			{
				const effFLOAT verts[3*2] =
				{
					(effFLOAT)cmd.rect.x*s+0.5f, (effFLOAT)cmd.rect.y*s+0.5f,
					(effFLOAT)cmd.rect.x*s+0.5f+(effFLOAT)cmd.rect.w*s-1, (effFLOAT)cmd.rect.y*s+0.5f+(effFLOAT)cmd.rect.h*s/2-0.5f,
					(effFLOAT)cmd.rect.x*s+0.5f, (effFLOAT)cmd.rect.y*s+0.5f+(effFLOAT)cmd.rect.h*s-1,
				};
				drawPolygon(verts, 3, 1.0f, cmd.col);
			}
			if (cmd.flags == 2)
			{
				const effFLOAT verts[3*2] =
				{
					(effFLOAT)cmd.rect.x*s+0.5f, (effFLOAT)cmd.rect.y*s+0.5f+(effFLOAT)cmd.rect.h*s-1,
					(effFLOAT)cmd.rect.x*s+0.5f+(effFLOAT)cmd.rect.w*s/2-0.5f, (effFLOAT)cmd.rect.y*s+0.5f,
					(effFLOAT)cmd.rect.x*s+0.5f+(effFLOAT)cmd.rect.w*s-1, (effFLOAT)cmd.rect.y*s+0.5f+(effFLOAT)cmd.rect.h*s-1,
				};
				drawPolygon(verts, 3, 1.0f, cmd.col);
			}
		}
		else if (cmd.type == IMGUI_GFXCMD_TEXT)
		{
			drawText(cmd.text.x, cmd.text.y, cmd.text.text, cmd.text.align, cmd.col);
		}
		else if (cmd.type == IMGUI_GFXCMD_SCISSOR)
		{
			if (cmd.flags)
			{
				//glEnable(GL_SCISSOR_TEST);
				//glScissor(cmd.rect.x, cmd.rect.y, cmd.rect.w, cmd.rect.h);
			}
			else
			{
				//glDisable(GL_SCISSOR_TEST);
			}
		}
	}
	//glDisable(GL_SCISSOR_TEST);
}


EFF3D_END