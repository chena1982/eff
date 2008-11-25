#pragma once
#include "math/vector3.hpp"
#include "math/matrix.hpp"

class BoundingBox;

// û�е���calculateFrustum�Ϳ�ʼʹ�� Frustum �ǷǷ���.
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
	
	void calculateFrustum(const Matrix *pView,const Matrix *pProjection);	// �������λ�ñ仯���ϵķ����仯��
	
	// ���ǵ����� plane ��������Ϊ������
	BOOL in			(const Vector3& v)							const;	// This takes a 3D point and returns TRUE if it's inside of the Frustum
	BOOL in			(const BoundingBox&	oBoundingBox )					const;	// ���� BoundingBox �Ƿ�ȫ������ Frustum ��
	BOOL intersect	(const BoundingBox&	oBoundingBox )					const;	// ���� BoundingBox �Ƿ�� Frustum �ཻ( ��ȫ���������߲��ֱ�������Ϊ�ཻ )
		
private:
	Vector3 frustumVertices_[8];		// view Frustum �İ˸�����
	D3DXPLANE   frustumPlane_[6];			// ���� plane �Է�������Ϊ������
};
