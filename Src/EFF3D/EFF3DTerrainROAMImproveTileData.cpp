/********************************************************************
	created:	23:12:2009   22:56
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTerrainROAMImproveTileData.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DDevice.h"
#include "EFF3DVertexBuffer.h"
#include "EFF3DIndexBuffer.h"
#include "EFF3DTerrainROAMImproveTileData.h"

//#define new EFFNEW

EFF3D_BEGIN



EFF3DTerrainROAMImproveTileData::EFF3DTerrainROAMImproveTileData(effINT nTileX, effINT nTileZ)
{
	m_nTileX = nTileX;
	m_nTileZ = nTileZ;

	m_pVB = NULL;

	memset(m_pIB, 0, sizeof(EFF3DIndexBuffer *) * 4);


}

EFF3DTerrainROAMImproveTileData::~EFF3DTerrainROAMImproveTileData()
{
}

effVOID EFF3DTerrainROAMImproveTileData::CalculateSize()
{

}

effBOOL EFF3DTerrainROAMImproveTileData::Reload()
{
	return effTRUE;
}

effBOOL EFF3DTerrainROAMImproveTileData::Unload()
{
	return effTRUE;
}

effBOOL IsVertexOnTileEdge(effINT nX,effINT nZ)
{
	if ( nX == 0 || nX == TERRAIN_TILE_PIXEL_SIZE - 1 || 
		 nZ == 0 || nZ == TERRAIN_TILE_PIXEL_SIZE - 1 )
	{
		return effTRUE;
	}

	return effFALSE;
}

effVOID	EFF3DTerrainROAMImproveTileData::CalculateVariance(effUINT16 * pVariance)
{

}

effVOID EFF3DTerrainROAMImproveTileData::GenerateGeometryDataFromElevationMap(effUINT16 * pEM, effUINT16 * pError, effINT nLevel)
{
	effBOOL * pVerticesUsed = new effBOOL[TERRAIN_TILE_PIXEL_SIZE * TERRAIN_TILE_PIXEL_SIZE];
	memset(m_nIndicesNum, 0, sizeof(effINT) * 4);
	memset(m_nVerticesNum, 0, sizeof(effINT) * 4);

	for ( effINT i = 0; i < nLevel; i++ )
	{

		TriTreeNode * pLeftNode = new TriTreeNode();
		TriTreeNode * pRightNode = new TriTreeNode();
		TriTreeNode * pTopNode = new TriTreeNode();
		TriTreeNode * pBottomNode = new TriTreeNode();

		effINT nGridNum = TERRAIN_TILE_PIXEL_SIZE - 1;

		effINT nCenterX = nGridNum / 2;
		effINT nCenterZ = nGridNum / 2;






		pLeftNode->Set(AL, nCenterX, nCenterZ, 0, 0, 0, nGridNum, pTopNode, NULL, NULL, NULL, pTopNode, pBottomNode, NULL);
		pBottomNode->Set(AL, nCenterX, nCenterZ, 0, nGridNum, nGridNum, nGridNum, NULL, pRightNode, NULL, NULL, pLeftNode, pRightNode, NULL);
		pRightNode->Set(AL, nCenterX, nCenterZ, nGridNum, nGridNum, nGridNum, 0, pBottomNode, pTopNode, NULL, NULL, pBottomNode, pTopNode, NULL);
		pTopNode->Set(AL, nCenterX, nCenterZ, nGridNum, 0, 0, 0, pRightNode, pLeftNode, NULL, NULL, pRightNode, pLeftNode, NULL);



		effINT varianceCount = TERRAIN_TILE_VARIANCE_NODE_NUM;

		effUINT16 * pLeftVariance = EFFNEW effUINT16[varianceCount];
		memset(pLeftVariance, 0, sizeof(effUINT16) * varianceCount);
		CalculateVariance(pEM, pLeftVariance, pLeftNode->nApexX, pLeftNode->nApexZ, pLeftNode->nLeftX, pLeftNode->nLeftZ,
						  pLeftNode->nRightX, pLeftNode->nRightZ, 1);

		effUINT16 * pBottomVariance = EFFNEW effUINT16[varianceCount];
		memset(pBottomVariance, 0, sizeof(effUINT16) * varianceCount);
		CalculateVariance(pEM, pBottomVariance, pBottomNode->nApexX, pBottomNode->nApexZ, pBottomNode->nLeftX, pBottomNode->nLeftZ,
						  pBottomNode->nRightX, pBottomNode->nRightZ, 1);

		effUINT16 * pRightVariance = EFFNEW effUINT16[varianceCount];
		memset(pRightVariance, 0, sizeof(effUINT16) * varianceCount);
		CalculateVariance(pEM, pRightVariance, pRightNode->nApexX, pRightNode->nApexZ, pRightNode->nLeftX, pRightNode->nLeftZ,
						  pRightNode->nRightX, pRightNode->nRightZ, 1);

		effUINT16 * pTopVariance = EFFNEW effUINT16[varianceCount];
		memset(pTopVariance, 0, sizeof(effUINT16) * varianceCount);
		CalculateVariance(pEM, pTopVariance, pTopNode->nApexX, pTopNode->nApexZ, pTopNode->nLeftX, pTopNode->nLeftZ,
						  pTopNode->nRightX, pTopNode->nRightZ, 1);



		pLeftNode->pVariance = pLeftVariance;
		pBottomNode->pVariance = pBottomVariance;
		pRightNode->pVariance = pRightVariance;
		pTopNode->pVariance = pTopVariance;


		TessellateTriTree(pBottomNode, pEM, pError[i]);

		TriTreeNode * pFirstNode = pBottomNode;
		while (pFirstNode->pPre != NULL)
		{
			pFirstNode = pFirstNode->pPre;
		}
		

		memset(pVerticesUsed, 0, sizeof(effBOOL) * TERRAIN_TILE_PIXEL_SIZE * TERRAIN_TILE_PIXEL_SIZE);

		std::vector<effUINT16> aryIndices;

		TriTreeNode * pPreNode = NULL;
		TriTreeNode * pNode = pFirstNode;
		while ( pNode != NULL )
		{
			effINT nIndex[3];

			nIndex[0] = pNode->nLeftZ * TERRAIN_TILE_PIXEL_SIZE + pNode->nLeftX;
			nIndex[1] = pNode->nApexZ * TERRAIN_TILE_PIXEL_SIZE + pNode->nApexX;
			nIndex[2] = pNode->nRightZ * TERRAIN_TILE_PIXEL_SIZE + pNode->nRightX;

			//add first triangle' three vertex
			if ( pPreNode == NULL )
			{
				for ( effINT j = 0; j < 3; j++ )
				{
					aryIndices.push_back((effUINT16)nIndex[j]);
					pVerticesUsed[nIndex[j]] = effTRUE;
					m_nIndicesNum[i]++;
				}
			}
			else
			{
				effINT nPreIndex[3];
				nPreIndex[0] = pPreNode->nLeftZ * TERRAIN_TILE_PIXEL_SIZE + pPreNode->nLeftX;
				nPreIndex[1] = pPreNode->nApexZ * TERRAIN_TILE_PIXEL_SIZE + pPreNode->nApexX;
				nPreIndex[2] = pPreNode->nRightZ * TERRAIN_TILE_PIXEL_SIZE + pPreNode->nRightX;


				for ( effINT j = 0; j < 3; j++ )
				{
		
					if ( nIndex[j] != nPreIndex[0] && nIndex[j] != nPreIndex[1] && nIndex[j] != nPreIndex[2]  )
					{
						//from the second triangle, we need to check if we should add a degenerate edge

						static effINT nSharedEdgeVertexIndex[3][2] = { {1,2}, {0,2}, {0,1} };
						effINT nSize = (effINT)aryIndices.size();

						effINT nSharedEdgeVertexIndex0 = nIndex[nSharedEdgeVertexIndex[j][0]];
						effINT nSharedEdgeVertexIndex1 = nIndex[nSharedEdgeVertexIndex[j][1]];

						//if the last edge(aryIndices[nSize-1], aryIndices[nSize-2]) in the triangle stripe(aryIndices) is not the shared edge
						//(nIndex[nSharedEdgeVertexIndex[j][0]], nIndex[nSharedEdgeVertexIndex[j][1]])
						//so we need add a degenerate edge
						if ( !(((nSharedEdgeVertexIndex0 == aryIndices[nSize-1]) && (nSharedEdgeVertexIndex1 == aryIndices[nSize-2])) ||
							((nSharedEdgeVertexIndex1 == aryIndices[nSize-1]) && (nSharedEdgeVertexIndex0 == aryIndices[nSize-2]))) )
						{
							TriTreeNode * pNextNode = pNode->pNext;

							if ( pNextNode != NULL )
							{
								effINT nNextIndex[3];

								nNextIndex[0] = pNextNode->nLeftZ*TERRAIN_TILE_PIXEL_SIZE + pNextNode->nLeftX;
								nNextIndex[1] = pNextNode->nApexZ*TERRAIN_TILE_PIXEL_SIZE + pNextNode->nApexX;
								nNextIndex[2] = pNextNode->nRightZ*TERRAIN_TILE_PIXEL_SIZE + pNextNode->nRightX;

								effBOOL bVertex0First = effTRUE;
								for ( effINT k = 0; k < 3; k++ )
								{
									if ( nNextIndex[k] == nSharedEdgeVertexIndex0 )
									{
										bVertex0First = effFALSE;
										break;
									}
								}

								if ( bVertex0First )
								{
									aryIndices.push_back(nSharedEdgeVertexIndex0);
									aryIndices.push_back(nSharedEdgeVertexIndex1);
									m_nIndicesNum[i] += 2;
								}
								else
								{
									aryIndices.push_back(nSharedEdgeVertexIndex1);
									aryIndices.push_back(nSharedEdgeVertexIndex0);
									m_nIndicesNum[i] += 2;
								}
							}
							//we don't care the vertex sequence
							else
							{
								aryIndices.push_back(nSharedEdgeVertexIndex0);
								aryIndices.push_back(nSharedEdgeVertexIndex1);
								m_nIndicesNum[i] += 2;
							}
						}

						aryIndices.push_back((effUINT16)nIndex[j]);
						//pVerticesUsed[nIndex[j]] = effTRUE;
						m_nIndicesNum[i]++;
					}
				}
			}

			pPreNode = pNode;
			pNode = pNode->pNext;
		}

		m_pIndices[i] = EFFNEW effUINT16[m_nIndicesNum[i]];
		memcpy(m_pIndices[i], &aryIndices[0], sizeof(effUINT16) * m_nIndicesNum[i]);

		pNode = pFirstNode;		
		while ( pNode != NULL )
		{
			TriTreeNode * pNext = pNode->pNext;
			SF_DELETE(pNode);
			pNode = pNext;
		}

	}



	effINT * pGeneratedVerticesId = EFFNEW effINT[TERRAIN_TILE_PIXEL_SIZE * TERRAIN_TILE_PIXEL_SIZE];
	memset(pGeneratedVerticesId, -1, sizeof(effINT)*TERRAIN_TILE_PIXEL_SIZE * TERRAIN_TILE_PIXEL_SIZE);
	effINT nGeneratedVerticesNum = 0;

	std::vector<effFLOAT> aryVertices;

	for ( effINT i = 0; i < nLevel; i++ )
	{
		for ( effINT j = 0; j < m_nIndicesNum[i]; j++ )
		{


			effINT nIndex = (effINT)m_pIndices[i][j];

			if ( pGeneratedVerticesId[nIndex] == -1 )
			{
				effINT nX = nIndex % TERRAIN_TILE_PIXEL_SIZE;
				effINT nZ = -nIndex / TERRAIN_TILE_PIXEL_SIZE;
				aryVertices.push_back((effFLOAT)nX);
				aryVertices.push_back(((effFLOAT)pEM[nIndex]) * 0.01f);
				aryVertices.push_back((effFLOAT)nZ);

				m_pIndices[i][j] = (effUINT16)nGeneratedVerticesNum;
				pGeneratedVerticesId[nIndex] = nGeneratedVerticesNum;

				nGeneratedVerticesNum++;
			}
			else
			{
				m_pIndices[i][j] = (effUINT16)pGeneratedVerticesId[nIndex];
			}
		}
		m_nVerticesNum[i] = nGeneratedVerticesNum;
	}

	m_pVertices = EFFNEW effFLOAT[sizeof(effFLOAT) * 3* nGeneratedVerticesNum];
	memcpy(m_pVertices, &aryVertices[0], sizeof(effFLOAT) * 3 * nGeneratedVerticesNum);

	SFT_DELETE(pVerticesUsed);
	SFT_DELETE(pGeneratedVerticesId);

}

effVOID EFF3DTerrainROAMImproveTileData::TessellateTriTree(TriTreeNode * pTriNode, effUINT16 * pEM, effUINT16 error)
{

	/*TriTreeNode * pNode = pTriNode;
	do
	{
		pNode = RealTessellateTriTree(pNode, pEM, error);
	}
	while ( pNode != NULL );*/

	RealTessellateTriTree(pTriNode, pEM, error);
}

