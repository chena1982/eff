/******************************************************************************
	created:	2014-3-15   00:22
	file path:	d:\EFF\EFFEngine\Src\EFFBase\YAMLWrap.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFScopeGuard.h"
#include "EFFObject.h"
#include "EFFComponent.h"
#include "EFFClass.h"
#include "EFFProperty.h"
#include <yaml.h>

EFFBASE_BEGIN


int write_handler(void * ext, unsigned char * buffer, size_t size)
{
	/*
		...
		Write `size` bytes.
		...
	*/
	return 1;
}

yaml_document_t document;
yaml_emitter_t emitter;
yaml_event_t yamlEvent;
FILE * outputFile = NULL;

effBOOL StartSaveToYAMLFile(const effString & filePath)
{




	/* Set a file output. */
	outputFile = NULL;
	errno_t error = fopen_s(&outputFile, EFFSTRING2ANSI(filePath), "wb");
	if (outputFile == NULL)
	{
		return effFALSE;
	}



	//yaml_document_initialize(&document, &version, &tag[0], &tag[1], 0, 0);
	

	/* Create the Emitter object. */
	yaml_emitter_initialize(&emitter);

	yaml_emitter_set_output_file(&emitter, outputFile);

	/* Set a generic writer. */
	effVOID * ext = NULL;


	//yaml_emitter_set_output(&emitter, write_handler, ext);

	/* Create and emit the STREAM-START event. */
	yaml_stream_start_event_initialize(&yamlEvent, YAML_UTF8_ENCODING);
	if (!yaml_emitter_emit(&emitter, &yamlEvent))
	{
		yaml_emitter_delete(&emitter);
		return effFALSE;
	}





	return effTRUE;
}

effVOID SaveScalarToFile(const effString & value)
{
	yaml_document_add_scalar(&document, NULL, (yaml_char_t *)EFFSTRING2UTF8(value), 0, YAML_PLAIN_SCALAR_STYLE);
}


effVOID BeginSaveMapToYAMLFile(const effString & tag, const effString & className)
{



	//int objectNodeId = yaml_document_add_mapping(&document, (yaml_char_t *)EFFSTRING2UTF8(newTag), YAML_BLOCK_MAPPING_STYLE);


	

	//int objectNameId = yaml_document_add_scalar(&document, NULL, (yaml_char_t *)scalarValue, -1, YAML_PLAIN_SCALAR_STYLE);
	//int propertiesNodeId = yaml_document_add_mapping(&document, NULL, YAML_BLOCK_MAPPING_STYLE);

	//yaml_document_append_mapping_pair(&document, objectNodeId, objectNameId, propertiesNodeId);








}

effVOID EndSaveMapToYAMLFile()
{
	yaml_mapping_end_event_initialize(&yamlEvent);
	yaml_emitter_emit(&emitter, &yamlEvent);
}

effVOID BeginSaveObjectBaseToYAMLFile(EFFObjectBase * objectBase, effINT implicit)
{


	yaml_version_directive_t version;
	version.major = 1;
	version.minor = 1;

	yaml_tag_directive_t tag[2];
	tag[0].prefix = (yaml_char_t *)"tag:eff.com,2014:";
	tag[0].handle = (yaml_char_t *)"!eff!";

	tag[1].prefix = NULL;
	tag[1].handle = NULL;

	yaml_document_start_event_initialize(&yamlEvent, &version, &tag[0], &tag[1], implicit);
	yaml_emitter_emit(&emitter, &yamlEvent);
	


	EFFClass * Class = objectBase->GetRuntimeClass();

	effWCHAR tagStr[32];
	swprintf_s(tagStr, 32, L"tag:eff.com,2014:%s", Class->GetName().c_str());

	effCHAR id[32];
	sprintf_s(id, 32, "%u", objectBase->GetID());

	yaml_mapping_start_event_initialize(&yamlEvent, (yaml_char_t *)id, (yaml_char_t *)EFFSTRING2UTF8(effString(tagStr)), 0, YAML_BLOCK_MAPPING_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	const effCHAR * scalarValue = EFFSTRING2UTF8(Class->GetName());
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	yaml_mapping_start_event_initialize(&yamlEvent, NULL, NULL, 1, YAML_BLOCK_MAPPING_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);
}

effVOID	EndSaveObjectBaseToYAMLFile(EFFObjectBase * objectBase)
{
	EndSaveMapToYAMLFile();
	EndSaveMapToYAMLFile();

	yaml_document_end_event_initialize(&yamlEvent, 1);
	yaml_emitter_emit(&emitter, &yamlEvent);
}

effVOID SaveStringPropertyToYAMLFile(const effString & propertyName, const effString & value)
{
	const effCHAR * scalarValue = EFFSTRING2UTF8(propertyName);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	scalarValue = EFFSTRING2UTF8(value);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);
}

