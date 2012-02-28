/********************************************************************
	created:	23:12:2009   22:55
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTerrainROAMImproveTileData.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3DTerrainROAMImproveTileData_2009_12_23__
#define __EFF3DTerrainROAMImproveTileData_2009_12_23__

#include "EFF3DTerrainTileData.h"

EFF3D_BEGIN




class EFF3D_API EFF3DTerrainROAMImproveTileData : public EFF3DITerrainTileData
{
public:
	EFF3DTerrainROAMImproveTileData(effINT nTileX,effINT nTileZ);
	~EFF3DTerrainROAMImproveTileData();

protected:

	enum CODE
	{
		AL = 0,
		AR,
		BL,
		BR,
		CL,
		CR
	};

	struct TriTreeNode
	{
		effINT				nApexX;
		effINT				nApexZ;
		effINT				nLeftX;
		effINT				nLeftZ;
		effINT				nRightX;
		effINT				nRightZ;

		TriTreeNode *	pPre;
		TriTreeNode *	pNext;
		TriTreeNode *	pLeftChild;
		TriTreeNode *	pRightChild;
		TriTreeNode *	pBaseNeighbor;
		TriTreeNode *	pLeftNeighbor;
		TriTreeNode *	pRightNeighbor;

		CODE				code;
		effINT				nDepth;

		TriTreeNode() : pPre(NULL),pNext(NULL),pLeftChild(NULL),pRightChild(NULL),
								pBaseNeighbor(NULL),pLeftNeighbor(NULL),pRightNeighbor(NULL),nDepth(0)
		{
		}

		effVOID Set(CODE code,effINT nApexX, effINT nApexZ, effINT nLeftX, effINT nLeftZ, effINT nRightX, effINT nRightZ,TriTreeNode * pPre,TriTreeNode * pNext,
							TriTreeNode * pLeftChild,TriTreeNode * pRightChild,TriTreeNode * pLeftNeighbor,TriTreeNode * pRightNeighbor,TriTreeNode * pBaseNeighbor)
		{
			this->code = code;
			this->nApexX = nApexX;
			this->nApexZ = nApexZ;
			this->nLeftX = nLeftX;
			this->nLeftZ = nLeftZ;
			this->nRightX = nRightX;
			this->nRightZ = nRightZ;
			this->pPre = pPre;
			this->pNext = pNext;
			this->pLeftChild = pLeftChild;
			this->pRightChild = pRightChild;
			this->pLeftNeighbor = pLeftNeighbor;
			this->pRightNeighbor = pRightNeighbor;
			this->pBaseNeighbor = pBaseNeighbor;
		}

		effVOID Set(effINT nApexX, effINT nApexZ, effINT nLeftX, effINT nLeftZ, effINT nRightX, effINT nRightZ)
		{
			this->nApexX = nApexX;
			this->nApexZ = nApexZ;
			this->nLeftX = nLeftX;
			this->nLeftZ = nLeftZ;
			this->nRightX = nRightX;
			this->nRightZ = nRightZ;
		}

	};
public:
	virtual effVOID								CalculateSize();
	virtual effBOOL								Reload();
	virtual effVOID								Unload();


	virtual EFF3DVertexBuffer *			GetVertexBuffer(effINT nLevel,EFF3DDevice * pDevice);
	virtual effINT								GetVerticesNum(effINT nLevel);
	virtual effINT								GetVerticesStride();
	virtual EFF3DIndexBuffer *			GetIndexBuffer(effINT nLevel,EFF3DDevice * pDevice);
	virtual effINT								GetIndicesNum(effINT nLevel);
	virtual effINT								GetTileX() { return m_nTileX; }
	virtual effINT								GetTileZ() { return m_nTileZ; }

	effVOID										GenerateGeometryDataFromElevationMap(effINT16 * pEM,effINT16 * pError,effINT nLevel);
protected:

	effVOID										TessellateTriTree(TriTreeNode * pTriNode, effINT16 * pEM, effINT16 nError);
	//effBOOL										ReplaceTriTreeNode(TriTreeNode * pTriNode);
	effVOID										CreateTriTreeChildNode(TriTreeNode * pTriNode);
	effVOID										CreateTriTreeChildNode(TriTreeNode * pTriNode,effBOOL bLeftChildFirst,CODE firstCode,CODE secondCode);
	effVOID										SplitTriTreeNode(TriTreeNode * pTriNode);
private:
	effINT											m_nTileX;
	effINT											m_nTileZ;
	effFLOAT *									m_pVertices;
	effINT											m_nVerticesNum[4];
	effUINT16 *									m_pIndices[4];
	effINT											m_nIndicesNum[4];

	EFF3DVertexBuffer *						m_pVB;
	EFF3DIndexBuffer *						m_pIB[4];

};

EFF3D_END

#endif