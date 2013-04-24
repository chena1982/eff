/******************************************************************************
	created:	2013-02-05   21:49
	file path:	d:\EFF\EFFEngine\Src\EFFMath\EFFPolygon3.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/
#include "stdafx.h"

#include "EFFMathFunction.h"
#include "EFFPolygon3.h"
#include "EFFPlane3.h"
#include "EFFMatrix4.h"
#include "EFFAabb.h"


EFFMATH_BEGIN

#define qisqrt(x) (1.0f / sqrt(x))
#define fqisqrt(x) ((effFLOAT)(1.0f / sqrt(x)))

#define POLYGON_EPSILON 0.1f

EFFPolygon3::EFFPolygon3(effINT maxCount)
{
	verticesMaxCount = maxCount;
	vertices = new EFFVector3[maxCount];
	SetEmpty();
}

EFFPolygon3::EFFPolygon3(const EFFPolygon3 & copy)
{
	verticesMaxCount = copy.verticesMaxCount;
	vertices = new EFFVector3[verticesMaxCount];
	verticesCount = copy.verticesCount;
	memcpy(vertices, copy.vertices, sizeof (EFFVector3) * verticesCount);
}

EFFPolygon3 & EFFPolygon3::operator = (const EFFPolygon3 & copy)
{
	delete [] vertices;
	
	verticesMaxCount = copy.verticesMaxCount;
	vertices = new EFFVector3[verticesMaxCount];
	verticesCount = copy.verticesCount;
	memcpy(vertices, copy.vertices, sizeof (EFFVector3) * verticesCount);

	return *this;
}

EFFPolygon3::~EFFPolygon3()
{
	delete [] vertices;
}

effVOID EFFPolygon3::GetInversePoly(EFFPolygon3 * result)
{
	EFFPolygon3 temp(verticesMaxCount);
	temp.verticesMaxCount = verticesMaxCount;
	temp.verticesCount = verticesCount;

	EFFVector3 * pVertices = vertices + verticesCount - 1;
	for ( effINT i = 0; i < verticesCount; i++ )
	{
		temp.vertices[i] = *pVertices;
		pVertices--;
	}

	*result = temp;
}

effBOOL EFFPolygon3::IsPointIn(EFFVector3 * polyVertices, effINT verticesCount, const EFFVector3 & v)
{
	effINT i, i1;
	i1 = verticesCount - 1;
	for ( i = 0 ; i < verticesCount; i++ )
	{
		if ( EFFWhichSide3D(&v, &polyVertices[i1], &polyVertices[i]) < 0 ) 
		{
			return effFALSE;
		}
		i1 = i;
	}

	return effTRUE;
}

inline EFFPlane3 EFFPolygon3::ComputePlane() const
{
	return ComputePlane(vertices, verticesCount);
}

/*effBOOL EFFPolygon3::IsPointIn(const EFFVector3 & v) const
{
	EFFPolygon2 poly2;
	EFFVector3 vNormal = ComputeNormal();
	if ( vNormal.LargestAxis() == _X ) 
	{
		ProjectXPlane(&poly2);
		return poly2.In(uc3dxVector2(v.y,v.z));
	}
	else if ( vNormal.LargestAxis() == _Y ) 
	{
		ProjectYPlane(&poly2);
		return poly2.In(uc3dxVector2(v.x,v.z));
	}
	else if( vNormal.LargestAxis() == _Z )
	{
		ProjectZPlane(&poly2);
		return poly2.In(uc3dxVector2(v.x,v.y));
	}
	//unable to here.
	return ucTRUE;
}*/

effVOID EFFPolygon3::Transform(EFFMatrix4 & m)
{
	for ( effINT i = 0; i < verticesCount; i++ ) 
	{
		vertices[i] *= m;
	}
	
}

effVOID EFFPolygon3::Transform(EFFVector3 & trans)
{
	for ( effINT i = 0; i < verticesCount; i++ ) 
	{
		vertices[i] += trans;
	}
}

effVOID EFFPolygon3::Resize(effINT newMaxCount)
{
	if ( newMaxCount <= verticesMaxCount ) 
	{
		return;
	}

	EFFVector3 * newVertices = new EFFVector3[newMaxCount];
	memcpy(newVertices, vertices, verticesCount * sizeof(EFFVector3));
	delete [] vertices;

	vertices = newVertices;
	verticesMaxCount = newMaxCount;
}

