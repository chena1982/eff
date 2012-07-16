/******************************************************************************
	created:	2008-12-13   0:42
	file path:	d:\EFF\EFFEngine\Include\EFFMath\EFFMatrix4.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFMatrix4_2008_12_13__
#define __EFFMatrix4_2008_12_13__


EFFMATH_BEGIN

class EFFVector3;

struct EFFMATRIX4
{
	union 
	{
		struct
		{
			effFLOAT				m11, m12, m13, m14;			
			effFLOAT				m21, m22, m23, m24;
			effFLOAT				m31, m32, m33, m34;
			effFLOAT				m41, m42, m43, m44;
		};
		effFLOAT					m[4][4];
	};
};

class EFFMATH_API EFFMatrix4 : public EFFMATRIX4
{
public:
	EFFMatrix4();
	~EFFMatrix4();

};


EFFMATH_API EFFMatrix4 * EFFMatrixPerspectiveFovLH(EFFMatrix4 *pOut,effFLOAT fovy,effFLOAT Aspect,effFLOAT zn,effFLOAT zf);
EFFMATH_API EFFMatrix4 * EFFMatrixLookAtLH(EFFMatrix4 *pOut,const EFFVector3 *pEye,const EFFVector3 *pAt,const EFFVector3 *pUp);

EFFMATH_END

#endif