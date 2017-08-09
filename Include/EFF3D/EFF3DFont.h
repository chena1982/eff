/******************************************************************************
	created:	2013-4-23   23:30
	file path:	d:\EFF\Include\EFF3D\EFF3DFont.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DFont_2013_4_23__
#define __EFF3DFont_2013_4_23__



struct FT_FaceRec_;

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

    effBOOL         DrawText(const effString & text, effINT & x, effINT & y, effUINT color);

protected:
	effBOOL AddCodePointsToTexture(const effString & text);
	effBOOL	CheckTextureSize(const VECTOR<effWCHAR> & unloadedCodePoints, effINT index, EFF3DLOCKED_RECT & rc);
protected:
	effString			fontName;
	effString			fontFilePath;
	effUINT				fontSize;
	effINT				fontLineSapcing;

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
	EFF3DFont *		CreateFromFile(const effString & fontFilePath, effINT fontSize);
	effVOID			ReleaseFont(EFF3DFont * font);
	effUINT		    GetFont(const effString & fontName);

    EFFId           GetFontTextureId(effUINT fontId);
    effBOOL         GenerateTextRenderCmd(effUINT fontIndex, effVOID * vertexBuffer, effWORD * indexBuffer, effWORD vertexBaseIndex, effUINT strIndex, effFLOAT x, effFLOAT y, effFLOAT width, effFLOAT height, effUINT color);
protected:
	class FontFaceInfo
	{
	public:
		FontFaceInfo()
		{
			face = NULL;
			buffer = NULL;
		};

		FontFaceInfo(const FontFaceInfo & rhs)
		{
			fontFilePath = rhs.fontFilePath;
			face = rhs.face;
			buffer = rhs.buffer;
			fontName = rhs.fontName;
		}
	public:
		effString		fontFilePath;
		FT_FaceRec_ *	face;
		effBYTE *		buffer;
		effString		fontName;
	};
protected:
	effBYTE *		LoadGlyphBitmapFromFile(const effString & fontFilePath, effUINT fontSize, const VECTOR<effWCHAR> & codePoints, EFF3DFontGlyphInfo * glyphsInfo, effUINT & maxWidth);
	FontFaceInfo *	GetFontFace(const effString & fontFilePath);
protected:
	VECTOR<EFF3DFont *>		fonts;
	VECTOR<FontFaceInfo>	fontsFaceInfo;
};


EFF3D_END


#endif