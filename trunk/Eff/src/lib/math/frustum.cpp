#include "pch.hpp"
#include "Frustum.h"
#include "BoundBox.hpp"
#include "xp_math.hpp"

//-----------------------------------------------------------------------------------
//构析
//-----------------------------------------------------------------------------------
Frustum::Frustum(void)
{
}

//-----------------------------------------------------------------------------------
//析构
//-----------------------------------------------------------------------------------
Frustum::~Frustum(void)
{
}

// 创建视角锥
void Frustum::calculateFrustum(const Matrix *pView,const Matrix *pProjection)
{
	Matrix mat;
    D3DXMatrixMultiply( (D3DXMATRIX*)&mat, pView, pProjection);

	// 在透视变换中，x-与y-方向的限制是-1和1。z-方向的限制是前表面为0，后表面为1。
	frustumVertices_[0] = Vector3(-1.0f, -1.0f,  0.0f); 
    frustumVertices_[1] = Vector3(-1.0f,  1.0f,  0.0f); 
    frustumVertices_[2] = Vector3( 1.0f,  1.0f,  0.0f); 
    frustumVertices_[3] = Vector3( 1.0f, -1.0f,  0.0f); 

    frustumVertices_[4] = Vector3(-1.0f, -1.0f,  1.0f); 
    frustumVertices_[5] = Vector3(-1.0f,  1.0f,  1.0f);
    frustumVertices_[6] = Vector3( 1.0f,  1.0f,  1.0f);
    frustumVertices_[7] = Vector3( 1.0f, -1.0f,  1.0f); 

	// 屏幕坐标变换到视角坐标--> 再变到世界坐标中, 就是一个逆过程
	D3DXMatrixInverse( &mat, NULL, &mat );
	//D3DXVECTOR4 Out;		
    for( int i = 0; i < 8; i++ )
	{
		//D3DXVec3Transform(&Out, &frustumVertices_[i], &mat);
		//frustumVertices_[i] = Vector3( Out.x, Out.y,Out.z);
        D3DXVec3TransformCoord( &frustumVertices_[i], &frustumVertices_[i], &mat );
	}

	// 从相机位置沿着 view 空间中的 Z 轴正方向看去, ** 各个 plane 以法向向内为正方向
    D3DXPlaneFromPoints( &frustumPlane_[FTS_FRONT],	    &frustumVertices_[0],	&frustumVertices_[2], &frustumVertices_[1] );	// near
    D3DXPlaneFromPoints( &frustumPlane_[FTS_BACK],		&frustumVertices_[4],	&frustumVertices_[5], &frustumVertices_[6] );	// far
    D3DXPlaneFromPoints( &frustumPlane_[FTS_LEFT],		&frustumVertices_[0],	&frustumVertices_[1], &frustumVertices_[5] );	// left
    D3DXPlaneFromPoints( &frustumPlane_[FTS_RIGHT],	    &frustumVertices_[3],	&frustumVertices_[6], &frustumVertices_[2] );	// right
    D3DXPlaneFromPoints( &frustumPlane_[FTS_TOP],		&frustumVertices_[1],	&frustumVertices_[2], &frustumVertices_[6] );	// top
    D3DXPlaneFromPoints( &frustumPlane_[FTS_BOTTOM],	&frustumVertices_[0],	&frustumVertices_[4], &frustumVertices_[3] );	// bottom
}

// This takes a 3D point and returns TRUE if it's inside of the Frustum
// i.e. ax + by + cz + d < 0 then return false
BOOL Frustum::in(const Vector3& v) const
{
	for( int i=0; i<6; i++ )
	{
		if( frustumPlane_[i].a * v.x + frustumPlane_[i].b * v.y + frustumPlane_[i].c * v.z + frustumPlane_[i].d < 0 )
			return FALSE;
	}
	return TRUE;
}