effINT EFFPolygon3::AddVertex(effFLOAT x, effFLOAT y, effFLOAT z)
{
	if ( verticesCount >= verticesMaxCount )
	{
		Resize(verticesMaxCount + 5);
	}

	vertices[verticesCount].x = x;
	vertices[verticesCount].y = y;
	vertices[verticesCount].z = z;
	verticesCount++;

	return verticesCount - 1;
}

/*bool EFFPolygon3::ProjectXPlane(const EFFVector3 & point, effFLOAT plane_x,
	uc3dxPolygon2* poly2d) const
{
	poly2d->MakeEmpty();
	uc3dxVector2 p;
	EFFVector3 v;
	effFLOAT x_dist = plane_x - point.x;
	effINT i;
	for (i = 0 ; i < verticesCount ; i++)
	{
		v = vertices[i]-point;
		if (ucABS (v.x) < SMALL_EPSILON) 
			return false;
		p.x = point.y + x_dist * v.y / v.x;
		p.y = point.z + x_dist * v.z / v.x;
		poly2d->AddVertex(p);
	}
	return true;
}

bool EFFPolygon3::ProjectYPlane (const EFFVector3& point, effFLOAT plane_y,
	uc3dxPolygon2* poly2d) const
{
	poly2d->MakeEmpty ();
	uc3dxVector2 p;
	EFFVector3 v;
	effFLOAT y_dist = plane_y - point.y;
	effINT i;
	for (i = 0 ; i < verticesCount ; i++)
	{
		v = vertices[i]-point;
		if (ucABS (v.y) < SMALL_EPSILON) 
			return false;
		p.x = point.x + y_dist * v.x / v.y;
		p.y = point.z + y_dist * v.z / v.y;
		poly2d->AddVertex (p);
	}
	return true;
}

bool EFFPolygon3::ProjectZPlane (const EFFVector3& point, effFLOAT plane_z,
	uc3dxPolygon2* poly2d) const
{
	poly2d->MakeEmpty ();
	uc3dxVector2 p;
	EFFVector3 v;
	effFLOAT z_dist = plane_z - point.z;
	effINT i;
	for (i = 0 ; i < verticesCount ; i++)
	{
	    v = vertices[i]-point;
		if (ucABS (v.z) < SMALL_EPSILON) return false;
		p.x = point.x + z_dist * v.x / v.z;
		p.y = point.y + z_dist * v.y / v.z;
		poly2d->AddVertex (p);
	}
	return true;
}

effVOID EFFPolygon3::ProjectXPlane(uc3dxPolygon2 * poly2d)
{
	UC_ASSERT(poly2d != NULL);
	poly2d->MakeEmpty();
	for ( effINT i = 0; i < verticesCount; i++ ) 
	{
		uc3dxVector2 p;
		p.x = vertices[i].y;
		p.y = vertices[i].z;
		poly2d->AddVertex(p);
	}
}


effVOID EFFPolygon3::ProjectYPlane(uc3dxPolygon2* poly2d)
{
	UC_ASSERT(poly2d != NULL);
	poly2d->MakeEmpty();
	for ( effINT i = 0; i < verticesCount; i++ ) 
	{
		uc3dxVector2 p;
		p.x = vertices[i].x;
		p.y = vertices[i].z;
		poly2d->AddVertex(p);
	}
}

effVOID EFFPolygon3::ProjectZPlane(uc3dxPolygon2* poly2d)
{
	UC_ASSERT(poly2d != NULL);
	poly2d->MakeEmpty();
	for ( effINT i = 0; i < verticesCount; i++ ) 
	{
		uc3dxVector2 p;
		p.x = vertices[i].x;
		p.y = vertices[i].y;
		poly2d->AddVertex(p);
	}
}

effVOID EFFPolygon3::Project(uc3dxMatrix4 * pWorldViewProj,uc3dxPolygon2 * pPolygon)
{
	UC_ASSERT(pWorldViewProj != NULL && pPolygon != NULL);
	pPolygon->MakeEmpty();
	for( effINT i = 0; i < verticesCount; i++ )
	{
		uc3dxVector4 vResult = uc3dxVector4(vertices[i],1.0f) * (*pWorldViewProj);
		pPolygon->AddVertex(vResult.x/fabs(vResult.w),vResult.y/fabs(vResult.w));
	}
	pPolygon->UpdateBoundBox();
}*/

