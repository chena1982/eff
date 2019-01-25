/******************************************************************************
	created:	2012-02-29   22:08
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFFunction.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFFunction_2012_02_29__
#define __EFFFunction_2012_02_29__

#include "EFFProperty.h"

EFFBASE_BEGIN



typedef EFFProperty2 EFFFunctionParameter;

class EFFFunction
{
public:
	EFFFunction();
	~EFFFunction();


public:
	EFFFunctionParameter * AddFunctionParameter();
public:
	VECTOR<EFFFunctionParameter *> parameters;
	EFFFunctionParameter returnType;

	effString name;

	EFFClass * parentClass;

	effDWORD flag;
};

EFFBASE_END

#endif