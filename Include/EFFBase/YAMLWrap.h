/******************************************************************************
	created:	2014-3-15   00:22
	file path:	d:\EFF\EFFEngine\Src\EFFBase\YAMLWrap.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __YAMLWrap_2014_3_15__
#define __YAMLWrap_2014_3_15__

#include "EFFSTLFile.h"
#include <yaml/yaml.h>

EFFBASE_BEGIN

//class EFFObject;
class EFFComponent;
class EFFObjectBase;

EFFBASE_API effBOOL StartSaveToYAMLFile(EFFSTLFile & outputFile);
EFFBASE_API effBOOL EndSaveToYAMLFile();

//effVOID BeginSaveMapToYAMLFile(const effString & tag, const effString & className);
//effVOID EndSaveMapToYAMLFile();

//effVOID BeginSaveObjectToYAMLFile(EFFObject * object);
//effVOID	EndSaveObjectToYAMLFile(EFFObject * object);


extern yaml_document_t document;
extern yaml_emitter_t emitter;
extern yaml_event_t yamlEvent;
extern FILE * outputFile;

effVOID SaveIntPropertyToYAMLFile(const effString & propertyName, effINT value);
effVOID SaveIntVectorPropertyToYAMLFile(const effString & propertyName, const VECTOR<effINT> & values);

effVOID SaveUintPropertyToYAMLFile(const effString & propertyName, effUINT value);
effVOID SaveFloatPropertyToYAMLFile(const effString & propertyName, effFLOAT value);
effVOID SaveNoPODPropertyToYAMLFile(const effString & propertyName, EFFComponent * component);


effVOID BeginSaveSequenceToYAMLFile(const effString & sequenceName);
effVOID	EndSaveSequenceToYAMLFile();
EFFBASE_API effVOID SaveComponentToYAMLFile(EFFComponent * component);



effVOID BeginSaveObjectBaseToYAMLFile(EFFObjectBase * objectBase, effINT implicit);
effVOID	EndSaveObjectBaseToYAMLFile(EFFObjectBase * objectBase);
//EFFBASE_API effVOID SaveObjectToYAMLFile(EFFObject * object);

template<typename Type>
effVOID SavePropertyToYAML(const effString & propertyName, const Type & data)
{

}

template<>
inline effVOID SavePropertyToYAML(const effString & propertyName, const effString & value)
{
	const effCHAR * scalarValue = EFFSTRING2UTF8(propertyName);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	scalarValue = EFFSTRING2UTF8(value);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);
}

template<>
inline effVOID SavePropertyToYAML(const effString & propertyName, const VECTOR<effString> & values)
{
	BeginSaveSequenceToYAMLFile(propertyName);

	for (effSIZE i = 0; i < values.size(); i++)
	{
		const effCHAR * scalarValue = EFFSTRING2UTF8(values[i]);
		yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
		yaml_emitter_emit(&emitter, &yamlEvent);
	}

	EndSaveSequenceToYAMLFile();
}





EFFBASE_END

#endif