#include "pch.hpp"
#include "BoundBox.h"

//----------------------------------------------------------------------------------------
// 构造
//----------------------------------------------------------------------------------------
BoundBox::BoundBox(void)
{
}

//----------------------------------------------------------------------------------------
// 析构
//----------------------------------------------------------------------------------------
BoundBox::~BoundBox(void)
{
}

//----------------------------------------------------------------------------------------
// 用参数创建 bound box
//----------------------------------------------------------------------------------------
BoundBox::BoundBox( const Vector3 & vMin, const Vector3 & vMax )
{
	setBound( vMin, vMax );
}

//----------------------------------------------------------------------------------------
// 用参数创建 bound box
//----------------------------------------------------------------------------------------
BoundBox::BoundBox( const Vector3 & vCenter, float fRadius )
{
	setBound( vCenter, fRadius );
}

//----------------------------------------------------------------------------------------
// 设置, 并且创建 bound box
//----------------------------------------------------------------------------------------
void BoundBox::setBound( const Vector3 & vMin, const Vector3 & vMax )
{
	min_ = vMin;
	max_ = vMax;
	center_.x = (min_.x + max_.x ) / 2.0f;
	center_.y = (min_.y + max_.y ) / 2.0f;
	center_.z = (min_.z + max_.z ) / 2.0f;
	fOutsideRadius_ = (float)sqrt( (vMax.x-vMin.x)*(vMax.x-vMin.x) + (vMax.y-vMin.y)*(vMax.y-vMin.y) + (vMax.z-vMin.z)*(vMax.z-vMin.z) )/2.0f;
	fRadius_ = fOutsideRadius_ / 1.73205f;

	// create 8 vertices
	boxVertices_[0]	= min_;
	boxVertices_[1].x = min_.x;		boxVertices_[1].y = max_.y;		boxVertices_[1].z = min_.z;
	boxVertices_[2].x = max_.x;		boxVertices_[2].y = max_.y;		boxVertices_[2].z = min_.z;
	boxVertices_[3].x = max_.x;		boxVertices_[3].y = min_.y;		boxVertices_[3].z = min_.z;
	boxVertices_[4].x = min_.x;		boxVertices_[4].y = min_.y;		boxVertices_[4].z = max_.z;
	boxVertices_[5].x = min_.x;		boxVertices_[5].y = max_.y;		boxVertices_[5].z = max_.z;
	boxVertices_[6]	= max_;
	boxVertices_[7].x = max_.x;		boxVertices_[7].y = min_.y;		boxVertices_[7].z = max_.z;

	CreatePlane();
}

//----------------------------------------------------------------------------------------
// 设置, 并且创建 bound box
//----------------------------------------------------------------------------------------
void BoundBox::setBound( const Vector3 & vCenter,	float fRadius )
{
	center_= vCenter;

	min_.x = vCenter.x - fRadius;
	min_.y = vCenter.y - fRadius;
	min_.z = vCenter.z - fRadius;

	max_.x = vCenter.x + fRadius;
	max_.y = vCenter.y + fRadius;
	max_.z = vCenter.z + fRadius;


	// create 8 vertices
	boxVertices_[0]	= min_;
	boxVertices_[1].x = min_.x;		boxVertices_[1].y = max_.y;		boxVertices_[1].z = min_.z;
	boxVertices_[2].x = max_.x;		boxVertices_[2].y = max_.y;		boxVertices_[2].z = min_.z;
	boxVertices_[3].x = max_.x;		boxVertices_[3].y = min_.y;		boxVertices_[3].z = min_.z;
	boxVertices_[4].x = min_.x;		boxVertices_[4].y = min_.y;		boxVertices_[4].z = max_.z;
	boxVertices_[5].x = min_.x;		boxVertices_[5].y = max_.y;		boxVertices_[5].z = max_.z;
	boxVertices_[6]	= max_;
	boxVertices_[7].x = max_.x;		boxVertices_[7].y = min_.y;		boxVertices_[7].z = max_.z;

	CreatePlane();

	// 求出外接球的半径和球的中心 from math. we know R = sqrt(3)*r
	fOutsideRadius_ = fRadius * 1.73205f;
	fRadius_ = fRadius;
}

