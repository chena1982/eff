/********************************************************************
	created:	30:12:2009   20:35
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTerrainTile.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3DTerrainTile_2009_12_30__
#define __EFF3DTerrainTile_2009_12_30__

#include "EFF3DIRenderable.h"

EFF3D_BEGIN

class EFF3DITerrainTileData;

class EFF3DTerrainTile : public EFF3DRenderable
{
public:
	EFF3DTerrainTile(EFF3DITerrainTileData * pTileData,EFF3DMaterial * pMaterial = NULL,EFF3DIEntity * pEntity = NULL);
	~EFF3DTerrainTile();
public:
	virtual effVOID							Render(EFF3DDevice * pDevice);

	EFF3DITerrainTileData *			GetTerrainTileData() { return m_pTileData; }
private:
	EFF3DITerrainTileData *			m_pTileData;
	effINT										m_nCurrentLevel;
};

EFF3D_END

#endif