EFF3DTerrainROAMImproveTileData::TriTreeNode * EFF3DTerrainROAMImproveTileData::RealTessellateTriTree(TriTreeNode * pTriNode, effUINT16 * pEM, effUINT16 error)
{
	if ( pTriNode == NULL )
	{
		return NULL;
	}

	if ( pTriNode->nDepth >= TERRAIN_TILE_SPLIT_DEPTH )
	{
		return pTriNode->pNext;
	}

	effINT nCenterX = (pTriNode->nLeftX + pTriNode->nRightX) >> 1;
	effINT nCenterZ = (pTriNode->nLeftZ + pTriNode->nRightZ) >> 1;

	effINT nIndexLeft = pTriNode->nLeftZ * TERRAIN_TILE_PIXEL_SIZE + pTriNode->nLeftX;
	effINT nIndexRight = pTriNode->nRightZ * TERRAIN_TILE_PIXEL_SIZE + pTriNode->nRightX;
	effINT nIndexCenter = nCenterZ * TERRAIN_TILE_PIXEL_SIZE + nCenterX;


	//effUINT16 centerY = pEM[nIndexCenter];
	//effUINT16 interpolateY = (pEM[nIndexLeft] + pEM[nIndexRight]) >> 1;
	//effUINT16 tolerance = interpolateY > centerY ? interpolateY - centerY : centerY - interpolateY;
	effUINT16 tolerance = pTriNode->pVariance[pTriNode->varianceIndex - 1];

	/*if ( tolerance > error )
	{
		TriTreeNode * pFirstNode = ReplaceTriTreeNode(pTriNode);

		return pFirstNode;
	}
	else
	{
		return pTriNode->pNext;
	}*/

	if ( tolerance > error )
	{
		TriTreeNode * pFirstNode = SplitTriTreeNode(pTriNode);
		RealTessellateTriTree(pFirstNode, pEM, error);
	}
	else
	{
		RealTessellateTriTree(pTriNode->pNext, pEM, error);
	}

	return NULL;
}

