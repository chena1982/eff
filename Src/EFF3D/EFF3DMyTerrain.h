/********************************************************************
	created:	25:2:2010   22:04
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DMyTerrain.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3DMyTerrain_2010_2_25__
#define __EFF3DMyTerrain_2010_2_25__

#include "EFF3DTerrain.h"

EFF3D_BEGIN


class EFF3DMyTerrainTile;

class EFF3DMyTerrain : public EFF3DTerrain
{
public:
	EFF3DMyTerrain();
	~EFF3DMyTerrain();
public:
	struct EFF3DMyTerrainNode
	{
		EFF3DMyTerrainNode()
		{
			pParent = NULL;
			memset(pChild,0,sizeof(pChild));
			nDepth = -1;
			nId = -1;
			nLeftTopX = 0;
			nLeftTopY = 0;
			pTile = NULL;

		}
		EFF3DMyTerrainNode *		pParent;
		EFF3DMyTerrainNode *		pChild[4];
		effINT								nDepth;
		effINT								nId;
		effINT								nLeftTopX;
		effINT								nLeftTopY;

		EFF3DMyTerrainTile *		pTile;
	};
public:
	effBOOL								Init();
	effVOID								Test();
public:
	virtual effVOID						Render(EFF3DDevice * pDevice);

protected:
	effVOID								SplitNode(EFF3DMyTerrainNode * pNode);
private:
	EFF3DMyTerrainNode *			m_pRootNode;
};

EFF3D_END

#endif