effINT EFFPolygon3::Classify(const EFFPlane3 & plane) const
{
	effINT i;
	effINT front = 0, back = 0;

	for ( i = 0; i < verticesCount; i++ )
	{
		effFLOAT distance = (plane.normal | vertices[i]) + plane.d;

		if ( distance < -POLYGON_EPSILON ) 
		{
			back++;
		}
		else if ( distance > POLYGON_EPSILON ) 
		{
			front++;
		}
	}

	if ( back == 0 && front == 0 ) 
	{
		return COINCIDE;
	}

	if ( back == 0 ) 
	{
		return INFRONT;
	}

	if ( front == 0 )
	{
		return BEHIND;
	}

	return CROSS;
}

effINT EFFPolygon3::ClassifyX(effFLOAT x) const
{
	effINT i;
	effINT front = 0, back = 0;

	for ( i = 0; i < verticesCount; i++ )
	{
		effFLOAT xx = vertices[i].x - x;
		if ( xx < -POLYGON_EPSILON ) 
		{
			front++;
		}
		else if ( xx > POLYGON_EPSILON ) 
		{
			back++;
		}
	}

	if ( back == 0 )
	{
		return INFRONT;
	}

	if ( front == 0 )
	{
		return BEHIND;
	}

	return CROSS;
}

effINT EFFPolygon3::ClassifyY(effFLOAT y) const
{
	effINT i;
	effINT front = 0, back = 0;

	for ( i = 0; i < verticesCount; i++ )
	{
		effFLOAT yy = vertices[i].y - y;
		if ( yy < -POLYGON_EPSILON ) 
		{
			front++;
		}
		else if ( yy > POLYGON_EPSILON ) 
		{
			back++;
		}
	}

	if ( back == 0 )
	{
		return INFRONT;
	}

	if ( front == 0 )
	{
		return BEHIND;
	}

	return CROSS;
}

effINT EFFPolygon3::ClassifyZ(effFLOAT z) const
{
	effINT i;
	effINT front = 0, back = 0;

	for ( i = 0; i < verticesCount; i++ )
	{
		effFLOAT zz = vertices[i].z - z;
		if ( zz < -POLYGON_EPSILON ) 
		{
			front++;
		}
		else if ( zz > POLYGON_EPSILON ) 
		{
			back++;
		}
	}

	if ( back == 0 )
	{
		return INFRONT;
	}

	if ( front == 0 )
	{
		return BEHIND;
	}

	return CROSS;
}

effINT EFFPolygon3::SplitWithPlane(EFFPolygon3 & front, EFFPolygon3 & back, const EFFPlane3 & plane) const
{
	front.SetEmpty();	//等待接受分割后的前面部分
	back.SetEmpty();	//等待接受分割后的后面部分

	effINT frontCount = 0;
	effINT backCount = 0;

	for ( effINT i = 0; i < verticesCount; i++ ) 
	{
		effFLOAT result = (vertices[i] | plane.normal) + plane.d;

		if ( result > POLYGON_EPSILON ) 
		{
			frontCount++;
		}
		else if ( result < -POLYGON_EPSILON ) 
		{
			backCount++;
		}
		else
		{
			frontCount++;
			backCount++;
		}
	}

	if ( frontCount == verticesCount && backCount != verticesCount ) 
	{
		return INFRONT;
	}
	else if ( backCount == verticesCount && frontCount != verticesCount )
	{
		return BEHIND;
	}
	else if ( frontCount == verticesCount && backCount == verticesCount ) 
	{
		return COINCIDE;
	}
	
	EFFVector3 ptB;
	effFLOAT sideA, sideB;
	EFFVector3 ptA = vertices[verticesCount - 1];		//取出多边形最后的顶点
	sideA = (plane.normal | ptA) + plane.d;				//顶点到这个分割平面的距离
	if ( fabs(sideA) < POLYGON_EPSILON )				//顶点在分割平面上
	{
		sideA = 0;
	}

	frontCount = 0;
	backCount = 0;

	for ( effINT i = 0; i < verticesCount; i++ )
	{
		ptB = vertices[i];
		sideB = (plane.normal | ptB) + plane.d;

		if ( fabs(sideB) < POLYGON_EPSILON ) 
		{
			sideB = 0;
		}

		if ( sideB > 0 )
		{
			if ( sideA < 0 )		//最后顶点在分割平面后方
			{
				//计算直线与分割平面的交点，这是一个简单的线----面交叉
				EFFVector3 v = ptB; 
				v -= ptA;//得到一个向量，终点是当前计算点，起点是多边形最后点
				effFLOAT sect = -((plane.normal | ptA) + plane.d) / ( plane.GetNormal() | v );

				v *= sect;
				v += ptA;

				front.AddVertex(v);
				back.AddVertex(v);

				backCount++;
				frontCount++;
			}

			front.AddVertex(ptB);
			frontCount++;
		}
		else if ( sideB < 0 )
		{
			if ( sideA > 0 )
			{
				//计算直线与分割平面的交点，这是一个简单的线----面交叉
				EFFVector3 v = ptB; v -= ptA;
				effFLOAT sect = -((plane.normal | ptA) + plane.d) / ( plane.GetNormal () | v );

				v *= sect;
				v += ptA;
				front.AddVertex(v);
				back.AddVertex(v);

				frontCount++;
				backCount++;
			}

			back.AddVertex(ptB);
			backCount++;
		}
		else
		{
			front.AddVertex(ptB);
			back.AddVertex(ptB);
			backCount++;
			frontCount++;
		}

		ptA = ptB;
		sideA = sideB;
	}

	return CROSS;
}

