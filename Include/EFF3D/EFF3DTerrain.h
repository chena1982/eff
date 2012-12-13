/********************************************************************
	created:	23:12:2009   21:14
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DTerrain.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3DTerrain_2009_12_23__
#define __EFF3DTerrain_2009_12_23__

EFF3D_BEGIN

class EFF3DTerrainTile;
class EFF3DDevice;

class EFF3D_API EFF3DTerrain
{
public:
	EFF3DTerrain();
	~EFF3DTerrain();

public:
	//effVOID						AddTerrainTile(EFF3DTerrainTile * pTerrainTile);

	virtual effVOID						Render(EFF3DDevice * pDevice) = 0;
protected:


};

effVOID EFF3D_API TerrainPreproccess(effTCHAR * filePath,EFF3DTerrain * pTerrain);

effVOID EFF3D_API TestMyTerrain();

EFF3D_END

#endif


