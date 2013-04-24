/******************************************************************************
	created:	2013-02-02   23:35
	file path:	d:\EFF\EFFEngine\Src\EFFMath\EFFMathFunction.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/
#include "stdafx.h"

#include "EFFMathFunction.h"
#include "EFFVector3.h"
#include "EFFVector2.h"
#include "EFFAabb.h"
#include "EFFPlane3.h"
#include "EFFSegment3.h"
#include "EFFPolygon3.h"

EFFMATH_BEGIN


//下面两个函数做的事情是一样的，一个是变化过后的，第二个是没有优化的过程
EFFMATRIX4 * EFFMatrix4PerspectiveEx(EFFMATRIX4 * out, const effFLOAT nearPlane, const effFLOAT farPlane, const effFLOAT fovHoriz, const effFLOAT fovVert)
{
	effFLOAT h, w, Q;
	w = (effFLOAT)(cosf(fovHoriz * 0.5f));
	h = (effFLOAT)(cosf(fovVert * 0.5f));
	Q = farPlane / (farPlane - nearPlane);

	out->m11 = w;
	out->m12 = 0.f;
	out->m13 = 0.f;
	out->m14 = 0.f;

	out->m21 = 0.f;
	out->m22 = h;
	out->m23 = 0.f;
	out->m24 = 0.f;

	out->m31 = 0.f;
	out->m32 = 0.f;
	out->m33 = Q;
	out->m34 = 1.f;

	out->m41 = 0.f;
	out->m42 = 0.f;
	out->m43 = -Q * nearPlane;
	out->m44 = 0.f;

	return out;
}   




EFFMATRIX4* EFFMatrix4PerspectiveLH(EFFMATRIX4 * out, effFLOAT w, effFLOAT h, effFLOAT zn, effFLOAT zf)
{
	/*
	2*zn/w  0       0              0
	0       2*zn/h  0              0
	0       0       zf/(zf-zn)     1
	0       0       zn*zf/(zn-zf)  0
	*/

	
	out->m11 = 2.0f * zn / w;
	out->m12 = 0.f;
	out->m13 = 0.f;
	out->m14 = 0.f;

	out->m21 = 0.f;
	out->m22 = 2.0f * zn / h;
	out->m23 = 0.f;
	out->m24 = 0.f;

	out->m31 = 0.f;
	out->m32 = 0.f;
	out->m33 = zf / (zf - zn);
	out->m34 = 1.f;

	out->m41 = 0.f;
	out->m42 = 0.f;
	out->m43 = zn * zf / (zn - zf);
	out->m44 = 0.f;

	return out;
}

EFFMATRIX4 * EFFMatrix4PerspectiveFovLH(EFFMATRIX4 * out, effFLOAT fovy, effFLOAT aspect, effFLOAT zn, effFLOAT zf)
{
	/*xScale     0          0               0
		0        yScale       0               0
		0          0       zf/(zf-zn)         1
		0          0       -zn*zf/(zf-zn)     0
	where:
	yScale = cot(fovy/2)
	xScale = yScale / aspect ratio*/

	effFLOAT yScale = 1.0f / tanf(fovy / 2.0f);
	effFLOAT xScale = yScale / aspect;

	out->m11 = xScale;
	out->m12 = 0.f;
	out->m13 = 0.f;
	out->m14 = 0.f;

	out->m21 = 0.f;
	out->m22 = yScale;
	out->m23 = 0.f;
	out->m24 = 0.f;

	out->m31 = 0.f;
	out->m32 = 0.f;
	out->m33 = zf / (zf - zn);
	out->m34 = 1.f;

	out->m41 = 0.f;
	out->m42 = 0.f;
	out->m43 = zn * zf / (zn - zf);
	out->m44 = 0.f;

	return out;
}

//平行投影
EFFMATRIX4 * EFFMatrix4OrthoLH(EFFMATRIX4 * out, effFLOAT w, effFLOAT h, effFLOAT zn, effFLOAT zf)
{
	/*
	2/w  0    0           0
	0    2/h  0           0
	0    0    1/(zf-zn)   0
	0    0    zn/(zn-zf)  1
	*/
	out->m11 = 2.f / w;
	out->m12 = 0.f;
	out->m13 = 0.f;
	out->m14 = 0.f;

	out->m21 = 0.f;
	out->m22 = 2.f / h;
	out->m23 = 0.f;
	out->m24 = 0.f;

	out->m31 = 0.f;
	out->m32 = 0.f;
	out->m33 = 1.f / (zf - zn);
	out->m34 = 0.f;

	out->m41 = 0.f;
	out->m42 = 0.f;
	out->m43 = zn / (zn - zf);
	out->m44 = 1.f;

	return out;
}

