/******************************************************************************
	created:	2013-02-02   21:27
	file path:	d:\EFF\trunk\EFFEngine\Include\EFF3D\EFF3DFrustum.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DFrustum_2013_02_02__
#define __EFF3DFrustum_2013_02_02__



EFF3D_BEGIN


class EFF3DFrustum
{
public:
	EFF3DFrustum();
	~EFF3DFrustum();


public:
	enum FrustumSide
	{
		RIGHT,
		LEFT,
		BOTTOM,
		TOP,
		BACK,
		FRONT
	};

	enum FrustunmPoeffINT
	{
		FRONT_LEFTBOTTOM,
		BACK_LEFTBOTTOM,
		FRONT_LEFTTOP,
		BACK_LEFTTOP,
		FRONT_RIGHTBOTTOM,
		BACK_RIGHTBOTTOM,
		FRONT_RIGHTTOP,
		BACK_RIGHTTOP
	};

public:
	effVOID CalculateFrustum(const EFFMatrix4 & matView, const EFFMatrix4 & matProj);

	effVOID CalculateFrustum(EFFVector3 cameraPos, EFFVector3 lookAt, EFFVector3 right, EFFVector3 up, effFLOAT aspect, effFLOAT fov, effFLOAT nearPlane, effFLOAT farPlane);
	
	effVOID CalculatePoint(EFFVector3 pointBuffer[8]);

	effBOOL PointInFrustum(effFLOAT x, effFLOAT y, effFLOAT z) const;

	effBOOL PointInFrustum(EFFVector3 & pos) const;

	effBOOL SphereInFrustum(effFLOAT x, effFLOAT y, effFLOAT z, effFLOAT radius) const;

	effBOOL SphereInFrustum(EFFVector3 & center, effFLOAT radius) const;

	effBOOL AABBInFrustum(const EFFAabb & aabb) const;

	effBOOL InAABB(const EFFAabb & aabb) const;
	
	effBOOL PolygonInFrustum(EFFPolygon3 & polygon) const;

	effBOOL ClipWithPolygon(EFFPolygon3 & polygon, EFFVector3 & cameraPos, EFFVector3 & lookAt, EFF3DFrustum * resultFrustum);

	inline EFFPlane3 GetPlane(effINT planeIndex) const
	{
		assert(planeIndex >= 0 && planeIndex < 6);
		return EFFPlane3(plane[planeIndex][0], plane[planeIndex][1], plane[planeIndex][2], plane[planeIndex][3]);
	}
private:
	// This holds the A B C and D values for each side of our frustum.
public:
	effFLOAT			plane[6][4];
	EFFVector3			corner[8];
};


EFF3D_END

#endif