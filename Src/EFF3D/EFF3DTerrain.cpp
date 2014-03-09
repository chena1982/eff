/********************************************************************
	created:	9:12:2009   21:06
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTerrain.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"

#include "EFF3DTerrainDataPreprocess.h"
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

	width = 0;
	height = 0;
	tileWidth = 0;
	horizontalResolution = 0.0f;


	tileNumX = 0;
	tileNumZ = 0;

	tiles = NULL;
	
}

EFF3DTerrain::~EFF3DTerrain()
{

}

effVOID EFF3DTerrain::LoadFromFile(const effString & filePath)
{
	TerrainPreprocess(filePath, this);
}


effVOID	EFF3DTerrain::Init(effINT width, effINT height, effINT tileWidth, effFLOAT horizontalResolution)
{
	this->width = width;
	this->height = height;
	this->tileWidth = tileWidth;
	this->horizontalResolution = horizontalResolution;

	tileNumX = width / tileWidth;
	tileNumZ = width / tileWidth;

	tiles = EFFNEW EFF3DTerrainTile *[tileNumX * tileNumZ];
	memset(tiles, 0, tileNumX * tileNumZ * sizeof(EFF3DTerrainTile *));
}



effVOID EFF3DTerrain::AddTile(EFF3DTerrainTileData * tileData)
{
	EFF3DTerrainTile * tile = EFFNEW EFF3DTerrainTile(tileData);

	effINT tileIndex = tileData->GetTileZ() * tileNumX + tileData->GetTileX();
	tiles[tileIndex] = tile;
}

effVOID EFF3DTerrain::Render(EFF3DDevice * pDevice)
{
	tiles[0]->Render(pDevice);
}

effVOID TerrainPreprocess(const effString & filePath, EFF3DTerrain * pTerrain)
{
	TerrainDataPreprocess(filePath, pTerrain);
}


EFF3DTerrain * TestMyTerrain()
{

	/*EFF3DMyTerrain * pTerrain = new EFF3DMyTerrain();
	pTerrain->Init();
	pTerrain->Test();*/

	EFF3DTerrain * pTerrain = new EFF3DTerrain();
	pTerrain->Init(64, 64, 32, 1.0f);
	TerrainPreprocess(_effT("Data\\Terrain\\heightmap.tif"), pTerrain);

	return pTerrain;
}



EFF3D_END