EFFMATRIX4 * EFFMatrix4OrthoOffCenterLH(EFFMATRIX4 *out, effFLOAT l, effFLOAT r, effFLOAT b, effFLOAT t, effFLOAT zn, effFLOAT zf)
{
	/*2/(r-l)      0            0           0
	0            2/(t-b)      0           0
	0            0            1/(zf-zn)   0
	(l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  1*/

	out->m11 = 2.0f / (r -l);
	out->m12 = 0.f;
	out->m13 = 0.f;
	out->m14 = 0.f;

	out->m21 = 0.f;
	out->m22 = 2.0f / (t - b);
	out->m23 = 0.f;
	out->m24 = 0.f;

	out->m31 = 0.f;
	out->m32 = 0.f;
	out->m33 = 1.0f / (zf - zn);
	out->m34 = 0.f;

	out->m41 = (l + r) / (l - r);
	out->m42 = (t + b) / (b - t);
	out->m43 = zn / (zn - zf);
	out->m44 = 1.0f;

	return out;
}


effBOOL EFFMatrixIsPositiveDefinite(const effFLOAT * m, effINT n)
{
	effFLOAT * copy = NULL;
	assert((n == 3) || (n == 4));
	effINT size = n * n;
	copy = new effFLOAT[size];
	memcpy(copy, m, size * sizeof(effFLOAT));
	effBOOL result = EFFMatrixCanCholeskyDecomposition(copy, n);	
	delete[] copy;
	return result;
}

effBOOL EFFMatrixCanCholeskyDecomposition(effFLOAT * m, effINT n)
{
	effFLOAT sum, *a, *b, *aa, *bb, *cc, *recip;

	recip = new effFLOAT[n];
	aa = m;
	for ( effINT i = 0; i < n; i++ )
	{
		bb = m;
		cc = m + i * n;
		for ( effINT j = 0; j < i; j++ )
		{
			sum = *cc;
			a = aa;
			b = bb;
			for ( effINT k = j; k; k-- )
			{
				sum -= (*(a++)) * (*(b++));
			}
			*cc = sum * recip[j];
			bb += n;
			cc++;
		}
		sum = *cc;
		a = aa;

		for ( effINT k = i; k; k--, a++ )
		{
			sum -= (*a)*(*a);
		}

		if ( sum <= 0.0f )
		{
			delete[] recip;
			return effFALSE;
		}

		*cc = sqrtf(sum);
		recip[i] = 1.0f / (*cc);
		aa += n;
	}

	delete[] recip;
	return effTRUE;
}


effVOID EFFMatrixSolveCholesky(const effFLOAT * l, effFLOAT * b, effINT n)
{
	effFLOAT sum, *y;
	y = new effFLOAT[n];

	for ( effINT i = 0; i < n; i++ )
	{
		sum = 0;
		for ( effINT k = 0; k < i; k++ )
		{
			sum += l[i*n + k] * y[k];
		}

		y[i] = (b[i] - sum) / l[i*n + i];
	}

	for ( effINT i = n - 1; i >= 0; i-- )
	{
		sum = 0;
		for ( effINT k = i + 1; k < n; k++ )
		{
			sum += l[k*n + i] * b[k];
		}

		b[i] = (y[i] - sum) / l[i*n + i];
	}

	delete[] y;
}


effFLOAT EFFPointLineSquareDistance(const EFFVECTOR3 * p, const EFFVECTOR3 * v1, const EFFVECTOR3 * v2)
{
	EFFVector3 W;
	EFFVector3Sub(&W, v1, p);
	EFFVector3 L;
	EFFVector3Sub(&L, v2, v1);

	effFLOAT ratio = EFFVector3Dot(&W, &L) / EFFVector3Dot(&L, &L);
	EFFVector3 vtL;
	EFFVector3Mul(&vtL, ratio, &L);

	EFFVector3 p2l;
	EFFVector3Sub(&p2l, &W, &vtL);

	/*EFFVector3 p2l = W - L * (W*L)/(L*L);//(W*L)/(L*L)
	画出矢量图并且代入计算可知道，它的意义是垂足到lp的距离与l2-l1距离的比
	L*这个比例，就是向量，经过向量减法，可以得到垂线的向量*/

	return EFFVector3Length(&p2l);
}


effFLOAT EFFPointPlaneDistance(const EFFVECTOR3 & p, EFFPLANE & plane)
{
	//plane.Normalize();
	//return EFFVector3Dot(&p, &plane.GetNormal()) + plane.m_fD;

	return 0.0f;
}

