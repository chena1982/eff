/******************************************************************************
	created:	2008-12-13   0:48
	file path:	d:\EFF\EFFEngine\Src\EFFMath\EFFMatrix4.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"

#include "EFFVector3.h"
#include "EFFMatrix4.h"

//#define new EFFNEW

EFFMatrix4::EFFMatrix4()
{
}

EFFMatrix4::~EFFMatrix4()
{
}

EFFMatrix4 * EFFMatrixPerspectiveFovLH(EFFMatrix4 *pOut,effFLOAT fovy,effFLOAT Aspect,effFLOAT zn,effFLOAT zf)
{
	/*
	xScale     0          0               0
	0        yScale       0               0
	0          0       zf/(zf-zn)         1
	0          0       -zn*zf/(zf-zn)     0
	where:
	yScale = cot(fovY/2)

	xScale = yScale / aspect ratio

	*/

	effFLOAT yScale = 1.f/tanf(fovy * 0.5f);
	effFLOAT xScale = yScale / Aspect;

	pOut->m11 = xScale; pOut->m12 = 0.f; pOut->m13 = 0.f; pOut->m14 = 0.f;
	pOut->m21 = 0.f; pOut->m22 = yScale; pOut->m23 = 0.f; pOut->m24 = 0.f;
	pOut->m31 = 0.f; pOut->m32 = 0.f;	 pOut->m33 = zf/(zf-zn); pOut->m34 = 1.f;
	pOut->m41 = 0.f; pOut->m42 = 0.f; pOut->m43 = zn*zf/(zn-zf); pOut->m44 = 0.f;

	return pOut;
}

EFFMatrix4 * EFFMatrixLookAtLH(EFFMatrix4 *pOut,const EFFVector3 *pEye,const EFFVector3 *pAt,const EFFVector3 *pUp)
{
	/*zaxis = normal(At - Eye)
		xaxis = normal(cross(Up, zaxis))
		yaxis = cross(zaxis, xaxis)

		xaxis.x           yaxis.x           zaxis.x          0
		xaxis.y           yaxis.y           zaxis.y          0
		xaxis.z           yaxis.z           zaxis.z          0
		-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye) 1.f*/

	EFFVector3 zaxis;
	EFFVec3Subtract(&zaxis,pAt,pEye);
	zaxis.Normalize();

	EFFVector3 xaxis;
	EFFVec3Cross(&xaxis,pUp,&zaxis);
	xaxis.Normalize();

	EFFVector3 yaxis;
	EFFVec3Cross(&yaxis,&zaxis,&xaxis);

	pOut->m11 = xaxis.x; pOut->m12 = yaxis.x; pOut->m13 = zaxis.x; pOut->m14 = 0.f;
	pOut->m21 = xaxis.y; pOut->m22 = yaxis.y; pOut->m23 = zaxis.y; pOut->m24 = 0.f;
	pOut->m31 = xaxis.z; pOut->m32 = yaxis.z;	pOut->m33 = zaxis.z; pOut->m34 = 0.f;

	pOut->m41 = -1.f * EFFVec3Dot(&xaxis,pEye);
	pOut->m42 = -1.f * EFFVec3Dot(&yaxis,pEye);
	pOut->m43 = -1.f * EFFVec3Dot(&zaxis,pEye);
	pOut->m44 = 1.f;


	return pOut;
}