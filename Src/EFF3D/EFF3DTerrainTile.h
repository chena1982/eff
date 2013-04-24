/********************************************************************
	created:	30:12:2009   20:35
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTerrainTile.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3DTerrainTile_2009_12_30__
#define __EFF3DTerrainTile_2009_12_30__

#include "EFF3DRenderable.h"

EFF3D_BEGIN

class EFF3DTerrainTileData;

class EFF3DTerrainTile : public EFF3DRenderable
{
	friend class EFF3DTerrain;
public:
	EFF3DTerrainTile(EFF3DTerrainTileData * tileData);
	~EFF3DTerrainTile();
public:
	virtual effVOID				Render(EFF3DDevice * device);

protected:
	//effVOID						SetTerrainTileData(EFF3DTerrainTileData * tileData) { this->tileData = tileData; }
	EFF3DTerrainTileData *		GetTerrainTileData() { return tileData; }
private:
	EFF3DTerrainTileData *		tileData;
	effINT						currentLevel;
};

EFF3D_END

#endif