/**
   计算3个点组成的面积
   函数返回的面积是三角形面积的2倍的平方
*/
effFLOAT EFFArea3D(const EFFVECTOR3 * a, const EFFVECTOR3 * b, const EFFVECTOR3 * c)
{
	//此处是拉格朗日恒等式的应用？不明白
	EFFVector3 v1;
	EFFVector3Sub(&v1, b, a);
	EFFVector3 v2;
	EFFVector3Sub(&v2, c, a);

	return ( (v1.y * v2.z + v1.z * v2.x + v1.x * v2.y) - (v1.y * v2.x + v1.x * v2.z + v1.z * v2.y) );
	
}

effFLOAT EFFVector3CalAngleXZ(const EFFVECTOR3 * v)
{
	EFFVector2 projection;
	projection.x = v->x;
	projection.y = v->z;

	EFFVector2Normalize(&projection, &projection);
	if ( v->z > 0.0f )
	{
		return acosf(projection.x);
	}
	else
	{
		return TWOPI - acosf(projection.x);
	}
}

EFFMATH_API effFLOAT EFFVector3CalAngleYZ(const EFFVECTOR3 * v)
{
	EFFVector2 vProjection;
	vProjection.x = v->y;
	vProjection.y = -v->z;
	EFFVector2Normalize(&vProjection, &vProjection);

	if ( v->z < 0.0f )
	{
		return acosf(vProjection.x);
	}
	else
	{
		return TWOPI - acosf(vProjection.x);
	}
}

EFFMATH_API effFLOAT EFFVector3CalAngleXY(const EFFVector3 * v)
{
	EFFVector2 vProjection;
	vProjection.x = v->y;
	vProjection.y = -v->x;
	EFFVector2Normalize(&vProjection, &vProjection);

	if ( v->x < 0.0f )
	{
		return acosf(vProjection.x);
	}
	else
	{
		return TWOPI - acosf(vProjection.x);
	}
}

EFFMATH_API effFLOAT EFFVector3Area(const EFFVector3 * a, const EFFVector3 * b, const EFFVector3 * c)
{
	EFFVector3 v1;
	EFFVector3Sub(&v1, b, a);

	EFFVector3 v2;
	EFFVector3Sub(&v2, c, a);

	EFFVector3 v = v1 % v2;
	return v.Length() * 0.5f;
	
}


effBOOL EFFRayIntersectPlane(effFLOAT * t, EFFVector3 * point, const EFFVector3 * pos, const EFFVector3 * dir, 
							const EFFVector3 * a, const EFFVector3 * b, const EFFVector3 * c)
{
	EFFVector3 vDir;
	EFFVector3Normalize(&vDir, dir);

	EFFVector3 normal;

	EFFVector3 AB = (*a) - (*b);
	EFFVector3 AC = (*a) - (*c);
	EFFVector3Cross(&normal, &AB, &AC);



	effFLOAT fromT, fromA;

	fromT = EFFVector3Dot(dir, &normal);
	if ( fromT < EFFMATH_EPSILON )
	{
		//反面
		return effFALSE;
	}

	EFFVector3 vPoint;
	EFFVector3 f = *a - *pos;
	fromA = EFFVector3Dot(&f, &normal);

	*t = fromA / fromT;

	(*point) = (*pos) + (*dir) * (*t);

	if ( normal.x < -EFFMATH_EPSILON || normal.x > EFFMATH_EPSILON )
	{
		effFLOAT kAB  = (a->y - point->y) * (b->z - a->z) - (a->z - point->z) * (b->y - a->y);
		effFLOAT kBC  = (b->y - point->y) * (c->z - b->z) - (b->z - point->z) * (c->y - b->y);
		effFLOAT kCA  = (c->y - point->y) * (a->z - c->z) - (c->z - point->z) * (a->y - c->y);

		if ( (kAB > 0.0f && kBC > 0.0f && kCA > 0.0f) )
		{
			return effTRUE;
		}
		else 
		{
			return effFALSE;
		}
	}
	else if ( normal.y < -EFFMATH_EPSILON || normal.y > EFFMATH_EPSILON )
	{
		effFLOAT kAB  = (a->x - point->x) * (b->z - a->z) - (a->z - point->z) * (b->x - a->x);
		effFLOAT kBC  = (b->x - point->x) * (c->z - b->z) - (b->z - point->z) * (c->x - b->x);
		effFLOAT kCA  = (c->x - point->x) * (a->z - c->z) - (c->z - point->z) * (a->x - c->x);

		if ( (kAB > 0.0f && kBC > 0.0f && kCA > 0.0f) )
		{
			return effTRUE;
		}
		else 
		{
			return effFALSE;
		}
	}
	else if ( normal.z < -EFFMATH_EPSILON || normal.z > EFFMATH_EPSILON )
	{
		effFLOAT kAB  = (a->x - point->x) * (b->y - a->y) - (a->y - point->y) * (b->x - a->x);
		effFLOAT kBC  = (b->x - point->x) * (c->y - b->y) - (b->y - point->y) * (c->x - b->x);
		effFLOAT kCA  = (c->x - point->x) * (a->y - c->y) - (c->y - point->y) * (a->x - c->x);

		if ( (kAB > 0.0f && kBC  >0.0f && kCA > 0.0f) )
		{
			return effTRUE;
		}
		else 
		{
			return effFALSE;
		}
	}

	return effFALSE;
}


