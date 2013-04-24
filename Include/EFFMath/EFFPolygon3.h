/********************************************************************
	created:	2013-02-05   21:47
	file path:	d:\EFF\EFFEngine\Include\EFFMath\EFFPolygon3.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFPolygon3_2013_02_05__
#define __EFFPolygon3_2013_02_05__

#include "EFFVector3.h"

EFFMATH_BEGIN

class EFFMatrix4;
class EFFPlane3;

enum GeometryRelativity
{
	INFRONT,
	BEHIND,
	COINCIDE,
	CROSS
};

enum PolygonSplitResult
{
	Front,
	Back,
	TwoFrontOneBack,
	OneFrontTwoBack,
	OneFrontOneBack,
};

class EFFMATH_API EFFPolygon3
{

public:
	EFFPolygon3(effINT vertexMaxNum = 12);

	EFFPolygon3(const EFFPolygon3 & rhs);

	EFFPolygon3 & operator = (const EFFPolygon3 & rhs);

	~EFFPolygon3();

	effVOID GetInversePoly(EFFPolygon3 * result);

	inline effVOID SetEmpty()
	{
		verticesCount = 0; 
	}

	inline effINT GetVerticesCount() const
	{ 
		return verticesCount; 
	}

	inline EFFVector3 * GetVertices() const
	{ 
		return vertices; 
	}

	inline EFFVector3 * GetVertex(effINT i) const
	{
		if ( i < 0 || i >= verticesCount )
		{
			return NULL;
		}

		return &(vertices[i]);
	}

	inline EFFVector3 & operator[] (effINT i)
	{
		return vertices[i];
	}

	inline const EFFVector3 & operator[] (effINT i) const
	{
		return vertices[i];
	}

	EFFVector3 * GetFirst () const
	{ 
		if ( verticesCount <= 0 ) 
		{
			return NULL;  
		}

		return vertices; 
	}

	EFFVector3 * GetLast() const
	{ 
		if ( verticesCount <= 0 ) 
		{
			return NULL; 
		}
 
		return &vertices[verticesCount-1]; 
	}

	//测试一个顶点是否在多边形内部
	effBOOL IsPointIn(const EFFVector3 & v) const
	{
		effINT i, i1;
		i1 = verticesCount - 1;

		for ( i = 0; i < verticesCount; i++ )
		{
			if ( EFFWhichSide3D(&v, &vertices[i1], &vertices[i]) < 0 ) 
			{
				return effFALSE;
			}

			i1 = i;
		}
		return effTRUE;
	}

	static effBOOL IsPointIn(EFFVector3 * polyVertices, effINT verticesCount, const EFFVector3 & v);

	effVOID Transform(EFFMatrix4 & m);
	effVOID Transform(EFFVector3 & trans);

	//添加一个顶点到多边形，返回被添加的顶点的索引
	effINT AddVertex(const EFFVector3 & v)
	{ 
		return AddVertex(v.x, v.y, v.z); 
	}

	effINT AddVertex(effFLOAT x, effFLOAT y, effFLOAT z);

	effVOID SetVertices(EFFVector3 * v, effINT count)
	{
		verticesCount = count;
		memcpy(vertices, v, verticesCount * sizeof(EFFVector3)); 
	}

	//通过point点透视投影这个多边形到一个X值为plane_x的垂直X轴的平面，得到一个EFFPolygon2对象，
	//这里假定拥有确切的投影，如果投影成为一个点，那么函数返回false
	/*bool ProjectXPlane (const EFFVector3& point, effFLOAT plane_x,
  			uc3dxPolygon2* poly2d) const;
	bool ProjectYPlane (const EFFVector3& point, effFLOAT plane_y,
  			uc3dxPolygon2* poly2d) const;
	bool ProjectZPlane (const EFFVector3& point, effFLOAT plane_z,
  			uc3dxPolygon2* poly2d) const;

	//整合了的投影
	bool ProjectAxisPlane (const EFFVector3& point, int plane_nr,
			effFLOAT plane_pos, uc3dxPolygon2* poly2d) const{
		switch (plane_nr){
		case 0: return ProjectXPlane (point, plane_pos, poly2d); 
		case 1: return ProjectYPlane (point, plane_pos, poly2d); 
		case 2: return ProjectZPlane (point, plane_pos, poly2d); 
		}
		return false;
	}


	effVOID Project(EFFMatrix4 * WVP, EFFPolygon2 * poly2d);

	//正交投影
	effVOID ProjectXPlane(EFFPolygon2 * poly2d);
	effVOID ProjectYPlane(EFFPolygon2 * poly2d);
	effVOID ProjectZPlane(EFFPolygon2 * poly2d);

	*/


	//区分这个多边形在一个平面的那个位置
	//如果在这个面上，返回POL_SAME_PLANE
	//如果完全再这个平面前面，返回POL_FRONT
	//如果完全再这个平面背后，返回POL_BACK
	//否则，返回POL_SPLIT_NEEDED
	effINT Classify(const EFFPlane3 & plane) const;
	
	//垂直坐标轴的特殊情况
	effINT ClassifyX(effFLOAT x) const;
	effINT ClassifyY(effFLOAT y) const;
	effINT ClassifyZ(effFLOAT z) const;

	//分割这个多边形，并且只留下前面的
	effVOID CutToPlane(const EFFPlane3 & plane);

	//用给定的平面，分割这个多边形
	effINT SplitWithPlane(EFFPolygon3 & front, EFFPolygon3 & back, const EFFPlane3 & plane) const;

	//分割特殊情况
	effVOID SplitWithPlaneX(EFFPolygon3 & front, EFFPolygon3 & back, effFLOAT x) const;
	effVOID SplitWithPlaneY(EFFPolygon3 & front, EFFPolygon3 & back, effFLOAT y) const;
	effVOID SplitWithPlaneZ(EFFPolygon3 & front, EFFPolygon3 & back, effFLOAT z) const;
	
	//计算法向量
	static EFFVector3 ComputeNormal(EFFVector3 * vertices, effINT count);

	EFFVector3 ComputeNormal() const
	{
		return ComputeNormal(vertices, verticesCount);
	}

	//计算多边形的平面
	static EFFPlane3 ComputePlane(EFFVector3 * vertices, effINT count);

	//计算这个多边形的平面
	inline EFFPlane3 ComputePlane() const;

	effVOID Resize(effINT newMaxCount);

	effVOID SetVerticesCount(effINT count)
	{
		Resize(count);
		verticesCount = count;
	}


	//计算多边形面积
	effFLOAT GetSignedArea();
  
	//计算多边形中心
	EFFVector3 GetCenter() const;

	effVOID Order(effBOOL CW = effTRUE);

	effBOOL LineIntersect(effFLOAT * t, EFFVector3 * point, const EFFVector3 & from, const EFFVector3 & to);


public:
	EFFVector3 *	vertices;			//3d顶点
	effINT			verticesCount;		//顶点数目
	effINT			verticesMaxCount;	//最大顶点数目
};


EFFMATH_API effINT PointRelativePolygon(const EFFVector3 & point, const EFFPolygon3 & polygon);

EFFMATH_API effINT PolygonRelativePolygon(const EFFPolygon3 & polygon1, const EFFPolygon3 & polygon2);

EFFMATH_API effBOOL IsPolygonInfrontOrCoincide(const EFFPolygon3 & polygon1, const EFFPolygon3 & polygon2);

EFFMATH_API effINT SplitPolygon(EFFPolygon3 & polygonToSplit, EFFPolygon3 & polygonDivider, EFFPolygon3 * result);



EFFMATH_END


#endif