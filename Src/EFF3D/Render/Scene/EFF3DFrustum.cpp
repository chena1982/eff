/******************************************************************************
	created:	2013-02-02   21:29
	file path:	d:\EFF\trunk\EFFEngine\Include\EFF3D\EFF3DFrustum.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "../../EFF3DPCH.h"
#include "EFF3DFrustum.h"




EFF3D_BEGIN

#define FRUSTUM_EPSILON (1.0e-6f)



enum PlaneData
{
	A,
	B,
	C,
	D
};

EFF3DFrustum::EFF3DFrustum()
{
}


EFF3DFrustum::~EFF3DFrustum()
{
}




effVOID NormalizePlane(effFLOAT frustum[6][4], effINT side)
{
	// Here we calculate the magnitude of the normal to the plane (point A B C)
	// Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
	// To calculate magnitude you use the equation:  magnitude = sqrt( x^2 + y^2 + z^2)
	effFLOAT magnitude = (effFLOAT)sqrt(frustum[side][A] * frustum[side][A] + frustum[side][B] * frustum[side][B] + frustum[side][C] * frustum[side][C]);

	// Then we divide the plane's values by it's magnitude.
	// This makes it easier to work with.
	frustum[side][A] /= magnitude;
	frustum[side][B] /= magnitude;
	frustum[side][C] /= magnitude;
	frustum[side][D] /= magnitude; 
}


/*effBOOL EFF3DFrustum::ClipWithPortal(UC3DPortal * pPortal,EFFVector3 & cameraPos,EFFVector3 & vView,EFF3DFrustum * pResultFrustum)
{
	UC_ASSERT(pPortal != NULL && pPortal->m_portal.GetNumVertices() == 4);

	return ClipWithPolygon(pPortal->m_portal,cameraPos,vView,pResultFrustum);

}*/


effBOOL EFF3DFrustum::ClipWithPolygon(EFFPolygon3 & polygon, EFFVector3 & cameraPos, EFFVector3 & view, EFF3DFrustum * resultFrustum)
{
	assert(resultFrustum != NULL);

	if ( PolygonInFrustum(polygon) ) 
	{
		effINT portalVertexNum = polygon.GetVerticesCount();

		// Create the planes from portal
		effINT planeCount = portalVertexNum;
		EFFPlane3 * plane = new EFFPlane3[planeCount];

		for ( effINT i = 0; i < planeCount; i++ )
		{
			effINT vertexId = (planeCount + i - 1) % planeCount;

			// get the normal from edges
			EFFVector3 edge1 = polygon.vertices[i] - cameraPos;
			EFFVector3 edge2 = polygon.vertices[vertexId] - cameraPos;

			EFFVector3 normal;
			normal = edge1 % edge2;
			EFFVector3Normalize(&normal, &normal);

			if ( (normal | view) < 0.0f )
			{
				normal = -normal;
			}

			plane[i].normal = normal;
			//because cameraPos on every plane
			plane[i].d = -(cameraPos | normal);

			resultFrustum->plane[i][0] = plane[i].GetNormal().x;
			resultFrustum->plane[i][1] = plane[i].GetNormal().y;
			resultFrustum->plane[i][2] = plane[i].GetNormal().z;
			resultFrustum->plane[i][3] = plane[i].GetD();
		}

		resultFrustum->plane[BACK][0] = this->plane[BACK][0];
		resultFrustum->plane[BACK][1] = this->plane[BACK][1];
		resultFrustum->plane[BACK][2] = this->plane[BACK][2];
		resultFrustum->plane[BACK][3] = this->plane[BACK][3];

		resultFrustum->plane[FRONT][0] = this->plane[FRONT][0];
		resultFrustum->plane[FRONT][1] = this->plane[FRONT][1];
		resultFrustum->plane[FRONT][2] = this->plane[FRONT][2];
		resultFrustum->plane[FRONT][3] = this->plane[FRONT][3];

		return effTRUE;
	}
	else
	{
		return effFALSE;
	}
}