void EFFPolygon3::CutToPlane(const EFFPlane3 & plane)
{
	EFFPolygon3 old(*this);
	SetEmpty();

	EFFVector3 ptB;
	effFLOAT sideA, sideB;

	EFFVector3 ptA = old.vertices[old.verticesCount - 1];
	sideA = (plane.normal | ptA) + plane.d;

	if ( fabs(sideA) < EFFMATH_EPSILON ) 
	{
		sideA = 0.0f;
	}

	for (effINT i = 0; i < old.verticesCount; i++ )
	{
		ptB = old.vertices[i];
		sideB = (plane.normal | ptB) + plane.d;

		if ( fabs(sideB) < EFFMATH_EPSILON ) 
		{
			sideB = 0.0f;
		}

		if ( sideB > 0.0f )
		{
			if ( sideA < 0.0f )
			{
				EFFVector3 v = ptB;
				v -= ptA;

				effFLOAT sect = - ((plane.normal | ptA) + plane.d) / (plane.GetNormal() | v);

				v *= sect;
				v += ptA;
				AddVertex(v);
			}
		}
		else if ( sideB < 0.0f )
		{
			if ( sideA > 0.0f )
			{
				EFFVector3 v = ptB;
				v -= ptA;

				effFLOAT sect = -((plane.normal | ptA) + plane.d) / ( plane.GetNormal() | v);

				v *= sect;
				v += ptA;
				AddVertex(v);
			}
			AddVertex(ptB);
		}
		else
		{
			AddVertex(ptB);
		}
		ptA = ptB;
		sideA = sideB;
	}
}

effVOID EFFPolygon3::SplitWithPlaneX(EFFPolygon3 & poly1, EFFPolygon3 & poly2, effFLOAT x) const
{
	poly1.SetEmpty();
	poly2.SetEmpty();

	EFFVector3 ptB;
	effFLOAT sideA, sideB;

	EFFVector3 ptA = vertices[verticesCount - 1];
	sideA = ptA.x - x;

	if ( fabs(sideA) < EFFMATH_EPSILON ) 
	{
		sideA = 0.0f;
	}

	for ( effINT i = 0; i < verticesCount; i++ )
	{
		ptB = vertices[i];
		sideB = ptB.x - x;
		if ( fabs(sideB) < EFFMATH_EPSILON ) 
		{
			sideB = 0.0f;
		}

		if ( sideB > 0.0f )
		{
			if ( sideA < 0.0f )
			{
				EFFVector3 v = ptB;
				v -= ptA;

				effFLOAT sect = -(ptA.x - x) / v.x;

				v *= sect;
				v += ptA;

				poly1.AddVertex(v);
				poly2.AddVertex(v);
			}

			poly2.AddVertex(ptB);
		}
		else if ( sideB < 0.0f )
		{
			if ( sideA > 0.0f )
			{
				EFFVector3 v = ptB;
				v -= ptA;

				effFLOAT sect = -(ptA.x - x) / v.x;

				v *= sect;
				v += ptA;
				poly1.AddVertex(v);
				poly2.AddVertex(v);
			}

			poly1.AddVertex (ptB);
		}
		else
		{
			poly1.AddVertex(ptB);
			poly2.AddVertex(ptB);
		}

		ptA = ptB;
		sideA = sideB;
	}
}

