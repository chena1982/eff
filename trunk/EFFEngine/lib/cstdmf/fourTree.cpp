#include "pch.hpp"
#include "FourTree.h"

FourSpaceNode::FourSpaceNode()
{
	for( int i = 0; i < 4; ++i )
	{
		children[i] = 0;
	}
}

FourSpaceNode::~FourSpaceNode()
{
	for( int i = 0; i < 4; ++i )
	{
		SAFE_DELETE( children[i] );
	}
}

void FourSpaceNode::write( DataSectionPtr pData )
{	
}

void FourSpaceNode::read( DataSectionPtr pData )
{
}


static BoundingBox calcuBoundBox( Vector3 position, float blockWidht, float blockLong, float blockHeight )
{
	BoundingBox box;
	float height = blockHeight*0.5f;
	Vector3 min = position - Vector3( blockWidht, -height, blockLong );
	Vector3 max = position + Vector3( blockWidht, height, blockLong );
	box.setBounds( min, max );
	return box;
}

void FourSpaceNode::create( Vector3 position, float mapWidth, float mapHeight, float mapLong, float blockWidht, float blockLong )
{
	onCreateNode( calcuBoundBox( position, blockWidht, blockLong, mapHeight ) );

	float mWidth = mapWidth * 0.5f;
	float mLong = mapLong * 0.5f;

	if( mapWidth < mWidth * 2 && mapLong < mLong * 2 )
	{
		Vector3 center = position + Vector3( mWidth, 0, mLong );
		children[0] = new FourSpaceNode;
		children[0]->create( center, mWidth, mapHeight, mLong, blockWidht, blockLong );

		center = position + Vector3( -mWidth, 0, mLong );
		children[1] = new FourSpaceNode;
		children[1]->create( center, mWidth, mapHeight, mLong, blockWidht, blockLong );

		center = position + Vector3( mWidth, 0, -mLong );
		children[2] = new FourSpaceNode;
		children[2]->create( center, mWidth, mapHeight, mLong, blockWidht, blockLong );

		center = position + Vector3( -mWidth, 0, -mLong );
		children[3] = new FourSpaceNode;
		children[3]->create( center, mWidth, mapHeight, mLong, blockWidht, blockLong );
	}
}

// 创建子结点
void FourSpaceNode::onCreateNode( const BoundingBox& bBox )
{
	boundBox = bBox;
}

FourTree::FourTree(void)
{
	root = 0;
}

FourTree::~FourTree(void)
{
	SAFE_DELETE( root );
}

void FourTree::create( Vector3 position, float mapWidth, float mapHeight, float mapLong, float blockWidht, float blockLong )
{
	SAFE_DELETE( root );
	root = new FourSpaceNode;

	root->create( position, mapWidth, mapHeight, mapLong, blockWidht, blockLong );
}