effVOID EFF3DFrustum::CalculateFrustum(EFFVector3 cameraPos, EFFVector3 lookAt, EFFVector3 right, EFFVector3 up,
									effFLOAT aspect, effFLOAT fov, effFLOAT nearPlane, effFLOAT farPlane)
{

	EFFVector3 view = lookAt - cameraPos;
	EFFVector3Normalize(&view, &view);
	EFFVector3Normalize(&up, &up);
	EFFVector3Normalize(&right, &right);

	
	effFLOAT tanHalfFov = tan(fov * 0.5f);

	EFFVector3 nearUpMiddlePos = cameraPos + view * nearPlane + up * nearPlane * tanHalfFov;

	corner[0] = nearUpMiddlePos - right * nearPlane * tanHalfFov * aspect;
	corner[1] = nearUpMiddlePos + right * nearPlane * tanHalfFov * aspect;

	EFFVector3 nearBottomMiddlePos = cameraPos + view * nearPlane - up * nearPlane * tanHalfFov;

	corner[2] = nearBottomMiddlePos - right * nearPlane * tanHalfFov * aspect;
	corner[3] = nearBottomMiddlePos + right * nearPlane * tanHalfFov * aspect;

	EFFVector3 farUpMiddlePos = cameraPos + view * farPlane + up * farPlane * tanHalfFov;

	corner[4] = farUpMiddlePos - right * farPlane * tanHalfFov * aspect;
	corner[5] = farUpMiddlePos + right * farPlane * tanHalfFov * aspect;

	EFFVector3 farBottomMiddlePos = cameraPos + view * farPlane - up * farPlane * tanHalfFov;

	corner[6] = farBottomMiddlePos - right * farPlane * tanHalfFov * aspect;
	corner[7] = farBottomMiddlePos + right * farPlane * tanHalfFov * aspect;



	EFFPlane3 rightPlane(corner[5], corner[1], corner[3]);
	plane[RIGHT][A] = rightPlane.normal.x;
	plane[RIGHT][B] = rightPlane.normal.y;
	plane[RIGHT][C] = rightPlane.normal.z;
	plane[RIGHT][D] = rightPlane.d;

	EFFPlane3 left(corner[4], corner[6], corner[2]);
	plane[LEFT][A] = left.normal.x;
	plane[LEFT][B] = left.normal.y;
	plane[LEFT][C] = left.normal.z;
	plane[LEFT][D] = left.d;

	EFFPlane3 bottom(corner[2], corner[6], corner[7]);
	plane[BOTTOM][A] = bottom.normal.x;
	plane[BOTTOM][B] = bottom.normal.y;
	plane[BOTTOM][C] = bottom.normal.z;
	plane[BOTTOM][D] = bottom.d;

	EFFPlane3 top(corner[0], corner[1], corner[5]);
	plane[TOP][A] = top.normal.x;
	plane[TOP][B] = top.normal.y;
	plane[TOP][C] = top.normal.z;
	plane[TOP][D] = top.d;

	EFFPlane3 back(corner[6], corner[4], corner[5]);
	plane[BACK][A] = back.normal.x;
	plane[BACK][B] = back.normal.y;
	plane[BACK][C] = back.normal.z;
	plane[BACK][D] = back.d;

	EFFPlane3 front(corner[3], corner[1], corner[0]);
	plane[FRONT][A] = front.normal.x;
	plane[FRONT][B] = front.normal.y;
	plane[FRONT][C] = front.normal.z;
	plane[FRONT][D] = front.d;


}

