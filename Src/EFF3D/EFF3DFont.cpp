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
#include FT_SFNT_NAMES_H

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
	fontManager = NULL;
	SF_RELEASE(fontTexture);
}

//maybe need a optimize that render all glyph once time, but all top window have a cache buffer, 
//so need accord to test result to determine whether it needs to be optimized
effBOOL EFF3DFont::DrawText(const effString & text, effINT & x, effINT & y, effUINT color)
{
	AddCodePointsToTexture(text);

	EFF3DDevice * device = GetDevice();
	device->SetTexture(0, fontTexture);
	device->SetRenderState(EFF3DRS_CULLMODE, EFF3DCULL_NONE);
	device->SetRenderState(EFF3DRS_TEXTUREFACTOR, color);

	device->SetRenderState(EFF3DRS_ALPHABLENDENABLE, effTRUE);
	device->SetRenderState(EFF3DRS_SRCBLEND, EFF3DBLEND_SRCALPHA);
	device->SetRenderState(EFF3DRS_DESTBLEND, EFF3DBLEND_INVSRCALPHA);

	device->SetTextureStageState(0, EFF3DTSS_COLOROP, EFF3DTOP_SELECTARG1);
	device->SetTextureStageState(0, EFF3DTSS_COLORARG1,	EFF3DTA_TFACTOR);

	device->SetTextureStageState(0, EFF3DTSS_ALPHAOP, EFF3DTOP_SELECTARG1);
	device->SetTextureStageState(0, EFF3DTSS_ALPHAARG1, EFF3DTA_TEXTURE);


	device->SetSamplerState(0, EFF3DSAMP_MINFILTER, EFF3DTEXF_POINT);
	device->SetSamplerState(0, EFF3DSAMP_MAGFILTER, EFF3DTEXF_POINT);

	//don't worry about redundant state change, EFF3DDevice will process it

	effUINT fvf = QuadVertex::fvf;
	device->SetFVF(fvf);

	for ( effUINT i = 0; i < text.size(); i++ )
	{
		//text[i] is a ucs-2 character, so don't need check text[i] value
		EFF3DFontGlyphInfo & glyphInfo = glyphsInfo[text[i]];

		//glyph bitmap don't loaded
		if ( glyphInfo.y1 == 0 )
		{
			continue;
		}


		effINT16 glyphWidth = glyphInfo.x1 - glyphInfo.x0;
		effINT16 glyphHeight = glyphInfo.y1 - glyphInfo.y0;

		//all x, y coordinate -0.5f, to Directly Mapping Texels to Pixels

		QuadVertex buff[6];
		memset(buff, 0, sizeof(buff));
		buff[0].x = x + glyphInfo.xoffset - 0.5f;
		buff[0].y = y + glyphInfo.yoffset - 0.5f;
		buff[0].u = (effFLOAT)glyphInfo.x0 / (effFLOAT)fontTexture->GetImageInfo().width;
		buff[0].v = (effFLOAT)glyphInfo.y0 / (effFLOAT)fontTexture->GetImageInfo().height;

		buff[1].x = x + glyphInfo.xoffset + glyphWidth - 0.5f;
		buff[1].y = y + glyphInfo.yoffset + glyphHeight - 0.5f;
		buff[1].u = (effFLOAT)glyphInfo.x1 / (effFLOAT)fontTexture->GetImageInfo().width;
		buff[1].v = (effFLOAT)glyphInfo.y1 / (effFLOAT)fontTexture->GetImageInfo().height;

		buff[2].x = x + glyphInfo.xoffset + glyphWidth - 0.5f;
		buff[2].y = y + glyphInfo.yoffset - 0.5f;
		buff[2].u = (effFLOAT)glyphInfo.x1 / (effFLOAT)fontTexture->GetImageInfo().width;
		buff[2].v = (effFLOAT)glyphInfo.y0 / (effFLOAT)fontTexture->GetImageInfo().height;


		buff[3].x = x + glyphInfo.xoffset - 0.5f;
		buff[3].y = y + glyphInfo.yoffset - 0.5f;
		buff[3].u = (effFLOAT)glyphInfo.x0 / (effFLOAT)fontTexture->GetImageInfo().width;
		buff[3].v = (effFLOAT)glyphInfo.y0 / (effFLOAT)fontTexture->GetImageInfo().height;
			


		buff[4].x = x + glyphInfo.xoffset - 0.5f;
		buff[4].y = y + glyphInfo.yoffset + glyphHeight - 0.5f;
		buff[4].u = (effFLOAT)glyphInfo.x0 / (effFLOAT)fontTexture->GetImageInfo().width;
		buff[4].v = (effFLOAT)glyphInfo.y1 / (effFLOAT)fontTexture->GetImageInfo().height;


		buff[5].x = x + glyphInfo.xoffset + glyphWidth - 0.5f;
		buff[5].y = y + glyphInfo.yoffset + glyphHeight - 0.5f;
		buff[5].u = (effFLOAT)glyphInfo.x1 / (effFLOAT)fontTexture->GetImageInfo().width;
		buff[5].v = (effFLOAT)glyphInfo.y1 / (effFLOAT)fontTexture->GetImageInfo().height;

		x += glyphInfo.width;




		device->DrawPrimitiveUP(EFF3DPT_TRIANGLELIST, 2, buff, sizeof(QuadVertex));
	}

	return effTRUE;
}


