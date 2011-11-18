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

	// ����BoundBox��Plane
	void CreatePlane();
public:
	BoundBox(void);
	BoundBox( const Vector3 & vMin,	const Vector3 & vMax );				// �ò������� bound box
	BoundBox( const Vector3 & vCenter, float fRadius );						// �ò������� bound box
	~BoundBox(void);

	void setBound( const Vector3 & vMin, const Vector3 & vMax );		// ����, ���Ҵ��� bound box
	void setBound( const Vector3 & vCenter,	float fRadius );				// ����, ���Ҵ��� bound box

	const Vector3&	getBoxVertices( int i ) const	{ return boxVertices_[i]; }
	const Vector3&  getMinCorner()			const	{ return getBoxVertices(0);	}
	const Vector3&  getMaxCorner()			const	{ return getBoxVertices(6);	}
	const Vector3&  getBoxCenter()			const	{ return center_;			}
	const float			getBoxOutsideRadius()	const	{ return fOutsideRadius_;	}
	const float			getBoxRadius()			const	{ return fRadius_;			}

	// ���� BoundBox �Ƿ�� BoundBox �ཻ ( BoundBox ��ȫ���������߲��ֱ�������Ϊ�ཻ )
	BOOL intersect(const BoundBox& oBoundBox) const;
	//���Ƿ���BOX���
	BOOL in( const Vector3& v );

	Vector3			min_,max_;		// ��С�������󶥵�, �γɶԽ�
	Vector3			boxVertices_[8];	// cube �İ˸�����
	D3DXPLANE 		boxPlane_[6];		// cube ��������

	Vector3			center_;			// ����������
	float			fOutsideRadius_;	// �����İ뾶
	float			fRadius_;
};