// 测试 BoundingBox 是否全部落在 Frustum 里
BOOL Frustum::in(const BoundingBox& oBoundingBox) const
{
	Vector3 boxVertices[8];
	Vector3 min = oBoundingBox.minBounds();
	Vector3 max = oBoundingBox.maxBounds();
	boxVertices[0] = min;
	boxVertices[1].x = min.x;		boxVertices[1].y = max.y;		boxVertices[1].z = min.z;
	boxVertices[2].x = max.x;		boxVertices[2].y = max.y;		boxVertices[2].z = min.z;
	boxVertices[3].x = max.x;		boxVertices[3].y = min.y;		boxVertices[3].z = min.z;
	boxVertices[4].x = min.x;		boxVertices[4].y = min.y;		boxVertices[4].z = max.z;
	boxVertices[5].x = min.x;		boxVertices[5].y = max.y;		boxVertices[5].z = max.z;
	boxVertices[6] = max;
	boxVertices[7].x = max.x;		boxVertices[7].y = min.y;		boxVertices[7].z = max.z;

	for(int i=0; i<6; i++)
	{	
		float a = frustumPlane_[i].a;
		float b = frustumPlane_[i].b;
		float c = frustumPlane_[i].c;
		float d = frustumPlane_[i].d;

		// 只要有一个点不在 frustumPlane_[i] 法向方向，就返回false
		if( a * boxVertices[0].x + b * boxVertices[0].y + c * boxVertices[0].z + d < 0 )	return FALSE;
		if( a * boxVertices[1].x + b * boxVertices[1].y + c * boxVertices[1].z + d < 0 )	return FALSE;
		if( a * boxVertices[2].x + b * boxVertices[2].y + c * boxVertices[2].z + d < 0 )	return FALSE;
		if( a * boxVertices[3].x + b * boxVertices[3].y + c * boxVertices[3].z + d < 0 )	return FALSE;
		if( a * boxVertices[4].x + b * boxVertices[4].y + c * boxVertices[4].z + d < 0 )	return FALSE;
		if( a * boxVertices[5].x + b * boxVertices[5].y + c * boxVertices[5].z + d < 0 )	return FALSE;
		if( a * boxVertices[6].x + b * boxVertices[6].y + c * boxVertices[6].z + d < 0 )	return FALSE;
		if( a * boxVertices[7].x + b * boxVertices[7].y + c * boxVertices[7].z + d < 0 )	return FALSE;
	}
	return TRUE;
}

// 测试 BoundingBox 是否和 Frustum 相交 ( BoundingBox 完全被包含或者部分被包含都为相交 )
BOOL Frustum::intersect(const BoundingBox& oBoundingBox) const
{
	Vector3 boxVertices[8];
	Vector3 min = oBoundingBox.minBounds();
	Vector3 max = oBoundingBox.maxBounds();
	boxVertices[0] = min;
	boxVertices[1].x = min.x;		boxVertices[1].y = max.y;		boxVertices[1].z = min.z;
	boxVertices[2].x = max.x;		boxVertices[2].y = max.y;		boxVertices[2].z = min.z;
	boxVertices[3].x = max.x;		boxVertices[3].y = min.y;		boxVertices[3].z = min.z;
	boxVertices[4].x = min.x;		boxVertices[4].y = min.y;		boxVertices[4].z = max.z;
	boxVertices[5].x = min.x;		boxVertices[5].y = max.y;		boxVertices[5].z = max.z;
	boxVertices[6] = max;
	boxVertices[7].x = max.x;		boxVertices[7].y = min.y;		boxVertices[7].z = max.z;

	for(int i=0; i<6; i++)
	{	
		float a = frustumPlane_[i].a;
		float b = frustumPlane_[i].b;
		float c = frustumPlane_[i].c;
		float d = frustumPlane_[i].d;

		if( a * boxVertices[0].x + b * boxVertices[0].y + c * boxVertices[0].z + d > 0 )	continue;
		if( a * boxVertices[1].x + b * boxVertices[1].y + c * boxVertices[1].z + d > 0 )	continue;
		if( a * boxVertices[2].x + b * boxVertices[2].y + c * boxVertices[2].z + d > 0 )	continue;
		if( a * boxVertices[3].x + b * boxVertices[3].y + c * boxVertices[3].z + d > 0 )	continue;
		if( a * boxVertices[4].x + b * boxVertices[4].y + c * boxVertices[4].z + d > 0 )	continue;
		if( a * boxVertices[5].x + b * boxVertices[5].y + c * boxVertices[5].z + d > 0 )	continue;
		if( a * boxVertices[6].x + b * boxVertices[6].y + c * boxVertices[6].z + d > 0 )	continue;
		if( a * boxVertices[7].x + b * boxVertices[7].y + c * boxVertices[7].z + d > 0 )	continue;
				
		return FALSE;		// 如果八个点都落在 frustumPlane_[i] 负法向方向
	}
	return TRUE;
}
