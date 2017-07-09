/******************************************************************************
	created:	2014-3-15   00:22
	file path:	d:\EFF\EFFEngine\Src\EFFBase\YAMLWrap.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __YAMLWrap_2014_3_15__
#define __YAMLWrap_2014_3_15__



EFFBASE_BEGIN

//class EFFObject;
class EFFComponent;
class EFFObjectBase;

EFFBASE_API effBOOL StartSaveToYAMLFile(const effString & filePath);
EFFBASE_API effBOOL EndSaveToYAMLFile();

//effVOID BeginSaveMapToYAMLFile(const effString & tag, const effString & className);
//effVOID EndSaveMapToYAMLFile();

//effVOID BeginSaveObjectToYAMLFile(EFFObject * object);
//effVOID	EndSaveObjectToYAMLFile(EFFObject * object);

EFFBASE_API effVOID SaveStringPropertyToYAMLFile(const effString & propertyName, const effString & value);
EFFBASE_API effVOID SaveStringVectorPropertyToYAMLFile(const effString & propertyName, const VECTOR<effString> & values);

EFFBASE_API effVOID SaveIntPropertyToYAMLFile(const effString & propertyName, effINT value);
EFFBASE_API effVOID SaveIntVectorPropertyToYAMLFile(const effString & propertyName, const VECTOR<effINT> & values);

EFFBASE_API effVOID SaveUintPropertyToYAMLFile(const effString & propertyName, effUINT value);
EFFBASE_API effVOID SaveFloatPropertyToYAMLFile(const effString & propertyName, effFLOAT value);
EFFBASE_API effVOID SaveNoPODPropertyToYAMLFile(const effString & propertyName, EFFComponent * component);


effVOID BeginSaveSequenceToYAMLFile(const effString & sequenceName);
effVOID	EndSaveSequenceToYAMLFile();
EFFBASE_API effVOID SaveComponentToYAMLFile(EFFComponent * component);



effVOID BeginSaveObjectBaseToYAMLFile(EFFObjectBase * objectBase, effINT implicit);
effVOID	EndSaveObjectBaseToYAMLFile(EFFObjectBase * objectBase);
//EFFBASE_API effVOID SaveObjectToYAMLFile(EFFObject * object);

EFFBASE_END

#endif