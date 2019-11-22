/******************************************************************************
	created:	2013-4-23   23:30
	file path:	d:\EFF\Src\EFF3D\EFF3DFont.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DFont.h"
#include "EFF3DDevice.h"
#include "EFF3DTexture.h"
#include "EFF3DVertexDeclaration.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_SFNT_NAMES_H

EFF3D_BEGIN

#define FONT_TEXTURE_DEFAULT_SIZE 32

EFF3DFont::EFF3DFont()
{
	fontManager = NULL;
    //fontTextureHandle = EFFId();

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
	//SF_RELEASE(fontTexture);

    EFF3DDevice * device = EFF3DGetDevice();
    device->GetTextureManager()->Release(fontTextureHandle);
}

//maybe need a optimize that render all glyph once time, but all top window have a cache buffer, 
//so need accord to test result to determine whether it needs to be optimized
effBOOL EFF3DFont::DrawText(const effString & text, effINT & x, effINT & y, effUINT color)
{
	AddCodePointsToTexture(text);

	EFF3DDevice * device = EFF3DGetDevice();
	device->SetTexture(0, fontTextureHandle);
	/*device->SetRenderState(EFF3DRS_CULLMODE, EFF3DCULL_NONE);
	device->SetRenderState(EFF3DRS_TEXTUREFACTOR, color);

	device->SetRenderState(EFF3DRS_ALPHABLENDENABLE, effTRUE);
	device->SetRenderState(EFF3DRS_SRCBLEND, EFF3DBLEND_SRCALPHA);
	device->SetRenderState(EFF3DRS_DESTBLEND, EFF3DBLEND_INVSRCALPHA);

	device->SetTextureStageState(0, EFF3DTSS_COLOROP, EFF3DTOP_SELECTARG1);
	device->SetTextureStageState(0, EFF3DTSS_COLORARG1,	EFF3DTA_TFACTOR);

	device->SetTextureStageState(0, EFF3DTSS_ALPHAOP, EFF3DTOP_SELECTARG1);
	device->SetTextureStageState(0, EFF3DTSS_ALPHAARG1, EFF3DTA_TEXTURE);


	device->SetSamplerState(0, EFF3DSAMP_MINFILTER, EFF3DTEXF_POINT);
	device->SetSamplerState(0, EFF3DSAMP_MAGFILTER, EFF3DTEXF_POINT);*/

	//don't worry about redundant state change, EFF3DDevice will process it

	//effUINT fvf = QuadVertex::fvf;
	//device->SetFVF(fvf);

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

        const EFF3DImageInfo & imageInfo = device->GetTextureManager()->GetImageInfo(fontTextureHandle);
        effUINT width = imageInfo.width;
        effUINT height = imageInfo.height;

		QuadVertex buff[6];
		memset(buff, 0, sizeof(buff));
		buff[0].x = x + glyphInfo.xoffset - 0.5f;
		buff[0].y = y + glyphInfo.yoffset - 0.5f;
        buff[0].u = (effFLOAT)glyphInfo.x0 / (effFLOAT)width;
		buff[0].v = (effFLOAT)glyphInfo.y0 / (effFLOAT)height;

		buff[1].x = x + glyphInfo.xoffset + glyphWidth - 0.5f;
		buff[1].y = y + glyphInfo.yoffset + glyphHeight - 0.5f;
		buff[1].u = (effFLOAT)glyphInfo.x1 / (effFLOAT)width;
		buff[1].v = (effFLOAT)glyphInfo.y1 / (effFLOAT)height;

		buff[2].x = x + glyphInfo.xoffset + glyphWidth - 0.5f;
		buff[2].y = y + glyphInfo.yoffset - 0.5f;
		buff[2].u = (effFLOAT)glyphInfo.x1 / (effFLOAT)width;
		buff[2].v = (effFLOAT)glyphInfo.y0 / (effFLOAT)height;


		buff[3].x = x + glyphInfo.xoffset - 0.5f;
		buff[3].y = y + glyphInfo.yoffset - 0.5f;
		buff[3].u = (effFLOAT)glyphInfo.x0 / (effFLOAT)width;
		buff[3].v = (effFLOAT)glyphInfo.y0 / (effFLOAT)height;
			


		buff[4].x = x + glyphInfo.xoffset - 0.5f;
		buff[4].y = y + glyphInfo.yoffset + glyphHeight - 0.5f;
		buff[4].u = (effFLOAT)glyphInfo.x0 / (effFLOAT)width;
		buff[4].v = (effFLOAT)glyphInfo.y1 / (effFLOAT)height;


		buff[5].x = x + glyphInfo.xoffset + glyphWidth - 0.5f;
		buff[5].y = y + glyphInfo.yoffset + glyphHeight - 0.5f;
		buff[5].u = (effFLOAT)glyphInfo.x1 / (effFLOAT)width;
		buff[5].v = (effFLOAT)glyphInfo.y1 / (effFLOAT)height;

		x += glyphInfo.width;




		device->DrawPrimitiveUP(TriangleList, 2, buff, sizeof(QuadVertex));
	}

	return effTRUE;
}


