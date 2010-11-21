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

#define new EFFNEW

EFF3D_BEGIN

EFF3DTerrainTile::EFF3DTerrainTile(EFF3DITerrainTileData * pTileData,EFF3DMaterial * pMaterial /* = NULL */,EFF3DIEntity * pEntity /* = NULL */) : EFF3DRenderable(pMaterial,pEntity)
{
	m_pTileData = pTileData;
	m_nCurrentLevel = 0;
}

EFF3DTerrainTile::~EFF3DTerrainTile()
{

}

effVOID EFF3DTerrainTile::Render(EFF3DDevice * pDevice)
{
	if ( m_pTileData != NULL )
	{
		pDevice->SetFVF(EFF3DFVF_XYZ);
		pDevice->SetStreamSource(0,m_pTileData->GetVertexBuffer(m_nCurrentLevel,pDevice),0,sizeof(effFLOAT)*3);
		pDevice->SetIndices(m_pTileData->GetIndexBuffer(m_nCurrentLevel,pDevice));

		pDevice->DrawIndexedPrimitive(EFF3DPT_TRIANGLESTRIP,0,0,m_pTileData->GetVerticesNum(m_nCurrentLevel),0,m_pTileData->GetIndicesNum(m_nCurrentLevel)-2);
		//pDevice->DrawIndexedPrimitive(EFF3DPT_TRIANGLELIST,0,0,m_pTileData->GetVerticesNum(m_nCurrentLevel),0,m_pTileData->GetIndicesNum(m_nCurrentLevel)/3);
	}

}


EFF3D_END