effVOID EFF3DFrustum::CalculateFrustum(const EFFMatrix4 & matView, const EFFMatrix4 & matProj)
{    
	EFFMatrix4 matClip;			// This will hold the clipping planes

	matClip = matView * matProj;


	// Now we actually want to get the sides of the   To do this we take
	// the clipping planes we received above and extract the sides from them.

	// This will extract the RIGHT side of the frustum
	plane[RIGHT][A] = matClip.m14 - matClip.m11;
	plane[RIGHT][B] = matClip.m24 - matClip.m21;
	plane[RIGHT][C] = matClip.m34 - matClip.m31;
	plane[RIGHT][D] = matClip.m44 - matClip.m41;

	// Now that we have a normal (A,B,C) and a distance (D) to the plane,
	// we want to normalize that normal and distance.

	// Normalize the RIGHT side
	NormalizePlane(plane, RIGHT);

	// This will extract the LEFT side of the frustum
	plane[LEFT][A] = matClip.m14 + matClip.m11;
	plane[LEFT][B] = matClip.m24 + matClip.m21;
	plane[LEFT][C] = matClip.m34 + matClip.m31;
	plane[LEFT][D] = matClip.m44 + matClip.m41;

	// Normalize the LEFT side
	NormalizePlane(plane, LEFT);

	// This will extract the BOTTOM side of the frustum
	plane[BOTTOM][A] = matClip.m14 + matClip.m12;
	plane[BOTTOM][B] = matClip.m24 + matClip.m22;
	plane[BOTTOM][C] = matClip.m34 + matClip.m32;
	plane[BOTTOM][D] = matClip.m44 + matClip.m42;

	// Normalize the BOTTOM side
	NormalizePlane(plane, BOTTOM);

	// This will extract the TOP side of the frustum
	plane[TOP][A] = matClip.m14 - matClip.m12;
	plane[TOP][B] = matClip.m24 - matClip.m22;
	plane[TOP][C] = matClip.m34 - matClip.m32;
	plane[TOP][D] = matClip.m44 - matClip.m42;

	// Normalize the TOP side
	NormalizePlane(plane, TOP);

	// This will extract the BACK side of the frustum
	plane[BACK][A] = matClip.m14 - matClip.m13;
	plane[BACK][B] = matClip.m24 - matClip.m23;
	plane[BACK][C] = matClip.m34 - matClip.m33;
	plane[BACK][D] = matClip.m44 - matClip.m43;

	// Normalize the BACK side
	NormalizePlane(plane, BACK);

	// This will extract the FRONT side of the frustum
	plane[FRONT][A] = matClip.m14 + matClip.m13;
	plane[FRONT][B] = matClip.m24 + matClip.m23;
	plane[FRONT][C] = matClip.m34 + matClip.m33;
	plane[FRONT][D] = matClip.m44 + matClip.m43;

	// Normalize the FRONT side
	NormalizePlane(plane, FRONT);
}

effBOOL EFF3DFrustum::PointInFrustum(effFLOAT x, effFLOAT y, effFLOAT z) const
{
	// Go through all the sides of the frustum
	for ( effINT i = 0; i< 6; i++ )
	{
		if ( plane[i][A] * x + plane[i][B] * y + plane[i][C] * z + plane[i][D] <= 0.0f )
		{
			return effFALSE;
		}
	}

	// The point was inside of the frustum (In front of ALL the sides of the frustum)
	return effTRUE;
}

effBOOL EFF3DFrustum::PointInFrustum(EFFVector3 & point) const
{
	return PointInFrustum(point.x, point.y, point.z);
}

effBOOL EFF3DFrustum::InAABB(const EFFAabb & aabb) const
{
	/*for ( effINT i = 0; i < 8; i++ )
	{
		if ( !aabb.ContainPoint(corner[i]) )
		{
			return effFALSE;
		}
	}*/
	return effTRUE;
}

effBOOL EFF3DFrustum::SphereInFrustum(effFLOAT x, effFLOAT y, effFLOAT z, effFLOAT radius) const
{
	// Go through all the sides of the frustum
	for ( effINT i = 0; i< 6; i++ )
	{
		if ( plane[i][A] * x + plane[i][B] * y + plane[i][C] * z + plane[i][D] <= -radius )
		{
			return effFALSE;
		}
	}

	// The sphere was inside of the frustum!
	return effTRUE;
}



