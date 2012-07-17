#include "stdafx.h"


#include "EFFProperty.h"
#include "EFFFile.h"
#include "EFFRtti.h"


EFFBASE_BEGIN

/*EFFProperty::EFFProperty()
{
	offset = 0;
	size = 0;
	defaultValue = NULL;
	minValue = NULL;
	maxValue = NULL;
	Class = NULL;

	isPointer = effFALSE;
	isArray = effFALSE;
	isSTLContainer = effFALSE;
	savePropertyFP = NULL;
}

EFFProperty::~EFFProperty()
{

}*/

/*effVOID EFFStringProperty::SaveToFile(EFFFile * file, effVOID * baseAddress)
{
	effString & data = *((effString *)((effBYTE *)baseAddress + offset));
	effUINT length = data.length();
	file->Write(&length, 4);
	file->Write((effVOID *)data.c_str(), length * sizeof(effTCHAR));
}

effVOID EFFPODProperty::SaveToFile(EFFFile * file, effVOID * baseAddress)
{
	effVOID * source = (effVOID *)((effBYTE *)baseAddress + offset);
	file->Write(source, size);
}*/
effINT EFFProperty::GetElementCount(effVOID * baseAddress)
{
	if ( stlContainerType == EFFProperty::ContainerType_Vector )
	{
		effString propertyClassName = Class->GetName();
		if ( propertyClassName == _effT("effString") )
		{
			std::vector<effString> & propertyVector = *((std::vector<effString> *)((effBYTE *)baseAddress + offset));
			return propertyVector.size();
		}
	}

	return 0;
}



EFFBASE_END