effBOOL EFFLineIntersectPlane(effFLOAT * t, EFFVECTOR3 * point, const EFFVECTOR3 * from, const EFFVECTOR3 * length,
							const EFFVECTOR3 * a, const EFFVECTOR3 * b, const EFFVECTOR3 * c)
{
	EFFVector3 normal;

	EFFVector3 AB, BC;
	AB = *((const EFFVector3 *)b) - *((const EFFVector3 *)a);
	BC = *((const EFFVector3 *)c) - *((const EFFVector3 *)b);
	EFFVector3Cross(&normal, &AB, &BC);

	EFFVector3 vPoint;
	
	effFLOAT fromT, fromA;
	
	fromT = EFFVector3Dot(length, &normal);
	
	if ( fromT < EFFMATH_EPSILON )
	{
		//反面
		return effFALSE;
	}
	
	EFFVector3 vFromA;
	vFromA = *((const EFFVector3 *)a) - *((const EFFVector3 *)from);
	fromA = EFFVector3Dot(&vFromA, &normal);
	*t = fromA / fromT;
	
	if ( *t < 0.f || *t > 1.f )
	{
		return effFALSE;
	}
	
	*point = *((const EFFVector3 *)from) + (*(const EFFVector3 *)length) * (*t);
	
	return effTRUE;
}

effUINT EFFLineIntersectPlane(EFFVector3 * point, const EFFVector3 * from, const EFFVector3 * length,
							const EFFVector3 * a, const EFFVector3 * b, const EFFVector3 * c)
{
	EFFVector3 normal;

	EFFVector3 AB, BC;
	AB = *b - *a;
	BC = *c - *b;
	EFFVector3Cross(&normal, &AB, &BC);

	EFFVector3 vPoint;

	effFLOAT fromT, fromA;

	fromT = EFFVector3Dot(length, &normal);

	EFFVector3 vFromA;
	vFromA = *a - *from;
	fromA = EFFVector3Dot(&vFromA, &normal);

	effFLOAT ft = fromA / fromT;
	
	if ( ft < 0.0f || ft > 1.0f )
	{
		return effFALSE;
	}

	*point = *from + (*length) * ft;

	return effTRUE;
}