effBOOL EFF3DFrustum::AABBInFrustum(const EFFAabb & aabb) const
{
	// Basically, what is going on is, that we are given the center of the aabb,
	// and half the length.  Think of it like a radius.  Then we checking each point
	// in the aabb and seeing if it is inside the   If a point is found in front
	// of a side, then we skip to the next side.  If we get to a plane that does NOT have
	// a point in front of it, then it will return false.

	// *Note* - This will sometimes say that a cube is inside the frustum when it isn't.
	// This happens when all the corners of the aabb are not behind any one plane.
	// This is rare and shouldn't effect the overall rendering speed.

	for ( effINT i = 0; i < 6; i++ )
	{
		EFFVector3 center, extent;
		center = aabb.GetCenter();
		extent = aabb.GetExtent();


		if ( plane[i][A] * (center.x - extent.x) + plane[i][B] * (center.y - extent.y) + plane[i][C] * (center.z - extent.z) + plane[i][D] > 0.0f )
		{
			continue;
		}

		if ( plane[i][A] * (center.x + extent.x) + plane[i][B] * (center.y - extent.y) + plane[i][C] * (center.z - extent.z) + plane[i][D] > 0.0f )
		{
			continue;
		}

		if ( plane[i][A] * (center.x - extent.x) + plane[i][B] * (center.y + extent.y) + plane[i][C] * (center.z - extent.z) + plane[i][D] > 0.0f )
		{
			continue;
		}

		if ( plane[i][A] * (center.x + extent.x) + plane[i][B] * (center.y + extent.y) + plane[i][C] * (center.z - extent.z) + plane[i][D] > 0.0f )
		{
			continue;
		}

		if ( plane[i][A] * (center.x - extent.x) + plane[i][B] * (center.y - extent.y) + plane[i][C] * (center.z + extent.z) + plane[i][D] > 0.0f )
		{
			continue;
		}

		if ( plane[i][A] * (center.x + extent.x) + plane[i][B] * (center.y - extent.y) + plane[i][C] * (center.z + extent.z) + plane[i][D] > 0.0f )
		{
			continue;
		}

		if ( plane[i][A] * (center.x - extent.x) + plane[i][B] * (center.y + extent.y) + plane[i][C] * (center.z + extent.z) + plane[i][D] > 0.0f )
		{
			continue;
		}

		if ( plane[i][A] * (center.x + extent.x) + plane[i][B] * (center.y + extent.y) + plane[i][C] * (center.z + extent.z) + plane[i][D] > 0.0f )
		{
			continue;
		}

		// If we get here, it isn't in the frustum
		return effFALSE;
	}

	return effTRUE;
}

/*effBOOL EFF3DFrustum::PortalInFrustum(UC3DPortal * pPortal) const
{
	UC_ASSERT( pPortal != NULL );
	return PolygonInFrustum(pPortal->m_portal);
}*/

effBOOL EFF3DFrustum::PolygonInFrustum(EFFPolygon3 & polygon) const
{
	for ( effINT i = 0; i < 6; i++ ) 
	{
		effBOOL haveOnePointInfront = effFALSE;

		for ( effINT j = 0; j < polygon.GetVerticesCount(); j++ )
		{
			EFFVector3 temp = *(polygon.GetVertex(j));
			if ( plane[i][A] * (temp.x) + plane[i][B] * (temp.y) + plane[i][C] * (temp.z) + plane[i][D] > EFFMATH_EPSILON )
			{
				haveOnePointInfront = effTRUE;
				break;
			}
		}

		if ( haveOnePointInfront ) 
		{
			continue;
		}

		return effFALSE;
	}

	return effTRUE;
}

//  this function tests if the projection of a bounding sphere along the light direction effINTersects
//  the view frustum 