effVOID SaveStringVectorPropertyToYAMLFile(const effString & propertyName, const std::vector<effString> & values)
{
	BeginSaveSequenceToYAMLFile(propertyName);

	for ( effUINT i = 0; i < values.size(); i++ )
	{
		const effCHAR * scalarValue = EFFSTRING2UTF8(values[i]);
		yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
		yaml_emitter_emit(&emitter, &yamlEvent);
	}

	EndSaveSequenceToYAMLFile();
}

effVOID SaveIntPropertyToYAMLFile(const effString & propertyName, effINT value)
{
	const effCHAR * scalarValue = EFFSTRING2UTF8(propertyName);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	effCHAR buffer[32];
	sprintf_s(buffer, 32, "%d", value);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)buffer, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);
}

effVOID SaveIntVectorPropertyToYAMLFile(const effString & propertyName, const std::vector<effINT> & values)
{
	BeginSaveSequenceToYAMLFile(propertyName);

	for ( effUINT i = 0; i < values.size(); i++ )
	{
		effCHAR buffer[32];
		sprintf_s(buffer, 32, "%d", values[i]);
		yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)buffer, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
		yaml_emitter_emit(&emitter, &yamlEvent);
	}

	EndSaveSequenceToYAMLFile();
}

effVOID SaveUintPropertyToYAMLFile(const effString & propertyName, effUINT value)
{
	const effCHAR * scalarValue = EFFSTRING2UTF8(propertyName);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	effCHAR buffer[32];
	sprintf_s(buffer, 32, "%u", value);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)buffer, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);
}

effVOID SaveFloatPropertyToYAMLFile(const effString & propertyName, effFLOAT value)
{
	const effCHAR * scalarValue = EFFSTRING2UTF8(propertyName);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	effCHAR buffer[32];
	sprintf_s(buffer, 32, "%f", value);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)buffer, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);
}

effVOID SaveNoPODPropertyToYAMLFile(const effString & propertyName, EFFComponent * component)
{
	const effCHAR * scalarValue = EFFSTRING2UTF8(propertyName);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	yaml_mapping_start_event_initialize(&yamlEvent, NULL, NULL, 1, YAML_FLOW_MAPPING_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)"fileID", -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	effCHAR buffer[32];
	sprintf_s(buffer, 32, "%u", component->GetID());
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)buffer, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	yaml_mapping_end_event_initialize(&yamlEvent);
	yaml_emitter_emit(&emitter, &yamlEvent);
}

effVOID BeginSaveSequenceToYAMLFile(const effString & sequenceName)
{
	const effCHAR * scalarValue = EFFSTRING2UTF8(sequenceName);
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	yaml_sequence_start_event_initialize(&yamlEvent, NULL, NULL, 1, YAML_BLOCK_SEQUENCE_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

}

effVOID	EndSaveSequenceToYAMLFile()
{
	yaml_sequence_end_event_initialize(&yamlEvent);
	yaml_emitter_emit(&emitter, &yamlEvent);
}

effVOID SaveComponentToYAMLFile(EFFComponent * component)
{


	yaml_mapping_start_event_initialize(&yamlEvent, NULL, NULL, 1, YAML_BLOCK_MAPPING_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	EFFClass * Class = component->GetRuntimeClass();

	const effCHAR * scalarValue = EFFSTRING2UTF8(Class->GetName());
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)scalarValue, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	yaml_mapping_start_event_initialize(&yamlEvent, NULL, NULL, 1, YAML_FLOW_MAPPING_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)"fileID", -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	effCHAR buffer[32];
	sprintf_s(buffer, 32, "%u", component->GetID());
	yaml_scalar_event_initialize(&yamlEvent, NULL, NULL, (yaml_char_t *)buffer, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	yaml_emitter_emit(&emitter, &yamlEvent);

	yaml_mapping_end_event_initialize(&yamlEvent);
	yaml_emitter_emit(&emitter, &yamlEvent);

	yaml_mapping_end_event_initialize(&yamlEvent);
	yaml_emitter_emit(&emitter, &yamlEvent);
}

effBOOL EndSaveToYAMLFile()
{
	ON_SCOPE_EXIT([&] { fclose(outputFile); })

	ON_SCOPE_EXIT([&] {
		/* Destroy the Emitter object. */
		yaml_emitter_delete(&emitter);
	})

	/* Create and emit the STREAM-END event. */
	yaml_stream_end_event_initialize(&yamlEvent);
	if (!yaml_emitter_emit(&emitter, &yamlEvent))
	{
		return effFALSE;
	}

	//yaml_emitter_dump(&emitter, &document);

	return effTRUE;
}






EFFBASE_END