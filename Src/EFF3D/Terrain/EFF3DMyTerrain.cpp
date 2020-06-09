/********************************************************************
	created:	25:2:2010   22:10
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DMyTerrain.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "../EFF3DPCH.h"
#include "EFF3DMyTerrain.h"

//#define new EFFNEW

EFF3D_BEGIN

//��������Ĵ�С��16384*16384���ֱ�����10m����ͼ�Ĵ�С��16385*16385


#define TERRAIN_ELEVATION_PIXEL_SIZE	16385
#define TERRAIN_TILE_PIXEL_SIZE	65

#define TERRAIN_TILE_SPLIT_DEPTH  8//log2(16384) - log2(64)

EFF3DMyTerrain::EFF3DMyTerrain()
{

}

EFF3DMyTerrain::~EFF3DMyTerrain()
{
}

effBOOL EFF3DMyTerrain::Init()
{
	m_pRootNode = new EFF3DMyTerrainNode();
	m_pRootNode->nDepth = 1;
	m_pRootNode->nId = 0;
	
	
	return effTRUE;
}

effVOID EFF3DMyTerrain::SplitNode(EFF3DMyTerrainNode * pNode)
{

	static effINT nXOffset[4] = { 0, 1, 0, 1 };
	static effINT nYOffset[4] = { 0, 0, 1, 1 };

	if ( pNode->nDepth >=3 )
	{
		return;
	}

	//���ΪpNode->nDepth-1����ȫ�Ĳ����Ľڵ�ĸ���
	effINT nNodeNum1 = ( (effINT)pow(4.f,pNode->nDepth-1) - 1 ) / ( 4 - 1 );
	//���ΪpNode->nDepth����ȫ�Ĳ����Ľڵ�ĸ���
	effINT nNodeNum2 = ( (effINT)pow(4.f,pNode->nDepth) - 1 ) / ( 4 - 1 );

	effINT nHalfLength = (TERRAIN_TILE_PIXEL_SIZE - 1) * (effINT)pow(2.f, (TERRAIN_TILE_SPLIT_DEPTH - pNode->nDepth));

	for ( effINT i = 0; i < 4; i++ )
	{
		EFF3DMyTerrainNode * pChild = new EFF3DMyTerrainNode();
		pChild->nDepth = pNode->nDepth + 1;
		//pNode��ͬһ�ȼ���Node�е�����
		//�ȱ����еĺ�Sn = (a1 * (1-q^n)) / (1-q)�����У�nΪ������qΪ���a1Ϊ���
		


		pChild->nId = nNodeNum2 + (pNode->nId - nNodeNum1) * 4 + i;


		pChild->nLeftTopX = pNode->nLeftTopX + nHalfLength * nXOffset[i];
		pChild->nLeftTopY = pNode->nLeftTopY + nHalfLength * nYOffset[i];

		pNode->pChild[i] = pChild;
		pChild->pParent = pNode;

		SplitNode(pChild);
	}


}

effVOID EFF3DMyTerrain::Test()
{
	SplitNode(m_pRootNode);
}

effVOID EFF3DMyTerrain::Render(EFF3DDevice * pDevice)
{
}


EFF3D_END