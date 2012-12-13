/********************************************************************
	created:	30:12:2009   20:38
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTerrainTile.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"

#include "EFF3DDevice.h"
#include "EFF3DTerrainTileData.h"
#include "EFF3DTerrainTile.h"

//#define new EFFNEW

EFF3D_BEGIN

EFF3DTerrainTile::EFF3DTerrainTile(EFF3DITerrainTileData * tileData, EFF3DMaterial * material /* = NULL */, EFF3DObject * object /* = NULL */) : EFF3DRenderable()
{
	this->tileData = tileData;
	currentLevel = 0;
}

EFF3DTerrainTile::~EFF3DTerrainTile()
{

}

effVOID EFF3DTerrainTile::Render(EFF3DDevice * device)
{
	if ( tileData != NULL )
	{
		device->SetFVF(EFF3DFVF_XYZ);
		device->SetStreamSource(0, tileData->GetVertexBuffer(currentLevel, device), 0, sizeof(effFLOAT)*3);
		device->SetIndices(tileData->GetIndexBuffer(currentLevel, device));

		device->DrawIndexedPrimitive(EFF3DPT_TRIANGLESTRIP, 0, 0, tileData->GetVerticesNum(currentLevel), 0, tileData->GetIndicesNum(currentLevel)-2);
		//pDevice->DrawIndexedPrimitive(EFF3DPT_TRIANGLELIST,0,0,m_pTileData->GetVerticesNum(m_nCurrentLevel),0,m_pTileData->GetIndicesNum(m_nCurrentLevel)/3);
	}

}


EFF3D_END