#pragma once
#include "math/vector3.hpp"
#include "math/matrix.hpp"

class BoundingBox;

// 没有调用calculateFrustum就开始使用 Frustum 是非法的.
class Frustum
{
	// We create an enum of the sides so we don't have to call each side 0 or 1.
	// This way it makes it more understandable and readable when dealing with Frustum sides.
	enum eFrustumSide
	{
		FTS_FRONT	= 0,		// The NEAR	side of the Frustum, i.e. the front clippling plane
		FTS_BACK	= 1,		// The FAR	side of the Frustum	 i.e. the back clippling plane
		FTS_LEFT	= 2,		// The LEFT	 side of the Frustum
		FTS_RIGHT	= 3,		// The RIGHT side of the Frustum
		FTS_TOP		= 4,		// The TOP side of the Frustum
		FTS_BOTTOM	= 5			// The BOTTOM side of the Frustum
	}; 

public:
	Frustum(void);
	~Frustum(void);
	
	void calculateFrustum(const Matrix *pView,const Matrix *pProjection);	// 随着相机位置变化不断的发生变化的
	
	// 考虑到六个 plane 都以向内为正方向
	BOOL in			(const Vector3& v)							const;	// This takes a 3D point and returns TRUE if it's inside of the Frustum
	BOOL in			(const BoundingBox&	oBoundingBox )					const;	// 测试 BoundingBox 是否全部落在 Frustum 里
	BOOL intersect	(const BoundingBox&	oBoundingBox )					const;	// 测试 BoundingBox 是否和 Frustum 相交( 完全被包含或者部分被包含都为相交 )
		
private:
	Vector3 frustumVertices_[8];		// view Frustum 的八个顶点
	D3DXPLANE   frustumPlane_[6];			// 各个 plane 以法向向内为正方向
};