void EFFPolygon3::SplitWithPlaneY(EFFPolygon3 & poly1, EFFPolygon3 & poly2, effFLOAT y) const
{
	poly1.SetEmpty();
	poly2.SetEmpty ();

	EFFVector3 ptB;
	effFLOAT sideA, sideB;
	EFFVector3 ptA = vertices[verticesCount - 1];
	sideA = ptA.y - y;

	if ( fabs(sideA) < EFFMATH_EPSILON ) 
	{
		sideA = 0.0f;
	}

	for ( effINT i = 0; i < verticesCount; i++ )
	{
		ptB = vertices[i];
		sideB = ptB.y - y;

		if ( fabs(sideB) < EFFMATH_EPSILON ) 
		{
			sideB = 0.0f;
		}

		if ( sideB > 0.0f )
		{
			if ( sideA < 0.0f )
			{
				EFFVector3 v = ptB;
				v -= ptA;

				effFLOAT sect = -(ptA.y - y) / v.y;

				v *= sect;
				v += ptA;

				poly1.AddVertex(v);
				poly2.AddVertex(v);
			}
			poly2.AddVertex (ptB);
		}
		else if (sideB < 0)
		{
			if (sideA > 0)
			{
				EFFVector3 v = ptB; v -= ptA;
				effFLOAT sect = - (ptA.y - y) / v.y;
				v *= sect; v += ptA;
				poly1.AddVertex (v);
				poly2.AddVertex (v);
			}

			poly1.AddVertex(ptB);
		}
		else
		{
			poly1.AddVertex(ptB);
			poly2.AddVertex(ptB);
		}

		ptA = ptB;
		sideA = sideB;
	}
}

void EFFPolygon3::SplitWithPlaneZ(EFFPolygon3 & poly1, EFFPolygon3 & poly2, effFLOAT z) const
{
	poly1.SetEmpty();
	poly2.SetEmpty();

	EFFVector3 ptB;
	effFLOAT sideA, sideB;
	EFFVector3 ptA = vertices[verticesCount - 1];
	sideA = ptA.z - z;

	if ( fabs(sideA) < EFFMATH_EPSILON ) 
	{
		sideA = 0.0f;
	}

	for ( effINT i = 0; i< verticesCount; i++ )
	{
		ptB = vertices[i];
		sideB = ptB.z - z;
		if ( fabs(sideB) < EFFMATH_EPSILON ) 
		{
			sideB = 0.0f;
		}

		if ( sideB > 0.0f )
		{
			if ( sideA < 0.0f )
			{
				EFFVector3 v = ptB;
				v -= ptA;
				effFLOAT sect = -(ptA.z - z) / v.z;

				v *= sect;
				v += ptA;

				poly1.AddVertex(v);
				poly2.AddVertex(v);
			}
			poly2.AddVertex(ptB);
		}
		else if ( sideB < 0.0f )
		{
			if ( sideA > 0.0f )
			{
				EFFVector3 v = ptB;
				v -= ptA;
				effFLOAT sect = -(ptA.z - z) / v.z;

				v *= sect;
				v += ptA;
				poly1.AddVertex(v);
				poly2.AddVertex(v);
			}
			poly1.AddVertex(ptB);
		}
		else
		{
			poly1.AddVertex(ptB);
			poly2.AddVertex(ptB);
		}
		ptA = ptB;
		sideA = sideB;
	}
}

EFFVector3 EFFPolygon3::ComputeNormal(EFFVector3* vertices, effINT count)
{
	/*effFLOAT ayz = 0;
	effFLOAT azx = 0;
	effFLOAT axy = 0;
	effINT i, i1;
	effFLOAT x1, y1, z1, x, y, z;

	i1 = num-1;
	for (i = 0 ; i < num; i++)
	{
		x = vertices[i].x;
		y = vertices[i].y;
		z = vertices[i].z;
		x1 = vertices[i1].x;
		y1 = vertices[i1].y;
		z1 = vertices[i1].z;
		ayz += (z1+z) * (y-y1);//按照顺时针，逐个边的计算法向量的累加
		azx += (x1+x) * (z-z1);
		axy += (y1+y) * (x-x1);
		i1 = i;
	}

	effFLOAT sqd = ayz*ayz + azx*azx + axy*axy;
	effFLOAT invd;
	if (sqd < SMALL_EPSILON)
		invd = (effFLOAT)1.0f/SMALL_EPSILON;
	else
		invd = fqisqrt (sqd);
	return EFFVector3 (ayz * invd, azx * invd, axy * invd);*/

	assert(count >= 3);
	EFFVector3 edge0 = vertices[1] - vertices[0];
	EFFVector3 edge1 = vertices[2] - vertices[0];

	//cross
	EFFVector3 normal = edge0 % edge1;
	EFFVector3Normalize(&normal, &normal);
	return normal;
}