effUINT16 EFF3DTerrainROAMImproveTileData::CalculateVariance(effUINT16 * pEM, effUINT16 * pVariance, effINT nApexX, effINT nApexZ, effINT nLeftX, effINT nLeftZ, effINT nRightX, effINT nRightZ, effINT nNodeIndex)
{
	effINT nCenterZ = (nLeftZ + nRightZ) >> 1;
	effINT nCenterX = (nLeftX + nRightX) >> 1;

	effUINT16 leftY = pEM[nLeftZ * TERRAIN_TILE_PIXEL_SIZE + nLeftX];
	effUINT16 rightY = pEM[nRightZ * TERRAIN_TILE_PIXEL_SIZE + nRightX];

	// Variance of this triangle is the actual height at it's 
	// hypotenuse midpoint minus the interpolated height.

	effUINT16 centerY = pEM[nCenterZ * TERRAIN_TILE_PIXEL_SIZE + nCenterX];
	effUINT16 interpolateY = (leftY + rightY) >> 1;
	effUINT16 variance = interpolateY > centerY ? interpolateY - centerY : centerY - interpolateY;

	if ( nNodeIndex <= TERRAIN_TILE_VARIANCE_BRANCH_NODE_NUM )
	{
		// Compute down the variance tree
		// Final Variance for this node is the max of it's own variance and that of it's children.
		effUINT16 childVariance = CalculateVariance(pEM, pVariance, nCenterX, nCenterZ, nApexX, nApexZ, nLeftX, nLeftZ, nNodeIndex << 1);
		if ( childVariance > variance )
		{
			variance = childVariance;
		}

		childVariance = CalculateVariance(pEM, pVariance, nCenterX, nCenterZ, nRightX, nRightZ, nApexX, nApexZ, (nNodeIndex << 1) + 1);
		if ( childVariance > variance )
		{
			variance = childVariance;
		}
	}

	// Store the final variance for this node. Note Variance is never zero.
	pVariance[nNodeIndex - 1] = variance;
	return variance;
}

