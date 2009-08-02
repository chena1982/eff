/******************************************************************************
	created:	2008-12-15   2:23
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DAutoParamDataSource.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DAutoParamDataSource_2008_12_15__
#define __EFF3DAutoParamDataSource_2008_12_15__


EFF3D_BEGIN

class EFF3DIRenderable;

class EFF3DAutoParamDataSource
{
public:
	EFF3DAutoParamDataSource();
	~EFF3DAutoParamDataSource();
public:
	effVOID										SetRenderable(EFF3DIRenderable * pRenderable);
protected:
	EFF3DIRenderable	 *						m_pRenderable;
};

EFF3D_END

#endif