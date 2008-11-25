#pragma once

#include "math/vector3.hpp"
#include "math/boundbox.hpp"
#include "resmgr/bwresource.hpp"


class FourSpaceNode
{
public:
	FourSpaceNode();
	~FourSpaceNode();

	void create( Vector3 position, float mapWidth, float mapHeight, float mapLong, float blockWidht, float blockLong );
	void onCreateNode( const BoundingBox& boundBox  );
	void write( DataSectionPtr pData );
	void read( DataSectionPtr pData );

	BoundingBox boundBox;
	FourSpaceNode* children[4];
};


class FourTree
{
public:
	FourTree(void);
	~FourTree(void);

	void create( Vector3 position, float mapWidth, float mapHeight, float mapLong, float blockWidht, float blockLong );

	FourSpaceNode* root;
};
