#include "stdafx.h"

#include "EFFProperty.h"
#include "EFFClass.h"

EFFBASE_BEGIN

EFFProperty::EFFProperty()
{
	offset = 0;
	size = 0;
	defaultValue = NULL;
	minValue = NULL;
	maxValue = NULL;
	Class = NULL;
	savePropertyFP = NULL;
}

EFFProperty::~EFFProperty()
{

}

EFFBASE_END