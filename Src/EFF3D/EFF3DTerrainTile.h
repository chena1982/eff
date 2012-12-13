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

class EFF3DITerrainTileData;

class EFF3DTerrainTile : public EFF3DRenderable
{
public:
	EFF3DTerrainTile(EFF3DITerrainTileData * tileData, EFF3DMaterial * material = NULL, EFF3DObject * object = NULL);
	~EFF3DTerrainTile();
public:
	virtual effVOID					Render(EFF3DDevice * device);
	EFF3DITerrainTileData *			GetTerrainTileData() { return tileData; }
private:
	EFF3DITerrainTileData *			tileData;
	effINT							currentLevel;
};

EFF3D_END

#endif