effBOOL EFFLineIntersectTriangle(effFLOAT * t, EFFVector3 * point, const EFFVector3 * from, const EFFVector3 * length,
								const EFFVector3 * a, const EFFVector3 * b, const EFFVector3 * c, const EFFAabb * lineBox,
								const EFFAabb * triBox, const EFFVector3 * normal, effFLOAT precision)
{
	assert(from && length);
	assert(a && b && c);
	assert(t && point);
	
	EFFAabb TriBox;
	TriBox.SetEmpty();
	if ( triBox != NULL )
	{
		TriBox.Extend(*a);
		TriBox.Extend(*b);
		TriBox.Extend(*c);
		triBox = &TriBox;
	}

	EFFAabb LineBox;
	LineBox.SetEmpty();
	if ( !lineBox != NULL )
	{
		EFFVector3 vTo = *from + *length;

		LineBox.Extend(*from);
		LineBox.Extend(vTo);
		lineBox = &LineBox;
	}

	// Do fast reject.
	if ( lineBox->GetMin().x > triBox->GetMax().x ||
		 lineBox->GetMin().y > triBox->GetMax().y ||
		 lineBox->GetMin().z > triBox->GetMax().z ||
		 lineBox->GetMax().x < triBox->GetMin().x ||
		 lineBox->GetMax().y < triBox->GetMin().y ||
		 lineBox->GetMax().z < triBox->GetMin().z )
	{
		return effFALSE;
	}

	// Generate normal if not inputed.
	
	EFFVector3 vAB, vBC;
	EFFVector3Sub(&vAB , b , a);
	EFFVector3Sub(&vBC , c , b);

	EFFVector3 vNormal;
	if ( !normal )
	{
		EFFVector3Cross(&vNormal, &vAB, &vBC);
		normal = (EFFVector3 *)&vNormal;
	}

	effFLOAT fFromT, fFromA;

	fFromT = EFFVector3Dot(length, normal);
	if ( fFromT > 0.f )
	{
		//反面
		return effFALSE;
	}

	EFFVector3 vFromA;
	EFFVector3Sub(&vFromA, a, from);
	fFromA = EFFVector3Dot(&vFromA, normal);
	*t = fFromA / fFromT;
	
	if ( *t < 0.f || *t > 1.0f )
	{
		// out of range.
		return effFALSE;
	}

	*point = *from + (*length) * (*t);

	enum {PROJECT_X, PROJECT_Y, PROJECT_Z} enProjDir;
	
	effFLOAT sx, sy, sz;
	sx = vAB.y * vBC.z - vAB.z * vBC.y;
	sy = vAB.x * vBC.z - vAB.z * vBC.x;
	sz = vAB.x * vBC.y - vAB.y * vBC.x;
	sx = sx < 0.f ? -sx : sx;
	sy = sy < 0.f ? -sy : sy;
	sz = sz < 0.f ? -sz : sz;
	
	if ( sx >= sy && sx >= sz ) 
	{
		enProjDir = PROJECT_X;
	}
	else if ( sy >= sx && sy >= sz ) 
	{
		enProjDir = PROJECT_Y;
	}
	else 
	{
		enProjDir = PROJECT_Z;
	}

	effFLOAT x0, y0, x1, y1, x2, y2;
	switch(enProjDir)
	{
	case PROJECT_Z:
		x0 = a->x - point->x; y0 = a->y - point->y;
		x1 = b->x - point->x; y1 = b->y - point->y;
		x2 = c->x - point->x; y2 = c->y - point->y;
		break;
	case PROJECT_X:
		x0 = a->z - point->z; y0 = a->y - point->y;
		x1 = b->z - point->z; y1 = b->y - point->y;
		x2 = c->z - point->z; y2 = c->y - point->y;
		break;
	default: // case PROJECT_Y:
		y0 = a->x - point->x; x0 = a->z - point->z;
		y1 = b->x - point->x; x1 = b->z - point->z;
		y2 = c->x - point->x; x2 = c->z - point->z;
	}
	
	effFLOAT t1, t2, t3;
	t1 = x0 * y1 - x1 * y0;
	t2 = x2 * y0 - x0 * y2;
	t3 = x1 * y2 - x2 * y1;

	return (t1 >= -precision && t2 >= -precision && t3 >= -precision)
		|| (t1 <= precision && t2 <= precision && t3 <= precision);
}