EFF3DTerrainROAMImproveTileData::TriTreeNode * EFF3DTerrainROAMImproveTileData::ReplaceTriTreeNode(TriTreeNode * pTriNode)
{

	switch (pTriNode->code)
	{
	case AL:
		SplitTriTreeNode(pTriNode, effTRUE, CL, BL);
		return pTriNode;
	case AR:
		SplitTriTreeNode(pTriNode, effFALSE, CR, BR);
		return pTriNode->pPre;
	case CL:
		SplitTriTreeNode(pTriNode, effFALSE, AL, BR);
		return pTriNode->pPre;
	case CR:
		SplitTriTreeNode(pTriNode, effTRUE, AR, BL);
		return pTriNode;
	case BL:
		SplitTriTreeNode(pTriNode, effFALSE, CR, AL);
		return pTriNode->pPre;
	case BR:
		SplitTriTreeNode(pTriNode, effTRUE, CL, AR);
		return pTriNode;
	default:
		return NULL;
	}
}

EFF3DTerrainROAMImproveTileData::TriTreeNode * EFF3DTerrainROAMImproveTileData::SplitTriTreeNode(TriTreeNode * pTriNode)
{
	if ( pTriNode->pLeftChild != NULL )
	{
		return NULL;
	}

	// 1. If not part of a diamond, force-split the base neighbor before splitting the current node.
	TriTreeNode * pTriBase = pTriNode->pBaseNeighbor;
	if ( pTriBase != NULL && pTriBase->pBaseNeighbor != pTriNode && pTriNode->nDepth > pTriBase->nDepth )
	{
		SplitTriTreeNode(pTriBase);
		pTriBase = pTriNode->pBaseNeighbor;
	}

	// 2. Create children and link into mesh
	/*TriTreeNode	* pLeftChild = pTriNode->pLeftChild = new TriTreeNode();
	TriTreeNode	* pRightChild = pTriNode->pRightChild = new TriTreeNode();

	CreateTriTreeChildNode(pTriNode);*/

	TriTreeNode	* pLeftNeighbor = pTriNode->pLeftNeighbor;

	TriTreeNode	* pLeftNeighborBaseNeighbor = NULL;
	TriTreeNode * pLeftNeighborLeftNeighbor = NULL;
	TriTreeNode * pLeftNeighborRightNeighbor = NULL;
	if ( pLeftNeighbor != NULL )
	{
		pLeftNeighborBaseNeighbor = pLeftNeighbor->pBaseNeighbor;
		pLeftNeighborLeftNeighbor = pLeftNeighbor->pLeftNeighbor;
		pLeftNeighborRightNeighbor = pLeftNeighbor->pRightNeighbor;
	}


	TriTreeNode	* pRightNeighbor = pTriNode->pRightNeighbor;

	TriTreeNode	* pRightNeighborBaseNeighbor = NULL;
	TriTreeNode * pRightNeighborLeftNeighbor = NULL;
	TriTreeNode * pRightNeighborRightNeighbor = NULL;
	if ( pRightNeighbor != NULL )
	{
		pRightNeighborBaseNeighbor = pRightNeighbor->pBaseNeighbor;
		pRightNeighborLeftNeighbor = pRightNeighbor->pLeftNeighbor;
		pRightNeighborRightNeighbor = pRightNeighbor->pRightNeighbor;
	}



	TriTreeNode * pFirstNode = ReplaceTriTreeNode(pTriNode);
	TriTreeNode	* pLeftChild = pTriNode;
	TriTreeNode	* pRightChild = (pFirstNode == pTriNode) ? pFirstNode->pNext : pFirstNode;





	// 3. Fill in the neighbor information we can get from the parent
	pLeftChild->pBaseNeighbor = pLeftNeighbor;
	pLeftChild->pLeftNeighbor = pRightChild;

	pRightChild->pBaseNeighbor = pRightNeighbor;
	pRightChild->pRightNeighbor = pLeftChild;

	// 4. Link our Left Neighbor to the new children
	if ( pLeftNeighbor != NULL )
	{
		if ( pLeftNeighbor->pBaseNeighbor == pTriNode )
		{
			pLeftNeighbor->pBaseNeighbor = pLeftChild;
		}
		else if ( pLeftNeighbor->pLeftNeighbor == pTriNode )
		{
			pLeftNeighbor->pLeftNeighbor = pLeftChild;
		}
		else if ( pLeftNeighbor->pRightNeighbor == pTriNode )
		{
			pLeftNeighbor->pRightNeighbor = pLeftChild;
		}
		else
		{
			_asm int 3;// Illegal Left Neighbor!
		}
	}

	// 5. Link our Right Neighbor to the new children
	if ( pRightNeighbor != NULL )
	{
		if ( pRightNeighbor->pBaseNeighbor == pTriNode )
		{
			pRightNeighbor->pBaseNeighbor = pRightChild;
		}
		else if ( pRightNeighbor->pRightNeighbor == pTriNode )
		{
			pRightNeighbor->pRightNeighbor = pRightChild;
		}
		else if ( pRightNeighbor->pLeftNeighbor == pTriNode )
		{
			pRightNeighbor->pLeftNeighbor = pRightChild;
		}
		else
		{
			_asm int 3;// Illegal Right Neighbor!
		}
	}

	// 6. Link our Base Neighbor to the new children
	if ( pTriBase != NULL )
	{
		/*if ( pTriBase->pLeftChild != NULL )
		{
			pTriBase->pLeftChild->pRightNeighbor = pRightChild;
			pTriBase->pRightChild->pLeftNeighbor = pLeftChild;
			pLeftChild->pRightNeighbor = pTriBase->pRightChild;
			pRightChild->pLeftNeighbor = pTriBase->pLeftChild;
		}*/
		if ( pTriBase->nDepth == pTriNode->nDepth )
		{
			pTriBase->pRightNeighbor = pRightChild;
			pTriBase->pRightChild->pLeftNeighbor = pLeftChild;
			pLeftChild->pRightNeighbor = pTriBase->pRightChild;
			pRightChild->pLeftNeighbor = pTriBase;
		}
		else
		{	
			// Base Neighbor in a diamond with this split node was not split yet, so do that now.
			SplitTriTreeNode(pTriBase); 
		}
	}
	else
	{
		// If at the edge of a mesh, split as needed.
		pLeftChild->pRightNeighbor = NULL;
		pRightChild->pLeftNeighbor = NULL;
	}


	return pFirstNode;
}

