/******************************************************************************
	created:	2012-03-04   1:03
	file path:	d:\EFF\trunk\EFFEngine\Include\EFF3D\EFF3DCamera.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DCamera_2012_03_04__
#define __EFF3DCamera_2012_03_04__

#include "EFF3DObject.h"

EFF3D_BEGIN

class EFF3DAutoParamDataSource;



class EFF3DCamera : public EFF3DObject
{
	RTTI_DECLARE(EFF3DCamera, EFF3DObject)
public:
	EFF3DCamera();
	~EFF3DCamera();
public:

	inline effINT						GetDepth() { return depth; }
	inline effUINT						GetLayerMask() { return layerMask; }
	inline EFF3DAutoParamDataSource *	GetAutoParamDataSource() { return autoParamDataSource; }
protected:
	effINT								depth;
	EFF3DAutoParamDataSource *			autoParamDataSource;
	effUINT								layerMask;
};


EFF3D_END


#endif