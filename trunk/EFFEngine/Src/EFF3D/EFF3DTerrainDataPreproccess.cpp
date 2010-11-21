/********************************************************************
	created:	23:12:2009   20:40
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTerrainDataPreproccess.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"

#include "EFF3DTerrain.h"
#include "EFF3DTerrainTile.h"
#include "EFF3DTerrainROAMImproveTileData.h"
#include "EFF3DTerrainDataPreproccess.h"

#define new EFFNEW

//地形网格的大小是16384*16384，分辨率是10m，贴图的大小是16385*16385
//地形tile的大小是513*513

#define TERRAIN_ELEVATION_PIXEL_SIZE	16385
#define TERRAIN_TILE_PIXEL_SIZE	129

#define TERRAIN_TILE_SPLIT_DEPTH 13 //2 * log2(128) - 1

EFF3D_BEGIN




effVOID TerrainDataPreproccess(effTCHAR * filePath,EFF3DTerrain * pTerrain)
{
	ILboolean bSuccess = ilLoadImage(filePath);

	ILuint nTileNum = (TERRAIN_ELEVATION_PIXEL_SIZE-1) / (TERRAIN_TILE_PIXEL_SIZE-1);
	for ( ILuint i = 0; i < 1; i++ )
	{
		for ( ILuint j = 0; j < 1; j++ )
		{
			ILuint uiXOff = j * (TERRAIN_TILE_PIXEL_SIZE-1);
			ILuint uiYOff = i * (TERRAIN_TILE_PIXEL_SIZE-1);
			effINT16 * pData = new effINT16[TERRAIN_TILE_PIXEL_SIZE*TERRAIN_TILE_PIXEL_SIZE];
			ilCopyPixels(uiXOff,uiYOff,0,TERRAIN_TILE_PIXEL_SIZE,TERRAIN_TILE_PIXEL_SIZE,1,IL_LUMINANCE,IL_UNSIGNED_SHORT,pData);


			EFF3DTerrainROAMImproveTileData * pTileData = new EFF3DTerrainROAMImproveTileData(j,i);


			effINT16 error[2];
			error[0] = 250;
			error[1] = 250;

			pTileData->GenerateGeometryDataFromElevationMap(pData,error,2);



			EFF3DTerrainTile * pTile = new EFF3DTerrainTile(pTileData);
			//pTerrain->AddTerrainTile(pTile);
		}
	}
}

EFF3D_END