/******************************************************************************
created:	2018-11-17   1:27
file path:	d:\EFF\Src\EFFBase\EFFFunction.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFFBasePCH.h"

#include "EFFFunction.h"

EFFBASE_BEGIN

EFFFunction::EFFFunction()
{
	parentClass = NULL;
	flag = 0;
}


EFFFunction::~EFFFunction()
{

}


EFFFunctionParameter * EFFFunction::AddFunctionParameter()
{
	EFFFunctionParameter * parameter = EFFNEW EFFFunctionParameter;
	parameters.push_back(parameter);
	return parameter;
}

EFFBASE_END