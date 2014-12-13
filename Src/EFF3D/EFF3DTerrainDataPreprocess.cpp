/********************************************************************
	created:	23:12:2009   20:40
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTerrainDataPreprocess.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "EFF3DPCH.h"

#include "EFF3DTerrain.h"
#include "EFF3DTerrainTile.h"
#include "EFF3DTerrainROAMImproveTileData.h"
#include "EFF3DTerrainDataPreprocess.h"

//#define new EFFNEW


EFF3D_BEGIN




effVOID TerrainDataPreprocess(const effString & filePath, EFF3DTerrain * pTerrain)
{

	effFLOAT mapSizeY = 400.0f;

	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);


	ILboolean bSuccess = ilLoadImage(filePath.c_str());

	if ( !bSuccess )
	{
		ILenum error = ilGetError();
		return;
	}

	ILuint nTileNum = (TERRAIN_ELEVATION_PIXEL_SIZE - 1) / (TERRAIN_TILE_PIXEL_SIZE - 1);


	ILint width = ilGetInteger(IL_IMAGE_WIDTH);
	ILint height = ilGetInteger(IL_IMAGE_HEIGHT);
	ILint depth = ilGetInteger(IL_IMAGE_DEPTH);
	ILint format = ilGetInteger(IL_IMAGE_FORMAT);
	ILint bpp = ilGetInteger(IL_IMAGE_BPP);
	ILint type = ilGetInteger(IL_IMAGE_TYPE);

	for ( ILuint z = 0; z < 1; z++ )
	{
		for ( ILuint x = 0; x < 1; x++ )
		{
			ILuint uiXOff = x * (TERRAIN_TILE_PIXEL_SIZE - 1);
			ILuint uiYOff = z * (TERRAIN_TILE_PIXEL_SIZE - 1);
			effUINT16 * pData = EFFNEW effUINT16[TERRAIN_TILE_PIXEL_SIZE * TERRAIN_TILE_PIXEL_SIZE];
			ilCopyPixels(uiXOff, uiYOff, 0, TERRAIN_TILE_PIXEL_SIZE, TERRAIN_TILE_PIXEL_SIZE, depth, format, type, pData);


			EFF3DTerrainROAMImproveTileData * pTileData = EFFNEW EFF3DTerrainROAMImproveTileData(x, z);


			effUINT16 error[2];
			error[0] = 1;
			error[1] = 250;




			pTileData->GenerateGeometryDataFromElevationMap(pData, error, 1);


			pTerrain->AddTile(pTileData);
		}
	}

	ilDeleteImages(1, &imageID);
}

EFF3D_END