EFFPlane3 EFFPolygon3::ComputePlane(EFFVector3* vertices, effINT verticesCount)
{
	effFLOAT D;
	EFFVector3 pl = ComputeNormal(vertices, verticesCount);

	D = -pl.x * vertices[0].x - pl.y * vertices[0].y - pl.z * vertices[0].z;
	return EFFPlane3(pl, D);
}

effFLOAT EFFPolygon3::GetSignedArea()
{
	effFLOAT area = 0.0f;
	//三角链化多边形，三角形是(0,1,2), (0,2,3), (0,3,4), 等等..
	for ( effINT i = 0; i < verticesCount - 2; i++ )
	{
		area += (effFLOAT)EFFArea3D(&vertices[0], &vertices[i+1], &vertices[i+2]);
	}
	return area;
}

EFFVector3 EFFPolygon3::GetCenter () const
{
	effINT i;
	EFFAabb aabb;
	aabb.SetPoint(vertices[0]);
	for ( i = 1; i < verticesCount; i++ )
	{
		aabb.Extend(vertices[i]);
	}
	return aabb.GetCenter();
}

effBOOL IsPolygonInfrontOrCoincide(const EFFPolygon3 & polygon1, const EFFPolygon3 & polygon2)
{
	for( effINT i = 0; i < 3; i++ )
	{
		if ( PointRelativePolygon(polygon1.vertices[i], polygon2) == BEHIND )
		{
			return effFALSE;
		}
	}
	return effTRUE;
}

effINT PointRelativePolygon(const EFFVector3 & point, const EFFPolygon3 & polygon)
{
	EFFVector3 edge0 = polygon.vertices[1] - polygon.vertices[0];
	EFFVector3 edge1 = polygon.vertices[2] - polygon.vertices[0];

	//cross
	EFFVector3 normal = edge0 % edge1;

	EFFVector3Normalize(&normal, &normal);

	//plane equation D value
	effFLOAT d = - (polygon.vertices[0] | normal);

	if ( ((point | normal) + d) > POLYGON_EPSILON )
	{
		return INFRONT;
	}
	else if ( ((point | normal) + d) < -POLYGON_EPSILON )
	{
		return BEHIND;
	}
	else
	{
		return COINCIDE;
	}
}

effINT PolygonRelativePolygon(const EFFPolygon3 & polygon1, const EFFPolygon3 & polygon2)
{
	effINT positiveCount = 0;
	effINT negativeCount = 0;
	for(effINT i=0;i<3;i++)
	{
		switch( PointRelativePolygon(polygon1.vertices[i],polygon2) )
		{
		case INFRONT:
			positiveCount += 1;
			break;
		case BEHIND:
			negativeCount += 1;
			break;
		case COINCIDE:
			break;
		default:
			break;
		}
	}

	if ( (positiveCount > 0) && (negativeCount == 0) )
	{
		return INFRONT;
	}
	else if ( (positiveCount == 0) && (negativeCount > 0) )
	{
		return BEHIND;
	}
	else if ( (positiveCount == 0) && (negativeCount == 0) )
	{
		return COINCIDE;
	}
	else 
	{
		return CROSS;
	}
}

EFFVector3 GetEdgeIntersection(const EFFVector3 & point0, const EFFVector3 & point1, EFFPolygon3 & planePolygon)
{
	EFFVector3 edge1, edge2, planeNormal, pointOnPlane, intersection, temp;
	effFLOAT numerator, denominator, t;

	// get a point on the plane
	pointOnPlane = planePolygon.vertices[0];

	// get the splitting planes normal
	planeNormal = planePolygon.ComputeNormal();

	// find edge intersection:
	// intersection = p0 + (p1 - p0) * t
	// where t = (planeNormal . (pointOnPlane - p0)) / (planeNormal . (p1 - p0))

	//planeNormal . (pointOnPlane - point0)
	temp = pointOnPlane - point0;
	numerator = planeNormal | temp;

	//planeNormal . (point1 - point0)
	temp = point1 - point0;
	denominator = planeNormal | temp;

	if ( denominator )
	{
		t = numerator / denominator;
	}
	else
	{
		t = 0.0f;
	}

	intersection = point0 + temp * t;
	return intersection;
}

