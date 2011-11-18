#include "pch.hpp"
#include "Frustum.h"
#include "BoundBox.hpp"
#include "xp_math.hpp"

//-----------------------------------------------------------------------------------
//����
//-----------------------------------------------------------------------------------
Frustum::Frustum(void)
{
}

//-----------------------------------------------------------------------------------
//����
//-----------------------------------------------------------------------------------
Frustum::~Frustum(void)
{
}

// �����ӽ�׶
void Frustum::calculateFrustum(const Matrix *pView,const Matrix *pProjection)
{
	Matrix mat;
    D3DXMatrixMultiply( (D3DXMATRIX*)&mat, pView, pProjection);

	// ��͸�ӱ任�У�x-��y-�����������-1��1��z-�����������ǰ����Ϊ0�������Ϊ1��
	frustumVertices_[0] = Vector3(-1.0f, -1.0f,  0.0f); 
    frustumVertices_[1] = Vector3(-1.0f,  1.0f,  0.0f); 
    frustumVertices_[2] = Vector3( 1.0f,  1.0f,  0.0f); 
    frustumVertices_[3] = Vector3( 1.0f, -1.0f,  0.0f); 

    frustumVertices_[4] = Vector3(-1.0f, -1.0f,  1.0f); 
    frustumVertices_[5] = Vector3(-1.0f,  1.0f,  1.0f);
    frustumVertices_[6] = Vector3( 1.0f,  1.0f,  1.0f);
    frustumVertices_[7] = Vector3( 1.0f, -1.0f,  1.0f); 

	// ��Ļ����任���ӽ�����--> �ٱ䵽����������, ����һ�������
	D3DXMatrixInverse( &mat, NULL, &mat );
	//D3DXVECTOR4 Out;		
    for( int i = 0; i < 8; i++ )
	{
		//D3DXVec3Transform(&Out, &frustumVertices_[i], &mat);
		//frustumVertices_[i] = Vector3( Out.x, Out.y,Out.z);
        D3DXVec3TransformCoord( &frustumVertices_[i], &frustumVertices_[i], &mat );
	}

	// �����λ������ view �ռ��е� Z ��������ȥ, ** ���� plane �Է�������Ϊ������
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


// ���� BoundingBox �Ƿ�ȫ������ Frustum ��
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

		// ֻҪ��һ���㲻�� frustumPlane_[i] �����򣬾ͷ���false
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

// ���� BoundingBox �Ƿ�� Frustum �ཻ ( BoundingBox ��ȫ���������߲��ֱ�������Ϊ�ཻ )
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
				
		return FALSE;		// ����˸��㶼���� frustumPlane_[i] ��������
	}
	return TRUE;
}
