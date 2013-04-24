/******************************************************************************
	created:	2012-03-04   1:03
	file path:	d:\EFF\trunk\EFFEngine\Include\EFF3D\EFF3DCamera.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DCamera_2012_03_04__
#define __EFF3DCamera_2012_03_04__



EFF3D_BEGIN

class EFF3DAutoParamDataSource;



class EFF3D_API EFF3DCamera : public EFF3DObject
{
	RTTI_DECLARE(EFF3DCamera, EFF3DObject)
public:
	EFF3DCamera();
	~EFF3DCamera();
public:

	inline effINT						GetDepth() { return depth; }
	inline effUINT						GetLayerMask() { return layerMask; }
	inline EFF3DAutoParamDataSource *	GetAutoParamDataSource() { return autoParamDataSource; }

	effVOID								SetViewParams(const EFFVector3 & eye, const EFFVector3 & lookAt);
	effVOID								SetProjParams(effFLOAT fov, effFLOAT aspect, effFLOAT nearPlane, effFLOAT farPlane);

	const EFFMatrix4 &					GetViewMatirx() { return view; }
	const EFFMatrix4 &					GetProjMatrix() { return proj; }
protected:
	effINT								depth;
	EFF3DAutoParamDataSource *			autoParamDataSource;
	effUINT								layerMask;

	EFFVector3							eye;
	EFFVector3							lookAt;

	EFFMatrix4							view;
	EFFMatrix4							proj;
};


EFF3D_END


#endif