// 创建BoundBox的Plane
void BoundBox::CreatePlane()
{
	// create 6 planes (各个 plane 以法向向内为正方向)
	D3DXPlaneFromPoints( &boxPlane_[BBS_FRONT],	&getBoxVertices(0), &getBoxVertices(2), &getBoxVertices(1) );
	D3DXPlaneFromPoints( &boxPlane_[BBS_BACK],		&getBoxVertices(4), &getBoxVertices(5), &getBoxVertices(6) );
	D3DXPlaneFromPoints( &boxPlane_[BBS_LEFT],		&getBoxVertices(0), &getBoxVertices(1), &getBoxVertices(5) );
	D3DXPlaneFromPoints( &boxPlane_[BBS_RIGHT],	&getBoxVertices(2), &getBoxVertices(3), &getBoxVertices(6) );
	D3DXPlaneFromPoints( &boxPlane_[BBS_TOP],		&getBoxVertices(1), &getBoxVertices(2), &getBoxVertices(6) );
	D3DXPlaneFromPoints( &boxPlane_[BBS_BOTTOM],	&getBoxVertices(0), &getBoxVertices(4), &getBoxVertices(3) );
}


// 测试 BoundBox 是否和 BoundBox 相交 ( BoundBox 完全被包含或者部分被包含都为相交 )
BOOL BoundBox::intersect(const BoundBox& oBoundBox) const
{
	for(int i=0; i<6; i++)
	{	
		float a = boxPlane_[i].a;
		float b = boxPlane_[i].b;
		float c = boxPlane_[i].c;
		float d = boxPlane_[i].d;

		if( a * oBoundBox.getBoxVertices(0).x + b * oBoundBox.getBoxVertices(0).y + c * oBoundBox.getBoxVertices(0).z + d > 0 )	continue;
		if( a * oBoundBox.getBoxVertices(1).x + b * oBoundBox.getBoxVertices(1).y + c * oBoundBox.getBoxVertices(1).z + d > 0 )	continue;
		if( a * oBoundBox.getBoxVertices(2).x + b * oBoundBox.getBoxVertices(2).y + c * oBoundBox.getBoxVertices(2).z + d > 0 )	continue;
		if( a * oBoundBox.getBoxVertices(3).x + b * oBoundBox.getBoxVertices(3).y + c * oBoundBox.getBoxVertices(3).z + d > 0 )	continue;
		if( a * oBoundBox.getBoxVertices(4).x + b * oBoundBox.getBoxVertices(4).y + c * oBoundBox.getBoxVertices(4).z + d > 0 )	continue;
		if( a * oBoundBox.getBoxVertices(5).x + b * oBoundBox.getBoxVertices(5).y + c * oBoundBox.getBoxVertices(5).z + d > 0 )	continue;
		if( a * oBoundBox.getBoxVertices(6).x + b * oBoundBox.getBoxVertices(6).y + c * oBoundBox.getBoxVertices(6).z + d > 0 )	continue;
		if( a * oBoundBox.getBoxVertices(7).x + b * oBoundBox.getBoxVertices(7).y + c * oBoundBox.getBoxVertices(7).z + d > 0 )	continue;
				
		return FALSE;		// 如果八个点都落在 boxPlane_[i] 负法向方向
	}
	return TRUE;
}

//点是否在BOX里边
BOOL BoundBox::in( const Vector3& v )
{
	if( boxVertices_[0].x <= v.x && 	boxVertices_[6].x >= v.x &&
		boxVertices_[0].y <= v.y && 	boxVertices_[6].y >= v.y &&
		boxVertices_[0].z <= v.z && 	boxVertices_[6].z >= v.z )
		return TRUE;
	return FALSE;
}