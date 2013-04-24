/********************************************************************
	created:	23:12:2009   21:14
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DTerrain.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3DTerrain_2009_12_23__
#define __EFF3DTerrain_2009_12_23__

EFF3D_BEGIN

class EFF3DTerrainTile;
class EFF3DDevice;
class EFF3DTerrainTileData;

class EFF3D_API EFF3DTerrain
{
public:
	EFF3DTerrain();
	~EFF3DTerrain();

public:
	//effVOID				AddTerrainTile(EFF3DTerrainTile * pTerrainTile);
	effVOID					LoadFromFile(const effString & filePath);
	virtual effVOID			Render(EFF3DDevice * pDevice);


	effVOID					AddTile(EFF3DTerrainTileData * tileData);

	effVOID					Init(effINT width, effINT height, effINT tileWidth, effFLOAT horizontalResolution);

protected:

	effINT					width;
	effINT					height;
	effINT					tileWidth;
	effFLOAT				horizontalResolution;

	effINT					tileNumX;
	effINT					tileNumZ;

	EFF3DTerrainTile **		tiles;

};

EFF3D_API effVOID TerrainPreprocess(const effString & filePath, EFF3DTerrain * pTerrain);

EFF3D_API EFF3DTerrain * TestMyTerrain();

EFF3D_END

#endif


