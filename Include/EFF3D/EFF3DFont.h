/******************************************************************************
	created:	2013-4-23   23:30
	file path:	d:\EFF\Include\EFF3D\EFF3DFont.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DFont_2013_4_23__
#define __EFF3DFont_2013_4_23__

EFF3D_BEGIN


class EFF3DFontGlyphInfo
{
public:
	EFF3DFontGlyphInfo()
	{
		x0 = 0;
		y0 = 0;
		x1 = 0;
		y1 = 0;
		xoffset = 0;
		yoffset = 0;
	};

	~EFF3DFontGlyphInfo() {}
public:
	effINT16 x0;		//glyph left top x coordinate in texture
	effINT16 y0;
	effINT16 x1;		//glyph right bottom x coordinate in texture
	effINT16 y1;
	effCHAR xoffset;	//glyph render offset
	effCHAR yoffset;
	effBYTE width;		//glyph width, contains the interval between words
	effBYTE height;		
};

class EFF3DFontManager;
class EFF3DTexture;

class EFF3D_API EFF3DFont
{
	friend class EFF3DFontManager;
protected:
	EFF3DFont();
	~EFF3DFont();
public:

	effBOOL DrawText(const effString & text, effINT & x, effINT & y);

protected:
	effBOOL AddCodePointsToTexture(const effString & text);
protected:
	effString			fontName;
	effString			fontFilePath;
	effINT				fontSize;
	EFF3DFontGlyphInfo	glyphsInfo[0x10000];
	EFF3DFontManager *	fontManager;
	EFF3DTexture *		fontTexture;
	effINT				fontCount;
	effINT				maxFontCount;
	effINT				textureWidth;
	effINT				textureHeight;

	effINT				blankAreaX;
	effINT				blankAreaY;
	effINT				blankAreaWidth;
	effINT				blankAreaHeight;

	effINT				currentX;
	effINT				currentY;

};

class EFF3D_API EFF3DFontManager
{
	friend class EFF3DFont;
public:
	EFF3DFontManager();
	~EFF3DFontManager();

public:
	EFF3DFont *	CreateFromFile(const effString & fontFilePath, effINT fontSize);
	effVOID		ReleaseFont(EFF3DFont * font);
protected:
	effBYTE *	LoadGlyphBitmapFromFile(const effString & fontFilePath, effINT fontSize, std::vector<effWCHAR> codePoints, EFF3DFontGlyphInfo * glyphsInfo);

protected:
	std::vector<EFF3DFont *>	fonts;
};


EFF3D_END


#endif