effBOOL EFFLineIntersectAabb(effFLOAT * nearDistance, EFFVector3 * nearPos, effFLOAT * farDistance, EFFVector3 * farPos,
							const EFFVector3 * from, const EFFVector3 * dir, const EFFAabb * box)
{
	EFFVector3 vDir;
	EFFVector3Normalize(&vDir, dir);
	
	const EFFVector3 & vMin = box->GetMin();
	const EFFVector3 & vMax = box->GetMax();
	
	effINT nIntersect = 0;
	struct InSectPos
	{
		EFFVector3 vPos;
		effFLOAT f;
	};

	InSectPos vRet[2];

	//Proj_x
	EFFVector3 vP1,vP2;
	effFLOAT fT1 = (vMin.x - from->x)/vDir.x;
	effFLOAT fT2 = (vMax.x - from->x)/vDir.x;
	vP1.x = vMin.x;
	vP1.y = from->y + vDir.y*fT1;
	vP1.z = from->z + vDir.z*fT1;

	vP2.x = vMax.x;
	vP2.y = from->y + vDir.y*fT2;
	vP2.z = from->z + vDir.z*fT2;

	if ( (vP1.y > vMax.y && vP2.y > vMax.y) || (vP1.y < vMin.y && vP2.y < vMin.y) ||
		 (vP1.z > vMax.z && vP2.z > vMax.z) || (vP1.z < vMin.z && vP2.z < vMin.z) )
	{
		return effFALSE;
	}
	else
	{
		if ( vP1.y < vMax.y && vP1.y > vMin.y && vP1.z < vMax.z && vP1.z > vMin.z )
		{
			vRet[nIntersect].vPos = vP1;
			vRet[nIntersect].f = fT1;
			nIntersect++;
		}

		if ( vP2.y < vMax.y && vP2.y > vMin.y && vP2.z < vMax.z && vP2.z > vMin.z )
		{
			vRet[nIntersect].vPos = vP2;
			vRet[nIntersect].f = fT2;
			nIntersect++;
		}

		if ( nIntersect == 2 )
		{
			if( vRet[0].f > vRet[1].f )
			{
				*nearPos = vRet[1].vPos;
				*nearDistance = vRet[1].f;
				*farPos = vRet[0].vPos;
				*farDistance = vRet[0].f;
			}
			else
			{
				*nearPos = vRet[0].vPos;
				*nearDistance = vRet[0].f;
				*farPos = vRet[1].vPos;
				*farDistance = vRet[1].f;
			}
			return effTRUE;
		}
	}

	//Proj_y
	fT1 = (vMin.y - from->y) / vDir.y;
	fT2 = (vMax.y - from->y) / vDir.y;
	vP1.x = from->x + vDir.x * fT1;
	vP1.y = vMin.y;
	vP1.z = from->z + vDir.z * fT1;

	vP2.x = from->x + vDir.x * fT2;
	vP2.y = vMax.y;
	vP2.z = from->z + vDir.z * fT2;

	if ( (vP1.x > vMax.x && vP2.x > vMax.x) || (vP1.x < vMin.x && vP2.x < vMin.x) ||
		 (vP1.z > vMax.z && vP2.z > vMax.z) || (vP1.z < vMin.z && vP2.z < vMin.z) )
	{
		return effFALSE;
	}
	else
	{
		if ( vP1.x < vMax.x && vP1.x > vMin.x && vP1.z < vMax.z && vP1.z > vMin.z )
		{
			vRet[nIntersect].vPos = vP1;
			vRet[nIntersect].f = fT1;
			nIntersect++;
		}

		if ( nIntersect == 2 )
		{
			if( vRet[0].f > vRet[1].f )
			{
				*nearPos = vRet[1].vPos;
				*nearDistance = vRet[1].f;
				*farPos = vRet[0].vPos;
				*farDistance = vRet[0].f;
			}
			else
			{
				*nearPos = vRet[0].vPos;
				*nearDistance = vRet[0].f;
				*farPos = vRet[1].vPos;
				*farDistance = vRet[1].f;
			}

			return effTRUE;
		}


		if ( vP2.x < vMax.x && vP2.x > vMin.x && vP2.z < vMax.z && vP2.z > vMin.z )
		{
			vRet[nIntersect].vPos = vP2;
			vRet[nIntersect].f = fT2;
			nIntersect++;
		}

		if ( nIntersect == 2 )
		{
			if( vRet[0].f > vRet[1].f )
			{
				*nearPos = vRet[1].vPos;
				*nearDistance = vRet[1].f;
				*farPos = vRet[0].vPos;
				*farDistance = vRet[0].f;
			}
			else
			{
				*nearPos = vRet[0].vPos;
				*nearDistance = vRet[0].f;
				*farPos = vRet[1].vPos;
				*farDistance = vRet[1].f;
			}
			return effTRUE;
		}
	}

	//Proj_z
	fT1 = (vMin.z - from->z) / vDir.z;
	fT2 = (vMax.z - from->z) / vDir.z;
	vP1.x = from->x + vDir.x * fT1;
	vP1.y = from->y + vDir.y * fT1;
	vP1.z = vMin.z;

	vP2.x = from->x + vDir.x * fT2;
	vP2.y = from->y + vDir.y * fT2;
	vP2.z = vMax.z;

	if ( (vP1.x > vMax.x && vP2.x > vMax.x) || (vP1.x < vMin.x && vP2.x < vMin.x) ||
		 (vP1.y > vMax.y && vP2.y > vMax.y) || (vP1.y < vMin.y && vP2.y < vMin.y) )
	{
		return effFALSE;
	}
	else
	{
		if ( vP1.x < vMax.x && vP1.x > vMin.x && vP1.y < vMax.y && vP1.y > vMin.y )
		{
			vRet[nIntersect].vPos = vP1;
			vRet[nIntersect].f = fT1;
			nIntersect++;
		}

		if ( nIntersect == 2 )
		{
			if ( vRet[0].f > vRet[1].f )
			{
				*nearPos = vRet[1].vPos;
				*nearDistance = vRet[1].f;
				*farPos = vRet[0].vPos;
				*farDistance = vRet[0].f;
			}
			else
			{
				*nearPos = vRet[0].vPos;
				*nearDistance = vRet[0].f;
				*farPos = vRet[1].vPos;
				*farDistance = vRet[1].f;
			}
			return effTRUE;
		}

		if ( vP2.x < vMax.x && vP2.x > vMin.x && vP2.y < vMax.y && vP2.y > vMin.y )
		{
			vRet[nIntersect].vPos = vP2;
			vRet[nIntersect].f = fT2;
			nIntersect++;
		}

		if ( nIntersect==2 )
		{
			if ( vRet[0].f > vRet[1].f )
			{
				*nearPos = vRet[1].vPos;
				*nearDistance = vRet[1].f;
				*farPos = vRet[0].vPos;
				*farDistance = vRet[0].f;
			}
			else
			{
				*nearPos = vRet[0].vPos;
				*nearDistance = vRet[0].f;
				*farPos = vRet[1].vPos;
				*farDistance = vRet[1].f;
			}
			return effTRUE;
		}
	}

	return effFALSE;
}