effVOID EFF3DTerrainROAMImproveTileData::SplitTriTreeNode(TriTreeNode * pTriNode, effBOOL bLeftChildFirst, CODE firstCode, CODE followCode)
{
	effINT nCenterX = (pTriNode->nLeftX + pTriNode->nRightX) >> 1;
	effINT nCenterZ = (pTriNode->nLeftZ + pTriNode->nRightZ) >> 1;

	effINT nApexX = pTriNode->nApexX;
	effINT nApexZ = pTriNode->nApexZ;
	effINT nLeftX = pTriNode->nLeftX;
	effINT nLeftZ = pTriNode->nLeftZ;
	effINT nRightX = pTriNode->nRightX;
	effINT nRightZ = pTriNode->nRightZ;


	TriTreeNode * pRightChild = new TriTreeNode();
	pRightChild->pVariance = pTriNode->pVariance;
	pRightChild->Set(nCenterX, nCenterZ, nRightX, nRightZ, nApexX, nApexZ);
	pTriNode->Set(nCenterX, nCenterZ, nApexX, nApexZ, nLeftX, nLeftZ);

	pTriNode->nDepth++;
	pTriNode->varianceIndex *= 2;
	pTriNode->pRightChild = pRightChild;
	pRightChild->nDepth = pTriNode->nDepth;
	pRightChild->varianceIndex = pTriNode->varianceIndex + 1;

	if ( bLeftChildFirst )
	{
		TriTreeNode * pNext = pTriNode->pNext;
		pTriNode->pNext = pRightChild;
		pRightChild->pNext = pNext;
		pRightChild->pPre = pTriNode;

		if ( pNext != NULL )
		{
			pNext->pPre = pRightChild;
		}

		pTriNode->code = firstCode;
		pRightChild->code = followCode;
	}
	else
	{
		TriTreeNode * pPre = pTriNode->pPre;
		pRightChild->pPre = pPre;
		pRightChild->pNext = pTriNode;
		pTriNode->pPre = pRightChild;

		if ( pPre != NULL )
		{
			pPre->pNext = pRightChild;
		}

		pTriNode->code = followCode;
		pRightChild->code = firstCode;
	}
}



