/********************************************************************
	created:	9:12:2009   21:06
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTerrain.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"

#include "EFF3DTerrainDataPreproccess.h"
#include "EFF3DTerrainTileData.h"
#include "EFF3DTerrainTile.h"
#include "EFF3DDevice.h"
#include "EFF3DTerrain.h"
#include "EFF3DMyTerrain.h"

//#define new EFFNEW

EFF3D_BEGIN

EFF3DTerrain::EFF3DTerrain()
{
	/*m_ppTerrainTiles = new EFF3DTerrainTile *[nTileNumX*nTileNumZ];

	memset(m_ppTerrainTiles,0,sizeof(EFF3DTerrainTile *)*nTileNumX*nTileNumZ);

	m_nTileNumX = nTileNumX;
	m_nTileNumZ = nTileNumZ;*/
	
}

EFF3DTerrain::~EFF3DTerrain()
{

}

/*effVOID EFF3DTerrain::AddTerrainTile(EFF3DTerrainTile * pTerrainTile)
{
	EFF3DITerrainTileData * pTileData = pTerrainTile->GetTerrainTileData();
	effINT nIndex = pTileData->GetTileZ() * m_nTileNumX + pTileData->GetTileX();

	m_ppTerrainTiles[nIndex] = pTerrainTile;
}

effVOID EFF3DTerrain::Render(EFF3DDevice * pDevice)
{
	m_ppTerrainTiles[0]->Render(pDevice);
}*/

effVOID TerrainPreproccess(effTCHAR * filePath,EFF3DTerrain * pTerrain)
{
	TerrainDataPreproccess(filePath,pTerrain);
}


effVOID TestMyTerrain()
{

	EFF3DMyTerrain * pTerrain = new EFF3DMyTerrain();
	pTerrain->Init();
	pTerrain->Test();
}

EFF3D_END