/*----------------------------------------------------------------------------------------------
Although this function is called SplitPolygon, it really only splits triangles, the
function inputs the triangle to be split and another triangle to be used for the splitting
plane. The parameter 'result' is a pointer to 3 triangles for output.
The function splits the input triangle into either 0, 2 or 3 new triangles with
recalculated texture coordinates.
If the polygons pointer happens to be NULL then the function will just set the outputFlag and return.

The return value will be either Front, Back, TwoFrontOneBack, OneFrontTwoBack or OneFrontOneBack.
Front means that all points are infront of the plane or the polygon lies on the plane and faces the front.
Back means that all points are behind of the plane or the polygon lies on the plane and faces the back.
TwoFrontOneBack means that polygons 1 and 2 are infront of the plane and polygon 3 is behind.
OneFrontTwoBack means that polygon 1 is infront of the plane and polygons 2 and 3 are behind.
OneFrontOneBack means that polygon 1 is infront of the plane and polygon 2 is behind, polygon 3 is not used.
----------------------------------------------------------------------------------------------*/
effINT SplitPolygon(EFFPolygon3 & polygonToSplit, EFFPolygon3 & polygonDivider, EFFPolygon3 * result)
{
	EFFVector3 back[4], front[4], intersection;
	effINT count = 0, backNum = 0, frontNum = 0;
	//effFLOAT x1, x2, y1, y2, z1, z2, u1, u2, v1, v2, scale; // texture calculation variables

	// get a point on the divider polygon
	EFFVector3 vPointOnDivider = polygonDivider.vertices[0];

	// get the divider plane normal
	EFFVector3 vDividerNormal = polygonDivider.ComputeNormal();

	// get the normal of the polygon to split
	EFFVector3 vPolygonNormal = polygonToSplit.ComputeNormal();


	// find if all of the points are infront of or behind the plane
	EFFVector3 temp;
	effINT frontcount = 0, backcount = 0;
	for ( effINT loop = 0; loop < 3; loop++ )
	{
		temp = polygonToSplit.vertices[loop];
		effINT nResult = PointRelativePolygon(temp, polygonDivider);
		
		if ( nResult == COINCIDE )
		{
			frontcount++;
			backcount++;
		}
		else if ( nResult == INFRONT )
		{
			frontcount++;
		}
		else if ( nResult == BEHIND )
		{
			backcount++;
		}
	}

	if ( frontcount == 3 && backcount == 3 )
	{
		if ( (vPolygonNormal | vDividerNormal) >= 0 )
		{
			return Front;
		}
		else
		{
			return Back;
		}
	}

	if ( frontcount == 3 )
	{
		return Front;
	}
	if ( backcount == 3 )
	{
		return Back;
	}

	// try to split the polygon
	EFFVector3 ptA = polygonToSplit.vertices[2];
	effINT sideA = PointRelativePolygon(ptA, polygonDivider);

	for ( effINT i = 0; i < 3; i++ )
	{
		EFFVector3 ptB = polygonToSplit.vertices[i];
		effINT sideB = PointRelativePolygon(ptB, polygonDivider);

		if ( sideB == INFRONT )
		{
			if ( sideA == BEHIND )
			{
				// find intersection
				intersection = GetEdgeIntersection(ptA, ptB, polygonDivider);

				// find the new texture coordinates
				/*x1 = ptB.x - ptA.x;
				y1 = ptB.y - ptA.y;
				z1 = ptB.z - ptA.z;
				x2 = intersection.x - ptA.x;
				y2 = intersection.y - ptA.y;
				z2 = intersection.z - ptA.z;
				u1 = ptA.u;
				u2 = ptB.u;
				v1 = ptA.v;
				v2 = ptB.v;
				scale = sqrt(x2*x2+y2*y2+z2*z2)/sqrt(x1*x1+y1*y1+z1*z1);
				intersection.u = u1 + (u2-u1) * scale;
				intersection.v = v1 + (v2-v1) * scale;*/

				back[backNum++] = front[frontNum++] = intersection;
			}
			front[frontNum++] = ptB;
		}
		else if ( sideB == BEHIND )
		{
			if ( sideA == INFRONT )
			{
				// find intersection
				intersection = GetEdgeIntersection(ptA, ptB, polygonDivider);

				// find the new texture coordinates
				/*x1 = ptB.x - ptA.x;
				y1 = ptB.y - ptA.y;
				z1 = ptB.z - ptA.z;
				x2 = intersection.x - ptA.x;
				y2 = intersection.y - ptA.y;
				z2 = intersection.z - ptA.z;
				u1 = ptA.u;
				u2 = ptB.u;
				v1 = ptA.v;
				v2 = ptB.v;
				scale = sqrt(x2*x2+y2*y2+z2*z2)/sqrt(x1*x1+y1*y1+z1*z1);
				intersection.u = u1 + (u2-u1) * scale;
				intersection.v = v1 + (v2-v1) * scale;*/

				back[backNum++] = front[frontNum++] = intersection;
			}
			back[backNum++] = ptB;
		}
		else
		{
			back[backNum++] = front[frontNum++] = ptB;
		}
		ptA = ptB;
		sideA = sideB;
	}

	effINT outputFlag = 0;
	if ( frontNum == 4 )          // two polygons are infront, one behind
	{
		outputFlag = TwoFrontOneBack;
		if ( result != NULL )
		{
			result[0].AddVertex(front[0]);
			result[0].AddVertex(front[1]);
			result[0].AddVertex(front[2]);
			result[1].AddVertex(front[0]);
			result[1].AddVertex(front[2]);
			result[1].AddVertex(front[3]);
			result[2].AddVertex(back[0]);
			result[2].AddVertex(back[1]);
			result[2].AddVertex(back[2]);
		}
	}
	else if ( backNum == 4 )    // one polygon is infront, two behind
	{
		outputFlag = OneFrontTwoBack;
		if ( result != NULL)
		{
			result[0].AddVertex(front[0]);
			result[0].AddVertex(front[1]);
			result[0].AddVertex(front[2]);
			result[1].AddVertex(back[0]);
			result[1].AddVertex(back[1]);
			result[1].AddVertex(back[2]);
			result[2].AddVertex(back[0]);
			result[2].AddVertex(back[2]);
			result[2].AddVertex(back[3]);
		}
	}
	else if ( frontNum == 3 && backNum == 3 )  // plane bisects the polygon
	{
		outputFlag = OneFrontOneBack;
		if ( result != NULL )
		{
			result[0].AddVertex(front[0]);
			result[0].AddVertex(front[1]);
			result[0].AddVertex(front[2]);
			result[1].AddVertex(back[0]);
			result[1].AddVertex(back[1]);
			result[1].AddVertex(back[2]);
		}
	}
	else // then polygon must be totally infront of or behind the plane
	{
		effINT side;
		for ( effINT loop = 0; loop < 3; loop++ )
		{
			temp = polygonToSplit.vertices[loop];
			side = PointRelativePolygon(temp, polygonDivider);
			if ( side == INFRONT )
			{
				outputFlag = Front;
				break;
			}
			else if ( side == BEHIND )
			{
				outputFlag = Back;
				break;
			}
		}

	}

	return outputFlag;
}