effVOID EFF3DTerrainROAMImproveTileData::CreateTriTreeChildNode(TriTreeNode * pTriNode)
{
	switch (pTriNode->code)
	{
	case AL:
		CreateTriTreeChildNode(pTriNode, effTRUE, CL, BL);
		break;
	case AR:
		CreateTriTreeChildNode(pTriNode, effFALSE, CR, BR);
		break;
	case CL:
		CreateTriTreeChildNode(pTriNode, effFALSE, AL, BR);
		break;
	case CR:
		CreateTriTreeChildNode(pTriNode, effTRUE, AR, BL);
		break;
	case BL:
		CreateTriTreeChildNode(pTriNode, effFALSE, CR, AL);
		break;
	case BR:
		CreateTriTreeChildNode(pTriNode, effTRUE, CL, AR);
		break;
	default:
		break;
	}
}

effVOID EFF3DTerrainROAMImproveTileData::CreateTriTreeChildNode(TriTreeNode * pTriNode, effBOOL bLeftChildFirst, CODE firstCode, CODE secondCode)
{
	effINT nCenterX = (pTriNode->nLeftX + pTriNode->nRightX) >> 1;
	effINT nCenterZ = (pTriNode->nLeftZ + pTriNode->nRightZ) >> 1;

	effINT nApexX = pTriNode->nApexX;
	effINT nApexZ = pTriNode->nApexZ;
	effINT nLeftX = pTriNode->nLeftX;
	effINT nLeftZ = pTriNode->nLeftZ;
	effINT nRightX = pTriNode->nRightX;
	effINT nRightZ = pTriNode->nRightZ;

	TriTreeNode * pLeftChild = pTriNode->pLeftChild;
	TriTreeNode * pRightChild = pTriNode->pRightChild;

	pLeftChild->Set(nCenterX, nCenterZ, nApexX, nApexZ, nLeftX, nLeftZ);
	pRightChild->Set(nCenterX, nCenterZ, nRightX, nRightZ, nApexX, nApexZ);


	pLeftChild->nDepth = pTriNode->nDepth + 1;
	pRightChild->nDepth = pTriNode->nDepth + 1;


	if ( bLeftChildFirst )
	{
		TriTreeNode * pNext = pTriNode->pNext;
		pTriNode->pNext = pLeftChild;
		pLeftChild->pNext = pRightChild;
		pRightChild->pNext = pNext;

		pLeftChild->pPre = pTriNode;
		pRightChild->pPre = pLeftChild;
		if ( pNext != NULL )
		{
			pNext->pPre = pRightChild;
		}

		pLeftChild->code = firstCode;
		pRightChild->code = secondCode;
	}
	else
	{
		TriTreeNode * pNext = pTriNode->pNext;
		pTriNode->pNext = pRightChild;
		pRightChild->pNext = pLeftChild;
		pLeftChild->pNext = pNext;

		pRightChild->pPre = pTriNode;
		pLeftChild->pPre = pRightChild;
		if ( pNext != NULL )
		{
			pNext->pPre = pLeftChild;
		}

		pLeftChild->code = secondCode;
		pRightChild->code = firstCode;
	}
}



