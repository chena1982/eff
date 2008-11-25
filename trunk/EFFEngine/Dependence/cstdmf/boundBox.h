#pragma once

#include "math/vector3.hpp"


class BoundBox
{
	enum eBoundBoxSide
	{
		BBS_FRONT,		// The FRONT side of the frustum, i.e. the front clippling plane
		BBS_BACK,		// The BACK	 side of the frustum	 i.e. the back clippling plane
		BBS_LEFT,		// The LEFT	 side of the frustum
		BBS_RIGHT,		// The RIGHT side of the frustum
		BBS_TOP,		// The TOP side of the frustum
		BBS_BOTTOM			// The BOTTOM side of the frustum
	}; 

	// 创建BoundBox的Plane
	void CreatePlane();
public:
	BoundBox(void);
	BoundBox( const Vector3 & vMin,	const Vector3 & vMax );				// 用参数创建 bound box
	BoundBox( const Vector3 & vCenter, float fRadius );						// 用参数创建 bound box
	~BoundBox(void);

	void setBound( const Vector3 & vMin, const Vector3 & vMax );		// 设置, 并且创建 bound box
	void setBound( const Vector3 & vCenter,	float fRadius );				// 设置, 并且创建 bound box

	const Vector3&	getBoxVertices( int i ) const	{ return boxVertices_[i]; }
	const Vector3&  getMinCorner()			const	{ return getBoxVertices(0);	}
	const Vector3&  getMaxCorner()			const	{ return getBoxVertices(6);	}
	const Vector3&  getBoxCenter()			const	{ return center_;			}
	const float			getBoxOutsideRadius()	const	{ return fOutsideRadius_;	}
	const float			getBoxRadius()			const	{ return fRadius_;			}

	// 测试 BoundBox 是否和 BoundBox 相交 ( BoundBox 完全被包含或者部分被包含都为相交 )
	BOOL intersect(const BoundBox& oBoundBox) const;
	//点是否在BOX里边
	BOOL in( const Vector3& v );

	Vector3			min_,max_;		// 最小顶点和最大顶点, 形成对角
	Vector3			boxVertices_[8];	// cube 的八个顶点
	D3DXPLANE 		boxPlane_[6];		// cube 的六个面

	Vector3			center_;			// 外接球的球心
	float			fOutsideRadius_;	// 外接球的半径
	float			fRadius_;
};
