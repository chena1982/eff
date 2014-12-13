/******************************************************************************
	created:	2008-12-15   1:00
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DIRenderable.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DRenderable.h"

////#define new EFFNEW

EFF3D_BEGIN

//EFFIUNKNOWN_IMPLEMENT(EFF3DIRenderable)

RTTI_IMPLEMENT_PURE(EFF3DRenderable, 0)



EFF3DRenderable::EFF3DRenderable()
{
	material = NULL;

	fullscreenLayer = None;
	transparencyType = Normal;
	needRecalculateRenderOrder = effTRUE;
}



EFF3D_END