/**
	计算线段和三角形的交点，实用的witchside测试
*/
effBOOL EFFSegIntersectTriangle(const EFFVector3 & tr1, const EFFVector3 & tr2, const EFFVector3 & tr3, const EFFSegment3 & seg, EFFVector3 & intersectPos)
{
	EFFPlane3 plane(tr1, tr2, tr3);
	effFLOAT dist;

	EFFVector3 length = seg.end - seg.start;
	if ( !EFFLineIntersectPlane(&dist, &intersectPos, &seg.start, &length, &tr1, &tr2, &tr3) )
	{
		return effFALSE;
	}
	
	if ( EFFWhichSide3D(&intersectPos, &tr3, &tr1) > 0 ) 
	{
		return effFALSE;
	}

	if ( EFFWhichSide3D(&intersectPos, &tr1, &tr2) > 0 ) 
	{
		return effFALSE;
	}

	if ( EFFWhichSide3D(&intersectPos, &tr2, &tr3) > 0 ) 
	{
		return effFALSE;
	}

	return effTRUE;
}

/**
	检测一个3D线段和一个平面相交。如果有一个交点，返回true
	交点返回到isect中
	从u到交点的距离返回到dist，这个距离经过了归一化，
	比如0.5表示是u和v的中点
*/
effBOOL EFFLineIntersectPlane(const EFFVector3 & u, const EFFVector3 & v, effFLOAT A, effFLOAT B, effFLOAT C, effFLOAT D,
							EFFVector3 & intersectPos, effFLOAT & distance)
{
	effFLOAT x, y, z, denom;

	x = v.x - u.x;
	y = v.y - u.y;
	z = v.z - u.z;						//x,y,z构成了vu向量

	denom = A * x + B * y + C * z;		//直线与平面法向量点乘|UV|*|NORM|*cos(alpha)；
										//因为A，B，C构成了单位向量，|NORM|=1；
										//denom等于|UV|*cos(alpha)；这个说明了直线相对平面的倾斜程度
										//denom也是直线在平面上投影长度的那个三角形的对边（嘿嘿，画图就明白）
										//也就是垂线方向的投影长度

	if ( fabs(denom) < EFFMATH_EPSILON ) 
	{
		return effFALSE;				//sin值很小，说明直线平行于平面
	}

	distance = -(A * u.x + B * u.y + C * u.z + D) / denom;						//前面的括号是U到平面距离，这样，交点比值就出来了
	if ( distance < -EFFMATH_EPSILON || distance > 1.0f + EFFMATH_EPSILON )		//比值太小，太大都是看作不相交
	{
		return effFALSE;
	}

	intersectPos.x = u.x + distance * x;
	intersectPos.y = u.y + distance * y;
	intersectPos.z = u.z + distance * z;	//根据直线方程计算交点喽

	return effTRUE;
}

effBOOL EFFLineIntersectPlane(const EFFVector3 & u, const EFFVector3 & v, const EFFPlane3 & p, EFFVector3 & intersectPos, effFLOAT & distance)
{
	effFLOAT x, y, z, denom;

	x = v.x - u.x;
	y = v.y - u.y;
	z = v.z - u.z;

	denom = p.normal.x * x + p.normal.y * y + p.normal.z * z;

	if ( fabs(denom) < EFFMATH_EPSILON ) 
	{
		return effFALSE;	//平行
	}

	distance = -((p.normal | u) + p.d) / denom;			//u点到平面的距离 除以 他与平面夹角余玄 得到 直线上交点倒U点的距离

	if ( distance < -EFFMATH_EPSILON || distance > 1.0f + EFFMATH_EPSILON ) 
	{
		return effFALSE;
	}

	intersectPos.x = u.x + distance * x;
	intersectPos.y = u.y + distance * y;
	intersectPos.z = u.z + distance * z;

	return effFALSE;
}