effBOOL EFF3DFont::AddCodePointsToTexture(const effString & text)
{

	std::vector<effWCHAR> unloadedCodePoints;
	for ( effUINT i = 0; i < text.size(); i++ )
	{
		//text[i] is a ucs-2 character, so don't need check text[i] value
		if ( glyphsInfo[text[i]].x1 == 0 )
		{
			//set x1 to 1, so text[i] won't repeated add to unloadedCodePoints
			glyphsInfo[text[i]].x1 = 1;
			unloadedCodePoints.push_back(text[i]);
		}
	}

	if ( unloadedCodePoints.size() == 0 )
	{
		return effTRUE;
	}

	effBYTE * glyphBuffer = fontManager->LoadGlyphBitmapFromFile(fontFilePath, fontSize, unloadedCodePoints, glyphsInfo);
	if ( glyphBuffer == NULL )
	{
		return effFALSE;
	}

	//delete glyph buffer when function end
	ON_SCOPE_EXIT([&] { SFT_DELETE(glyphBuffer); })

	effBYTE * currentGlyphBuffer = glyphBuffer;

	EFF3DLOCKED_RECT rc;
	memset(&rc, 0, sizeof(EFF3DLOCKED_RECT));


	effINT newFontCount = fontCount + (effINT)unloadedCodePoints.size();

	if ( newFontCount > maxFontCount )
	{

		effINT newTextureWidth = textureWidth;
		effINT newTextureHeight = textureHeight;

		while ( newFontCount > maxFontCount )
		{
			//if texture width equal to texture height, extend texture width to 2 * width
			if ( newTextureWidth == newTextureHeight )
			{
				newTextureWidth *= 2;
			}
			//extend texture height to 2 * height
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




	for ( effUINT i = 0; i < unloadedCodePoints.size(); i++ )
	{
		//x exceed blank area
		if ( currentX + fontSize > blankAreaX + blankAreaWidth )
		{

			//go to next line
			if ( currentY + 2 * fontSize <= blankAreaY + blankAreaHeight )
			{
				currentX = blankAreaX;
				currentY += fontSize;
			}
			//go to next blank area
			else
			{
				//go to right blank area
				if ( blankAreaX + blankAreaWidth < textureWidth && (blankAreaX + blankAreaWidth) == (blankAreaY + blankAreaHeight) )
				{
					blankAreaX += blankAreaWidth;
					blankAreaY = 0;
					blankAreaHeight = blankAreaWidth;
				}
				//go to bottom blank area
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

		}

		for ( effINT j = 0; j < fontSize; j++ )
		{
			memcpy(((effBYTE *)rc.pBits) + currentX + rc.Pitch * (currentY + j), currentGlyphBuffer + j * fontSize, fontSize);
		}

		currentGlyphBuffer += fontSize * fontSize;

		EFF3DFontGlyphInfo & glyphInfo = glyphsInfo[unloadedCodePoints[i]];
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
	FT_Done_FreeType(library);
}

EFF3DFont * EFF3DFontManager::CreateFromFile(const effString & fontFilePath, effINT fontSize)
{
	for ( effUINT i = 0; i < fonts.size(); i++ )
	{
		EFF3DFont * font = fonts[i];
		if ( font->fontFilePath == fontFilePath && font->fontSize == fontSize )
		{
			return fonts[i];
		}
	}


	EFF3DFont * font = EFFNEW EFF3DFont();
	font->fontSize = fontSize;
	font->fontFilePath = fontFilePath;
	font->fontManager = this;
	font->maxFontCount = (font->textureWidth / fontSize) * (font->textureWidth / fontSize);

	FontFaceInfo * fontFaceInfo = GetFontFace(fontFilePath);
	if ( fontFaceInfo != NULL )
	{
		font->fontName = fontFaceInfo->fontName;
	}

	fonts.push_back(font);

	return font;
}

effVOID	EFF3DFontManager::ReleaseFont(EFF3DFont * font)
{
	if ( font == NULL )
	{
		return;
	}

	for ( effUINT i = 0; i < fonts.size(); i++ )
	{
		if ( fonts[i] == font )
		{
			for ( effUINT j = 0; j < fontsFaceInfo.size(); j++ )
			{
				if ( font->fontFilePath == fontsFaceInfo[i].fontFilePath )
				{
					SFT_DELETE(fontsFaceInfo[i].buffer);
					FT_Done_Face(fontsFaceInfo[i].face);
					fontsFaceInfo.erase(fontsFaceInfo.begin() + j);
					break;
				}
			}

			SF_DELETE(font);
			fonts.erase(fonts.begin() + i);
			break;
		}
	}
}

EFF3DFont *	EFF3DFontManager::GetFont(const effString & fontName)
{
	for ( effUINT i = 0; i < fonts.size(); i++ )
	{
		if ( fonts[i]->fontName == fontName )
		{
			return fonts[i];
		}
	}

	return NULL;
}

effBYTE * EFF3DFontManager::LoadGlyphBitmapFromFile(const effString & fontFilePath, effINT fontSize, std::vector<effWCHAR> codePoints, EFF3DFontGlyphInfo * glyphsInfo)
{

	FontFaceInfo * fontFaceInfo = GetFontFace(fontFilePath);

	if ( fontFaceInfo == NULL )
	{
		return NULL;
	}

	FT_Face face = fontFaceInfo->face;
		
	FT_Error error = FT_Set_Pixel_Sizes(face, 0, fontSize);


	effBYTE * glyphBuffer = EFFNEW effBYTE[codePoints.size() * fontSize * fontSize];
	memset(glyphBuffer, 0, codePoints.size() * fontSize * fontSize);

	effBYTE * currentGlyphBuffer = glyphBuffer;

	for ( effUINT i = 0; i < codePoints.size(); i++ )
	{
		effUINT glyphIndex = FT_Get_Char_Index(face, codePoints[i]);
		error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
		if ( error )
		{
			continue;
		}

		if ( face->glyph->format != FT_GLYPH_FORMAT_BITMAP )
		{
			error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
			if ( error )
			{
				continue;
			}
		}


		FT_Bitmap & bitmap = face->glyph->bitmap;

		effINT fontWidth = min(bitmap.width, fontSize);
		effINT fontHeight = min(bitmap.rows, fontSize);

		//内嵌点阵，bitmap是每pixel 1位的黑白图
		if ( bitmap.pixel_mode == FT_PIXEL_MODE_MONO )
		{
			for ( effINT j = 0; j < fontHeight; j++ )
			{
				effBYTE * bitmapBuffer = bitmap.buffer + j * bitmap.pitch;
				effBYTE * lineStartGlyphBuffer = currentGlyphBuffer + j * fontSize;
				effINT x = 0;
				for ( effINT k = 0; k < bitmap.pitch; k++ )
				{
					effBYTE value = *bitmapBuffer;
					for ( effINT l = 0; l < 8; l++ )
					{
						if ( x >= bitmap.width )
						{
							break;
						}

						if ( value & (1 << (7-l)) )
						{
							*(lineStartGlyphBuffer + x) = 255;
						}
						else
						{
							*(lineStartGlyphBuffer + x) = 0;
						}
						x++;
					}
					bitmapBuffer++;
				}
			}
		}
		else if ( bitmap.pixel_mode == FT_PIXEL_MODE_GRAY )
		{
			for ( effINT j = 0; j < fontHeight; j++ )
			{
				memcpy(currentGlyphBuffer + j * fontSize, bitmap.buffer + j * bitmap.pitch, fontWidth);
			}
		}

		currentGlyphBuffer += fontSize * fontSize;

		//codePoints[i] is a ucs-2 character, so don't need check codePoints[i] value
		EFF3DFontGlyphInfo * glyphInfo = &glyphsInfo[codePoints[i]];

		glyphInfo->x1 = fontWidth;
		glyphInfo->y1 = fontHeight;
		glyphInfo->xoffset = face->glyph->bitmap_left;
		glyphInfo->yoffset = fontSize - face->glyph->bitmap_top;
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

	return glyphBuffer;
}

EFF3DFontManager::FontFaceInfo * EFF3DFontManager::GetFontFace(const effString & fontFilePath)
{
	auto it = fontsFaceInfo.begin();
	for ( ; it != fontsFaceInfo.end(); it++ )
	{
		if ( (*it).fontFilePath == fontFilePath )
		{
			return &(*it);
		}
	}


	FILE * fp;
	_wfopen_s(&fp, fontFilePath.c_str(), _effT("rb"));

	if ( !fp )
	{
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	effINT size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	effBYTE * ttfBuffer = (effBYTE *)EFFNEW effBYTE[size]; 

	fread(ttfBuffer, 1, size, fp);
	fclose(fp);
	fp = NULL;

	FT_Face face = NULL;
	FT_Error error = FT_New_Memory_Face(library, ttfBuffer, size, 0, &face);
	if ( error == FT_Err_Unknown_File_Format )
	{
		EFFLOG("the font file could be opened and read, but it appears that its font format is unsupported");
		SFT_DELETE(ttfBuffer);
		FT_Done_Face(face);
		return NULL;
	}
	else if ( error )
	{
		EFFLOG("another error code means that the font file could not be opened or read, or simply that it is broken");
		SFT_DELETE(ttfBuffer);
		FT_Done_Face(face);
		return NULL;
	}


	effString fontName;

	FT_UInt nameCount = FT_Get_Sfnt_Name_Count(face);
	for ( FT_UInt i = 0; i < nameCount; i++ )
	{
		FT_SfntName name;
		FT_Get_Sfnt_Name(face, i, &name);

		if ( name.language_id == 2052 && name.name_id == 1 )
		{
			effBYTE * buffer = new effBYTE[name.string_len + 2];
			memcpy(buffer, name.string, name.string_len);

			for ( FT_UInt j = 0; j < name.string_len / 2; j++ )
			{
				effBYTE temp = buffer[2*j];
				buffer[2*j] = buffer[2*j + 1];
				buffer[2*j + 1] = temp;
			}

			buffer[name.string_len] = 0;
			buffer[name.string_len + 1] = 0;
			fontName = (effWCHAR *)buffer;
			SFT_DELETE(buffer);

			break;
		}
	}

	FontFaceInfo fontFaceInfo;
	fontFaceInfo.fontFilePath = fontFilePath;
	fontFaceInfo.face = face;
	fontFaceInfo.buffer = ttfBuffer;
	fontFaceInfo.fontName = fontName;

	fontsFaceInfo.push_back(fontFaceInfo);
	return &(*fontsFaceInfo.rbegin());
}


EFF3D_END