EFF3DVertexBuffer * EFF3DTerrainROAMImproveTileData::GetVertexBuffer(effINT nLevel, EFF3DDevice * pDevice)
{

	if ( m_pVB == NULL )
	{
		pDevice->CreateVertexBuffer(m_nVerticesNum[nLevel] * sizeof(effFLOAT) * 3, 0, EFF3DFVF_XYZ, EFF3DPOOL_DEFAULT, &m_pVB);

		effUINT offset = 0;
		if ( nLevel > 0 )
		{
			offset = m_nVerticesNum[nLevel-1] * sizeof(effFLOAT) * 3;
		}

		effUINT size = m_nVerticesNum[nLevel] * sizeof(effFLOAT) * 3;
		if ( nLevel > 0 )
		{
			size -= m_nVerticesNum[nLevel-1] * sizeof(effFLOAT) * 3;
		}

		effHRESULT hr;
		effVOID * pData = NULL;
		if ( !FAILED(hr = m_pVB->LockBuffer(offset, size, &pData, 0)) )
		{
			memcpy(pData, &m_pVertices[offset], size);

			m_pVB->UnlockBuffer();
		}
	}




	return m_pVB;
}

effINT EFF3DTerrainROAMImproveTileData::GetVerticesNum(effINT nLevel)
{
	return m_nVerticesNum[nLevel];
}

effINT EFF3DTerrainROAMImproveTileData::GetVerticesStride()
{
	return sizeof(effFLOAT) * 3;
}

EFF3DIndexBuffer * EFF3DTerrainROAMImproveTileData::GetIndexBuffer(effINT nLevel, EFF3DDevice * pDevice)
{
	if ( m_pIB[nLevel] == NULL )
	{
		effINT nSize = sizeof(effUINT16) * m_nIndicesNum[nLevel];
		pDevice->CreateIndexBuffer(nSize, 0, EFF3DFMT_INDEX16, EFF3DPOOL_DEFAULT, &m_pIB[nLevel]);

		effHRESULT hr;
		effVOID * pData;
		if ( !FAILED(hr = m_pIB[nLevel]->Lock(0, 0, &pData, 0)) )
		{
			memcpy(pData, m_pIndices[nLevel], nSize);
			m_pIB[nLevel]->Unlock();
		}
	}

	return m_pIB[nLevel];
}

effINT EFF3DTerrainROAMImproveTileData::GetIndicesNum(effINT nLevel)
{
	return m_nIndicesNum[nLevel];
}

EFF3D_END