/**
	计算多边形poly和平面plane的交点，如果没有交点，返回false，有的话segment
	就是交线，当segment的start，end相同，表示只有一个交点
*/
effBOOL EFFPlaneIntersectPolygon(const EFFPlane3 & plane, EFFPolygon3 * poly, EFFSegment3 & seg)
{
	EFFVector3 & v1 = seg.start;
	EFFVector3 & v2 = seg.end;

	effINT i, i1;
	effFLOAT c, c1;
	EFFVector3 sect;
	effFLOAT dist;

	i1 = poly->GetVerticesCount() - 1;					//得到多边形的边数
	c1 = (plane.normal | (*poly)[i1]) + plane.d;		//计算最后点到平面距离

	effBOOL found_v1 = effFALSE;
	effBOOL found_v2 = effFALSE;

	for ( i = 0; i < poly->GetVerticesCount(); i++ )
	{
		c = (plane.normal | (*poly)[i]) + plane.d;

		if ( (c < 0 && c1 > 0) || (c1 < 0 && c > 0) )	//分别在平面的两边
		{
			EFFLineIntersectPlane((*poly)[i1], (*poly)[i], plane, sect, dist);		//计算这条线段与平面的交点

			if ( !found_v1 ) 
			{ 
				v1 = sect; 
				found_v1 = effTRUE;						//找到了一个交点
			}
			else 
			{ 
				v2 = sect; 
				found_v2 = effTRUE;						//找到第二个交点，凸多边形最多2个交点
				break; 
			}
		}

		i1 = i;
		c1 = c;
	}

	if ( !found_v1 ) 
	{
		return effFALSE;	//没有一个交点
	}

	if ( !found_v2 ) 
	{
		v2 = v1;			//只有一个交点的情况，很特殊哦：）
	}

	return effTRUE;
}

//	This function uses the following formula to compute the returned matrix.
//	P = normalize(Plane);
//	-2 * P.a * P.a + 1  -2 * P.b * P.a      -2 * P.c * P.a        0
//	-2 * P.a * P.b      -2 * P.b * P.b + 1  -2 * P.c * P.b        0
//	-2 * P.a * P.c      -2 * P.b * P.c      -2 * P.c * P.c + 1    0
//	-2 * P.a * P.d      -2 * P.b * P.d      -2 * P.c * P.d        1
effVOID EFFReflect(EFFMATRIX4 * m, const EFFPLANE * plane)
{
	/*EFFPLANE p;
	EFFVector4Normalize((EFFVector4 *)&p,(EFFVector4 *)plane);
	m->m11 = -2.0f * p.m_vNormal.x * p.m_vNormal.x + 1.0f;
	m->m12 = -2.0f * p.m_vNormal.y * p.m_vNormal.x;
	m->m13 = -2.0f * p.m_vNormal.z * p.m_vNormal.x;
	m->m14 = 0.0f;

	m->m21 = -2.0f * p.m_vNormal.x * p.m_vNormal.y;
	m->m22 = -2.0f * p.m_vNormal.y * p.m_vNormal.y + 1.0f;
	m->m23 = -2.0f * p.m_vNormal.z * p.m_vNormal.y;
	m->m24 = 0.0f;

	m->m31 = -2.0f * p.m_vNormal.x * p.m_vNormal.z;
	m->m32 = -2.0f * p.m_vNormal.y * p.m_vNormal.z;
	m->m33 = -2.0f * p.m_vNormal.z * p.m_vNormal.z + 1.0f;
	m->m34 = 0.0f;

	m->m41 = -2.0f * p.m_vNormal.x * p.m_fD;
	m->m42 = -2.0f * p.m_vNormal.y * p.m_fD;
	m->m43 = -2.0f * p.m_vNormal.z * p.m_fD;
	m->m44 = 1.0f;*/
} 

//	This function uses the following formula to compute the returned matrix.
//	P = normalize(Plane);
//	L = Light;
//	d = dot(P, L)
//	P.a * L.x + d  P.a * L.y      P.a * L.z      P.a * L.w  
//	P.b * L.x      P.b * L.y + d  P.b * L.z      P.b * L.w  
//	P.c * L.x      P.c * L.y      P.c * L.z + d  P.c * L.w  
//	P.d * L.x      P.d * L.y      P.d * L.z      P.d * L.w + d
effVOID EFFShadow(EFFMATRIX4 * m, const EFFVector4 * l, const EFFPlane3 * plane)
{
	/*EFFPlane p;
	EFFVector4Normalize((EFFVector4 *)&p, (EFFVector4 *)plane);

	effFLOAT d = EFFVector4Dot((EFFVector4 *)&p,l);
	m->m11 = p.m_vNormal.x * l->x + d;
	m->m12 = p.m_vNormal.x * l->y;
	m->m13 = p.m_vNormal.x * l->z;
	m->m14 = p.m_vNormal.x * l->w;

	m->m21 = p.m_vNormal.y * l->x;
	m->m22 = p.m_vNormal.y * l->y + d;
	m->m23 = p.m_vNormal.y * l->z;
	m->m24 = p.m_vNormal.y * l->w;

	m->m31 = p.m_vNormal.z * l->x;
	m->m32 = p.m_vNormal.z * l->y;
	m->m33 = p.m_vNormal.z * l->z + d;
	m->m34 = p.m_vNormal.z * l->w;

	m->m41 = p.m_fD * l->x;
	m->m42 = p.m_fD * l->y;
	m->m43 = p.m_fD * l->z;
	m->m44 = p.m_fD * l->w + d;*/
}



EFFMATH_END