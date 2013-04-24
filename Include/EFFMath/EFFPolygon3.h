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

	//����һ�������Ƿ��ڶ�����ڲ�
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

	//���һ�����㵽����Σ����ر���ӵĶ��������
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

	//ͨ��point��͸��ͶӰ�������ε�һ��XֵΪplane_x�Ĵ�ֱX���ƽ�棬�õ�һ��EFFPolygon2����
	//����ٶ�ӵ��ȷ�е�ͶӰ�����ͶӰ��Ϊһ���㣬��ô��������false
	/*bool ProjectXPlane (const EFFVector3& point, effFLOAT plane_x,
  			uc3dxPolygon2* poly2d) const;
	bool ProjectYPlane (const EFFVector3& point, effFLOAT plane_y,
  			uc3dxPolygon2* poly2d) const;
	bool ProjectZPlane (const EFFVector3& point, effFLOAT plane_z,
  			uc3dxPolygon2* poly2d) const;

	//�����˵�ͶӰ
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

	//����ͶӰ
	effVOID ProjectXPlane(EFFPolygon2 * poly2d);
	effVOID ProjectYPlane(EFFPolygon2 * poly2d);
	effVOID ProjectZPlane(EFFPolygon2 * poly2d);

	*/


	//��������������һ��ƽ����Ǹ�λ��
	//�����������ϣ�����POL_SAME_PLANE
	//�����ȫ�����ƽ��ǰ�棬����POL_FRONT
	//�����ȫ�����ƽ�汳�󣬷���POL_BACK
	//���򣬷���POL_SPLIT_NEEDED
	effINT Classify(const EFFPlane3 & plane) const;
	
	//��ֱ��������������
	effINT ClassifyX(effFLOAT x) const;
	effINT ClassifyY(effFLOAT y) const;
	effINT ClassifyZ(effFLOAT z) const;

	//�ָ��������Σ�����ֻ����ǰ���
	effVOID CutToPlane(const EFFPlane3 & plane);

	//�ø�����ƽ�棬�ָ���������
	effINT SplitWithPlane(EFFPolygon3 & front, EFFPolygon3 & back, const EFFPlane3 & plane) const;

	//�ָ��������
	effVOID SplitWithPlaneX(EFFPolygon3 & front, EFFPolygon3 & back, effFLOAT x) const;
	effVOID SplitWithPlaneY(EFFPolygon3 & front, EFFPolygon3 & back, effFLOAT y) const;
	effVOID SplitWithPlaneZ(EFFPolygon3 & front, EFFPolygon3 & back, effFLOAT z) const;
	
	//���㷨����
	static EFFVector3 ComputeNormal(EFFVector3 * vertices, effINT count);

	EFFVector3 ComputeNormal() const
	{
		return ComputeNormal(vertices, verticesCount);
	}

	//�������ε�ƽ��
	static EFFPlane3 ComputePlane(EFFVector3 * vertices, effINT count);

	//�����������ε�ƽ��
	inline EFFPlane3 ComputePlane() const;

	effVOID Resize(effINT newMaxCount);

	effVOID SetVerticesCount(effINT count)
	{
		Resize(count);
		verticesCount = count;
	}


	//�����������
	effFLOAT GetSignedArea();
  
	//������������
	EFFVector3 GetCenter() const;

	effVOID Order(effBOOL CW = effTRUE);

	effBOOL LineIntersect(effFLOAT * t, EFFVector3 * point, const EFFVector3 & from, const EFFVector3 & to);


public:
	EFFVector3 *	vertices;			//3d����
	effINT			verticesCount;		//������Ŀ
	effINT			verticesMaxCount;	//��󶥵���Ŀ
};


EFFMATH_API effINT PointRelativePolygon(const EFFVector3 & point, const EFFPolygon3 & polygon);

EFFMATH_API effINT PolygonRelativePolygon(const EFFPolygon3 & polygon1, const EFFPolygon3 & polygon2);

EFFMATH_API effBOOL IsPolygonInfrontOrCoincide(const EFFPolygon3 & polygon1, const EFFPolygon3 & polygon2);

EFFMATH_API effINT SplitPolygon(EFFPolygon3 & polygonToSplit, EFFPolygon3 & polygonDivider, EFFPolygon3 * result);



EFFMATH_END


#endif