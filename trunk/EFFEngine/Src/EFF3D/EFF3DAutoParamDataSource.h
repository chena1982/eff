/******************************************************************************
	created:	2008-12-15   2:23
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DAutoParamDataSource.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DAutoParamDataSource_2008_12_15__
#define __EFF3DAutoParamDataSource_2008_12_15__


EFF3D_BEGIN

class EFF3DRenderable;

class EFF3DAutoParamDataSource
{
public:
	EFF3DAutoParamDataSource();
	~EFF3DAutoParamDataSource();
public:
	effVOID										SetRenderable(EFF3DRenderable * renderable) { this->renderable = renderable; }
protected:
	EFF3DRenderable *							renderable;
};

EFF3D_END

#endif