/*effBOOL SweptSpherePlaneeffINTersect(effFLOAT & t0, effFLOAT & t1, EFFPlane3 & plane, EFFSphere & sphere, EFFVector3 & vSweepDir)
{
	effFLOAT b_dot_n = plane.Distance(sphere.m_vCenter);
	effFLOAT d_dot_n = plane.normal | vSweepDir;

	if (d_dot_n == 0.f)
	{
		if (b_dot_n <= sphere.m_fRadius)
		{
			//  effectively infinity
			t0 = 0.f;
			t1 = 1e32f;
			return ucTRUE;
		}
		else
			return ucFALSE;
	}
	else
	{
		effFLOAT tmp0 = ( sphere.m_fRadius - b_dot_n) / d_dot_n;
		effFLOAT tmp1 = (-sphere.m_fRadius - b_dot_n) / d_dot_n;
		t0 = MIN(tmp0, tmp1);
		t1 = MAX(tmp0, tmp1);
		return ucTRUE;
	}
}

effBOOL EFF3DFrustum::SweptSphereInFrustum(uc3dxSphere & sphere,EFFVector3 & vSweepDir) ucCONST
{

	//  algorithm -- get all 12 effINTersection poeffINTs of the swept sphere with the view frustum
	//  for all poeffINTs >0, displace sphere along the sweep direction.  if the displaced sphere
	//  is inside the frustum, return TRUE.  else, return FALSE
	effFLOAT displacements[12];
	uceffINT cnt = 0;
	effFLOAT a, b;
	effBOOL bInFrustum = ucFALSE;

	for (uceffINT i=0; i<6; i++)
	{
		EFFPlane3 plane = GetPlane(i);
		if ( SweptSpherePlaneeffINTersect(a, b, plane, sphere, vSweepDir) )
		{
			if (a>=0.f)
				displacements[cnt++] = a;
			if (b>=0.f)
				displacements[cnt++] = b;
		}
	}

	for (uceffINT i=0; i<cnt; i++)
	{
		uc3dxSphere displacedSphere(sphere);
		displacedSphere.m_vCenter += vSweepDir * displacements[i];
		displacedSphere.m_fRadius *= 1.1f;

		if ( SphereInFrustum(displacedSphere.m_vCenter.x,displacedSphere.m_vCenter.y,displacedSphere.m_vCenter.y,displacedSphere.m_fRadius) )
		{
			bInFrustum = ucTRUE;
			break;
		}
	}
	return bInFrustum;
}*/


// computes the point where three planes intersect
// returns whether or not the point exists.
effBOOL PlaneIntersect(EFFVector3 * intersectPos, EFFPlane3 * p0, EFFPlane3 * p1, EFFPlane3 * p2)
{
	EFFVector3 n0 = p0->normal;
	EFFVector3 n1 = p1->normal;
	EFFVector3 n2 = p2->normal;

	EFFVector3 n1_n2, n2_n0, n0_n1;  

	EFFVector3Cross(&n1_n2, &n1, &n2);
	EFFVector3Cross(&n2_n0, &n2, &n0);
	EFFVector3Cross(&n0_n1, &n0, &n1);

	effFLOAT cosTheta = EFFVector3Dot(&n0, &n1_n2);

	if ( ALMOST_ZERO(cosTheta) || IS_SPECIAL(cosTheta) )
	{
		return effFALSE;
	}

	effFLOAT secTheta = 1.f / cosTheta;

	n1_n2 = n1_n2 * p0->d;
	n2_n0 = n2_n0 * p1->d;
	n0_n1 = n0_n1 * p2->d;

	*intersectPos = -(n1_n2 + n2_n0 + n0_n1) * secTheta;
	return effTRUE;
}

effVOID EFF3DFrustum::CalculatePoint(EFFVector3 pointBuffer[8])
{
	for ( effINT i = 0; i < 8; i++ )
	{
		EFFPlane3 p0 = (i&1) ? GetPlane(4) : GetPlane(5);
		EFFPlane3 p1 = (i&2) ? GetPlane(3) : GetPlane(2);
		EFFPlane3 p2 = (i&4) ? GetPlane(0) : GetPlane(1);

		PlaneIntersect(&pointBuffer[i], &p0, &p1, &p2);
	}
}


EFF3D_END