effBOOL EFF3DFont::AddCodePointsToTexture(const effString & text)
{

	VECTOR<effWCHAR> unloadedCodePoints;
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

	effUINT maxWidth = 0;
	effBYTE * glyphBuffer = fontManager->LoadGlyphBitmapFromFile(fontFilePath, fontSize, unloadedCodePoints, glyphsInfo, maxWidth);
	if ( glyphBuffer == NULL )
	{
		return effFALSE;
	}

	//delete glyph buffer when function end
	ON_SCOPE_EXIT([&] { SFT_DELETE(glyphBuffer); })

	effBYTE * currentGlyphBuffer = glyphBuffer;

    EFF3DLockedRect rc;
	memset(&rc, 0, sizeof(EFF3DLockedRect));

	for ( effUINT i = 0; i < unloadedCodePoints.size(); i++ )
	{

		if ( !CheckTextureSize(unloadedCodePoints, i, rc) )
		{
			break;
		}

		EFF3DFontGlyphInfo & glyphInfo = glyphsInfo[unloadedCodePoints[i]];
		effINT fontWidth = glyphInfo.x1 - glyphInfo.x0;

		//x exceed blank area
		if ( currentX + fontWidth > blankAreaX + blankAreaWidth )
		{

			//go to next line
			if ( currentY + 2 * (effINT)fontSize <= blankAreaY + blankAreaHeight )
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

		for ( effUINT j = 0; j < fontSize; j++ )
		{
			//memcpy(((effBYTE *)rc.pBits) + currentX + rc.Pitch * (currentY + j), currentGlyphBuffer + j * maxWidth, fontWidth);
		}

		currentGlyphBuffer += maxWidth * fontSize;

		glyphInfo.x0 += currentX;
		glyphInfo.x1 += currentX;
		glyphInfo.y0 += currentY;
		glyphInfo.y1 += currentY;

		currentX += fontWidth;
	}

    EFF3DDevice * device = EFF3DGetDevice();
    device->GetTextureManager()->UnlockRect(fontTextureHandle, 0);

	return effTRUE;
}


effBOOL EFF3DFont::CheckTextureSize(const VECTOR<effWCHAR> & unloadedCodePoints, effINT index, EFF3DLockedRect & rc)
{
    EFF3DDevice * device = EFF3DGetDevice();

	if ( !fontTextureHandle.IsValid() )
	{
        //effUINT flag = EFF3DUSAGE_DYNAMIC;

		device->CreateTexture(textureWidth, textureHeight, 1, 0, EFF3D_FORMAT_A8, EFF3DResourceType_Texture2D, &fontTextureHandle, 0);

		if ( !device->GetTextureManager()->LockRect(fontTextureHandle, 0, &rc, NULL, 0) )
		{
			return effFALSE;
		}

		return effTRUE;
	}


	EFF3DFontGlyphInfo & glyphInfo = glyphsInfo[unloadedCodePoints[index]];
	
	int newX = currentX + glyphInfo.x1 - glyphInfo.x0;
	int newY = currentY + 2 * fontSize;

	//贴图需要增长大小
	if ( newX > textureWidth && newY > textureHeight )
	{
		effINT newTextureWidth = textureWidth;
		effINT newTextureHeight = textureHeight;

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

		EFF3DTexture * newFontTexture = NULL;
        EFF3DTextureHandle newFontTextureHandle;
		EFF3DDevice * device = EFF3DGetDevice();

        //effUINT flag = EFF3DUSAGE_DYNAMIC;

		device->CreateTexture(newTextureWidth, newTextureHeight, 1, 0, EFF3D_FORMAT_A8, EFF3DResourceType_Texture2D, &newFontTextureHandle, 0);



        EFF3DLockedRect newRc;
		if ( FAILED(newFontTexture->LockRect(0, &newRc, NULL, 0)) )
		{
			return effFALSE;
		}

		if ( !fontTextureHandle.IsValid() )
		{
			if ( rc.data == NULL )
			{
				if ( FAILED(device->GetTextureManager()->LockRect(fontTextureHandle, 0, &rc, NULL, 0)) )
				{
					newFontTexture->UnlockRect(0);
					SF_RELEASE(newFontTexture);
					return effFALSE;
				}
			}

			for ( effINT i = 0; i < textureHeight; i++ )
			{
				memcpy((effBYTE *)newRc.data + i * newRc.pitch, (effBYTE *)rc.data + i * rc.pitch, textureWidth);
			}
		}


		device->GetTextureManager()->UnlockRect(fontTextureHandle, 0);
        device->GetTextureManager()->Release(fontTextureHandle);
		rc = newRc;
		textureWidth = newTextureWidth;
		textureHeight = newTextureHeight;
        fontTextureHandle = newFontTextureHandle;
	}
	else
	{
		if ( rc.data == NULL )
		{
			if ( !device->GetTextureManager()->LockRect(fontTextureHandle, 0, &rc, NULL, 0) )
			{
				return effFALSE;
			}
		}
	}

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

	FontFaceInfo * fontFaceInfo = GetFontFace(fontFilePath);
	if ( fontFaceInfo != NULL )
	{
		EFF3DFont * font = EFFNEW EFF3DFont();

		font->fontSize = fontSize;
		font->fontFilePath = fontFilePath;
		font->fontManager = this;
		font->fontName = fontFaceInfo->fontName;
		font->fontLineSapcing = fontFaceInfo->face->size->metrics.height / 64;
		fonts.push_back(font);

		return font;
	}

	return NULL;
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

effUINT EFF3DFontManager::GetFont(const effString & fontName)
{
	for ( effUINT i = 0; i < fonts.size(); i++ )
	{
		if ( fonts[i]->fontName == fontName )
		{
            return i;
		}
	}

	return NULL;
}

EFFId EFF3DFontManager::GetFontTextureId(effUINT fontId)
{
    return EFFId();
}

effBYTE * EFF3DFontManager::LoadGlyphBitmapFromFile(const effString & fontFilePath, effUINT fontSize, const VECTOR<effWCHAR> & codePoints, EFF3DFontGlyphInfo * glyphsInfo, effUINT & maxWidth)
{

	FontFaceInfo * fontFaceInfo = GetFontFace(fontFilePath);

	if ( fontFaceInfo == NULL )
	{
		return NULL;
	}

	FT_Face face = fontFaceInfo->face;
		
	FT_Error error = FT_Set_Pixel_Sizes(face, 0, fontSize);




	maxWidth = face->size->metrics.max_advance / 64;
	effBYTE * glyphBuffer = EFFNEW effBYTE[codePoints.size() * maxWidth * fontSize];
	memset(glyphBuffer, 0, codePoints.size() * maxWidth * fontSize);

	effBYTE * currentGlyphBuffer = glyphBuffer;

	for ( effUINT i = 0; i < codePoints.size(); i++ )
	{
		FT_ULong charcode = codePoints[i];
		effUINT glyphIndex = FT_Get_Char_Index(face, charcode);
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

		if ( bitmap.width > maxWidth )
		{
			int t = 0;
		}

		if ( bitmap.rows > fontSize )
		{
			int t = 0;
		}

		effINT fontWidth = bitmap.width;
		//effINT fontWidth = min(bitmap.width, fontSize);
		effINT fontHeight = MIN(bitmap.rows, fontSize);

		//内嵌点阵，bitmap是每pixel 1位的黑白图
		if ( bitmap.pixel_mode == FT_PIXEL_MODE_MONO )
		{
			for ( effINT j = 0; j < fontHeight; j++ )
			{
				effBYTE * bitmapBuffer = bitmap.buffer + j * bitmap.pitch;
				effBYTE * lineStartGlyphBuffer = currentGlyphBuffer + j * maxWidth;
				effUINT x = 0;
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
				memcpy(currentGlyphBuffer + j * maxWidth, bitmap.buffer + j * bitmap.pitch, fontWidth);
			}
		}

		currentGlyphBuffer += maxWidth * fontSize;

		//codePoints[i] is a ucs-2 character, so don't need check codePoints[i] value
		EFF3DFontGlyphInfo * glyphInfo = &glyphsInfo[codePoints[i]];

		glyphInfo->x1 = fontWidth;
		glyphInfo->y1 = fontHeight;

		glyphInfo->width = (effBYTE)(face->glyph->advance.x / 64);

		glyphInfo->xoffset = face->glyph->bitmap_left;
		//face->size->metrics.ascender是基线到字体顶部的距离(包括空白)， face->glyph->bitmap_top是基线到字体顶部的距离（不包括空白）
		//渲染的时候加上glyphInfo->yoffset就可以把字体对齐到基线了
		glyphInfo->yoffset = (effINT)(face->size->metrics.ascender / 64) - face->glyph->bitmap_top;


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


	FT_Select_Charmap(face,  FT_ENCODING_UNICODE);

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

struct FontVertex
{
    effFLOAT			x, y;
    effFLOAT			u, v;
};

effBOOL EFF3DFontManager::GenerateTextRenderCmd(effUINT fontIndex, effVOID * vertexBuffer, effWORD * indexBuffer, effWORD vertexBaseIndex, effUINT strIndex, 
    effFLOAT x, effFLOAT y, effFLOAT width, effFLOAT height, effUINT color)
{

    EFF3DFont * font = fonts[fontIndex];

    EFFStaticStringManager * strManager = EFF3DGetDevice()->GetStaticStringManager();
    const effWCHAR * text = strManager->GetString(strIndex);

    font->AddCodePointsToTexture(text);


    FontVertex * buff = (FontVertex *)vertexBuffer;



    for (effUINT i = 0; i < strManager->Length(strIndex); i++)
    {
        //text[i] is a ucs-2 character, so don't need check text[i] value
        EFF3DFontGlyphInfo & glyphInfo = font->glyphsInfo[text[i]];

        //glyph bitmap don't loaded
        if (glyphInfo.y1 == 0)
        {
            continue;
        }


        effINT16 glyphWidth = glyphInfo.x1 - glyphInfo.x0;
        effINT16 glyphHeight = glyphInfo.y1 - glyphInfo.y0;

        //EFF3DTexture * fontTexture = font->fontTextureHandle;





        //memset(buff, 0, sizeof(buff));

        // 0------1
        // |      |
        // |      |
        // 2------3

        // dx9, all x, y coordinate -0.5f, to Directly Mapping Texels to Pixels
        float offset = 0.5f;

        buff[0].x = x + glyphInfo.xoffset - offset;
        buff[0].y = y + glyphInfo.yoffset - offset;
        buff[0].u = (effFLOAT)glyphInfo.x0;
        buff[0].v = (effFLOAT)glyphInfo.y0;

        buff[1].x = x + glyphInfo.xoffset + glyphWidth - offset;
        buff[1].y = y + glyphInfo.yoffset - offset;
        buff[1].u = (effFLOAT)glyphInfo.x1;
        buff[1].v = (effFLOAT)glyphInfo.y0;

        buff[2].x = x + glyphInfo.xoffset - offset;
        buff[2].y = y + glyphInfo.yoffset + glyphHeight- offset;
        buff[2].u = (effFLOAT)glyphInfo.x0;
        buff[2].v = (effFLOAT)glyphInfo.y1;


        buff[3].x = x + glyphInfo.xoffset + glyphWidth - offset;
        buff[3].y = y + glyphInfo.yoffset + glyphHeight - offset;
        buff[3].u = (effFLOAT)glyphInfo.x1;
        buff[3].v = (effFLOAT)glyphInfo.y1;


        indexBuffer[0] = vertexBaseIndex + 0;
        indexBuffer[1] = vertexBaseIndex + 1;
        indexBuffer[2] = vertexBaseIndex + 2;
        indexBuffer[3] = vertexBaseIndex + 1;
        indexBuffer[4] = vertexBaseIndex + 2;
        indexBuffer[5] = vertexBaseIndex + 3;

        indexBuffer += 6;
        buff += 4;


        x += glyphInfo.width;

    }

    return effTRUE;
}

EFF3D_END