/********************************************************************
	created:	23:12:2009   22:11
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DTerrainTileData.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3DTerrainTileData_2009_12_23__
#define __EFF3DTerrainTileData_2009_12_23__

#include "EFF3DResource.h"

EFF3D_BEGIN

class EFF3DVertexBuffer;
class EFF3DIndexBuffer;
class EFF3DDevice;

class EFF3D_API EFF3DITerrainTileData : public EFF3DResource
{
public:
	EFF3DITerrainTileData() {}
	~EFF3DITerrainTileData() {}

public:
	virtual EFF3DVertexBuffer *			GetVertexBuffer(effINT nLevel,EFF3DDevice * pDevice) = 0;
	virtual effINT								GetVerticesNum(effINT nLevel) = 0;
	virtual effINT								GetVerticesStride() = 0;
	virtual EFF3DIndexBuffer *			GetIndexBuffer(effINT nLevel,EFF3DDevice * pDevice) = 0;
	virtual effINT								GetIndicesNum(effINT nLevel) = 0;
	virtual effINT								GetTileX() = 0;
	virtual effINT								GetTileZ() = 0;
};




EFF3D_END

#endif