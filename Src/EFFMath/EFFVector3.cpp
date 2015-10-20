/******************************************************************************
	created:	2009-3-13   0:05
	file path:	d:\EFF\EFFEngine\Src\EFFMath\EFFVector3.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/
#include "EFFMathPCH.h"

#include "EFFMathFunction.h"
#include "EFFVector3.h"



//#define new EFFNEW
EFFMATH_BEGIN

//RTTI_IMPLEMENT_BASE_NO_SAVE(EFFVector3, 0)
/*RTTI_IMPLEMENT_BASE_CUSTOM_SAVE(EFFVector3, 0)

effVOID EFFVector3::SaveToFile(EFFFile * file, effBOOL isBinary, YAML::Emitter * textOut)
{
	if ( isBinary )
	{
		file->Write(this, sizeof(EFFVector3));
	}
	else
	{
		*textOut << YAML::BeginMap;
		*textOut << YAML::Key << "x";
		*textOut << YAML::Value << x;
		*textOut << YAML::Key << "y";
		*textOut << YAML::Value << y;
		*textOut << YAML::Key << "z";
		*textOut << YAML::Value << z;
		*textOut << YAML::EndMap;
	}
}*/

EFFMATH_END