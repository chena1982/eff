/******************************************************************************
	created:	2013-4-23   23:30
	file path:	d:\EFF\Src\EFF3D\EFF3DFont.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DFont.h"
#include "EFF3DDevice.h"
#include "EFF3DTexture.h"

#include <ft2build.h>
#include FT_FREETYPE_H

EFF3D_BEGIN

#define FONT_TEXTURE_DEFAULT_SIZE 32

EFF3DFont::EFF3DFont()
{
	fontManager = NULL;
	fontTexture = NULL;

	fontSize = 0;
	fontCount = 0;
	maxFontCount = 0;

	textureWidth = FONT_TEXTURE_DEFAULT_SIZE;
	textureHeight = FONT_TEXTURE_DEFAULT_SIZE;

	blankAreaX = 0;
	blankAreaY = 0;
	blankAreaWidth = FONT_TEXTURE_DEFAULT_SIZE;
	blankAreaHeight = FONT_TEXTURE_DEFAULT_SIZE;

	currentX = 0;
	currentY = 0;
}


EFF3DFont::~EFF3DFont()
{
}

effBOOL EFF3DFont::DrawText(const effString & text)
{
	AddCodePointsToTexture(text);

	EFF3DDevice * device = GetDevice();
	device->SetTexture(0, fontTexture);

	return effTRUE;
}


effBOOL EFF3DFont::AddCodePointsToTexture(const effString & text)
{

	std::vector<effWCHAR> unloadedCodePoints;
	for ( effUINT i = 0; i < text.size(); i++ )
	{
		if ( glyphsInfo[text[i]].x1 == 0 )
		{
			//随便给个值，去掉text中重复的字符
			glyphsInfo[text[i]].x1 = 1;
			unloadedCodePoints.push_back(text[i]);
		}
	}

	if ( unloadedCodePoints.size() == 0 )
	{
		return effTRUE;
	}

	effBYTE * glyphBuffer = fontManager->LoadFromFile(fontFilePath, fontSize, unloadedCodePoints, glyphsInfo);
	if ( glyphBuffer == NULL )
	{
		return effFALSE;
	}


	EFF3DLOCKED_RECT rc;
	memset(&rc, 0, sizeof(EFF3DLOCKED_RECT));


	effINT newFontCount = fontCount + (effINT)unloadedCodePoints.size();

	if ( newFontCount > maxFontCount )
	{

		effINT newTextureWidth = textureWidth;
		effINT newTextureHeight = textureHeight;



		while ( newFontCount > maxFontCount )
		{
			//朝左扩展
			if ( newTextureWidth == newTextureHeight )
			{
				newTextureWidth *= 2;
			}
			//朝下扩展
			else
			{
				newTextureHeight *= 2;
			}

			maxFontCount *= 2;
		}



		EFF3DTexture * newFontTexture = NULL;
		EFF3DDevice * device = GetDevice();
		device->CreateTexture(newTextureWidth, newTextureHeight, 1, 0, EFF3DFMT_A8, EFF3DPOOL_MANAGED, &newFontTexture);


		if ( FAILED(newFontTexture->LockRect(0, &rc, NULL, 0)) )
		{
			return effFALSE;
		}

		if ( fontTexture != NULL )
		{
			EFF3DLOCKED_RECT oldRc;
			if ( FAILED(fontTexture->LockRect(0, &oldRc, NULL, 0)) )
			{
				return effFALSE;
			}

			for ( effINT i = 0; i < textureHeight; i++ )
			{
				memcpy((effBYTE *)rc.pBits + i * rc.Pitch, (effBYTE *)oldRc.pBits + i * oldRc.Pitch, textureWidth);
			}

			fontTexture->UnlockRect(0);
			SF_RELEASE(fontTexture);
		}

		textureWidth = newTextureWidth;
		textureHeight = newTextureHeight;
		fontTexture = newFontTexture;
	}
	else
	{

		if ( fontTexture == NULL )
		{
			EFF3DDevice * device = GetDevice();
			device->CreateTexture(textureWidth, textureHeight, 1, 0, EFF3DFMT_A8, EFF3DPOOL_MANAGED, &fontTexture);
		}

		if ( FAILED(fontTexture->LockRect(0, &rc, NULL, 0)) )
		{
			return effFALSE;
		}
	}




	for ( effINT i = fontCount; i < newFontCount; i++ )
	{
		//x超出空白区域
		if ( currentX >= blankAreaX + blankAreaWidth )
		{
			//切换空白区域
			if ( currentY + fontSize >= blankAreaY + blankAreaHeight )
			{
				//空白区域朝左扩展，如果非空白区域的宽已经是高的2倍，优先朝下扩展
				if ( blankAreaX + blankAreaWidth < textureWidth && (blankAreaX + blankAreaWidth) == (blankAreaY + blankAreaHeight) )
				{
					blankAreaX += blankAreaWidth;
					blankAreaY = 0;
					blankAreaHeight = blankAreaWidth;
				}
				//空白区域朝下扩展
				else if ( blankAreaY + blankAreaHeight < textureHeight )
				{
					blankAreaX = 0;
					blankAreaWidth *= 2;
					blankAreaY += blankAreaHeight;
				}
				else
				{
					return effFALSE;
				}

				currentX = blankAreaX;
				currentY = blankAreaY;
			}
			//换行
			else
			{
				currentX = blankAreaX;
				currentY += fontSize;
			}
		}

		for ( effINT j = 0; j < fontSize; j++ )
		{
			memcpy(((effBYTE *)rc.pBits) + currentX + rc.Pitch * (currentY + j), glyphBuffer + j * fontSize, fontSize);
		}

		glyphBuffer += fontSize * fontSize;

		EFF3DFontGlyphInfo & glyphInfo = glyphsInfo[unloadedCodePoints[i - fontCount]];
		glyphInfo.x0 += currentX;
		glyphInfo.x1 += currentX;
		glyphInfo.y0 += currentY;
		glyphInfo.y1 += currentY;

		currentX += fontSize;
	}

	fontCount = newFontCount;
	fontTexture->UnlockRect(0);


	return effTRUE;
}


static FT_Library library;

EFF3DFontManager::EFF3DFontManager()
{
	FT_Init_FreeType(&library);
};


EFF3DFontManager::~EFF3DFontManager()
{
}

EFF3DFont * EFF3DFontManager::CreateFromFile(const effString & fontFilePath, effINT fontSize)
{
	EFF3DFont * font = EFFNEW EFF3DFont();
	font->fontSize = fontSize;
	font->fontFilePath = fontFilePath;
	font->maxFontCount = (font->textureWidth / fontSize) * (font->textureWidth / fontSize);
	fonts.push_back(font);

	return font;
}

effBYTE * EFF3DFontManager::LoadFromFile(const effString & fontFilePath, effINT fontSize, std::vector<effWCHAR> codePoints, EFF3DFontGlyphInfo * glyphsInfo)
{

	FILE * fp;
	_wfopen_s(&fp, fontFilePath.c_str(), _effT("rb"));

	if ( !fp )
	{
		return effFALSE;
	}

	fseek(fp, 0, SEEK_END);
	effINT size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	effBYTE * ttfBuffer = (effBYTE *)EFFNEW effBYTE[size]; 
	if ( ttfBuffer == NULL )
	{
		fclose(fp);
		return NULL;
	}
	
	fread(ttfBuffer, 1, size, fp);
	fclose(fp);
	fp = 0;

	FT_Face face;
	FT_Error error = FT_New_Memory_Face(library, ttfBuffer, size, 0, &face);


	error = FT_Set_Pixel_Sizes(face, 0, fontSize);


	effBYTE * glyphBuffer = EFFNEW effBYTE[codePoints.size() * fontSize * fontSize];
	memset(glyphBuffer, 0, codePoints.size() * fontSize * fontSize);


	effBYTE * currentGlyphBuffer = glyphBuffer;

	for ( effUINT i = 0; i < codePoints.size(); i++ )
	{
		effUINT glyphIndex = FT_Get_Char_Index(face, codePoints[i]);
		FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);

		if ( face->glyph->format != FT_GLYPH_FORMAT_BITMAP )
		{
			FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		}


		FT_Bitmap & bitmap = face->glyph->bitmap;

		for ( effINT j = 0; j < bitmap.rows; j++ )
		{
			memcpy(currentGlyphBuffer + j * fontSize, bitmap.buffer + j * bitmap.pitch, min(bitmap.width, fontSize));
		}
		currentGlyphBuffer += fontSize * fontSize;

		EFF3DFontGlyphInfo * glyphInfo = &glyphsInfo[codePoints[i]];

		glyphInfo->x1 = min(bitmap.width, fontSize);
		glyphInfo->y1 = bitmap.rows;
		glyphInfo->xoffset = face->glyph->bitmap_left;
		glyphInfo->yoffset = face->glyph->bitmap_top;
		glyphInfo->width = (effBYTE)(face->glyph->advance.x / 64);

		if ( face->glyph->advance.y == 0 )
		{
			glyphInfo->height = glyphInfo->width;
		}
		else
		{
			glyphInfo->height = (effBYTE)(face->glyph->advance.y / 64);
		}

	}

	SFT_DELETE(ttfBuffer);

	return glyphBuffer;
}


EFF3D_END