/*effVOID EFFPolygon3::Order(effBOOL CW)
{
	EFFPolygon2 poly2;
	EFFVector3 vNormal = ComputeNormal();
	if ( vNormal.LargestAxis() == _X ) 
	{
		ProjectXPlane(&poly2);
	}
	else if ( vNormal.LargestAxis() == _Y ) 
	{
		ProjectYPlane(&poly2);
	}
	else if( vNormal.LargestAxis() == _Z )
	{
		ProjectZPlane(&poly2);
	}
	uc3dxVector2 vCenter;
	poly2.GetCenter(vCenter);
	poly2.Transform(-vCenter);
	UCSimpleMap<effFLOAT,effINT> aryAngle;
	for ( effINT i = 0; i < poly2.GetNumVertices(); i++  )
	{
		uc3dxVector2 vVertex = *(poly2.GetVertex(i));
		effFLOAT fLength = vVertex.Length();
		effFLOAT fAngle = acos(vVertex.x / fLength);
		if ( vVertex.y < 0.0f )
		{
			fAngle = 2.0f*UC3D_PI - fAngle;
		}
		aryAngle.Add(fAngle,i);
	}
	EFFPolygon3 temp(verticesMaxCount);
	temp.verticesMaxCount = verticesMaxCount;
	temp.verticesCount = verticesCount;
	for ( effINT i = 0; i < verticesCount; i++ )
	{
		temp.vertices[i] = *(GetVertex(aryAngle.GetValueAt(i)));		
	}
	*this = temp;
}*/

effBOOL EFFPolygon3::LineIntersect(FLOAT * t, EFFVector3 * point, const EFFVector3 & from, const EFFVector3 & to)
{
	EFFVector3 * a = &vertices[verticesCount - 1];
	for ( effINT i = 0; i < verticesCount - 2; i++ )
	{
		EFFVector3 * b = &vertices[i];
		EFFVector3 * c = &vertices[i + 1];

		EFFVector3 len = to - from;
		len.Normalize();

		if ( EFFLineIntersectPlane(t, point, &from, &len, a, b, c) )
		{
			return effTRUE;
		}
	}

	return effFALSE;
}


EFFMATH_END