/******************************************************************************
	created:	2011-11-30   22:48
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFProperty.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFProperty_2011_22_48__
#define __EFFProperty_2011_22_48__


EFFBASE_BEGIN

class EFFClass;
class EFFProperty;
class EFFFile;

#pragma warning(push)
#pragma warning(disable:4251)




typedef void (* SavePropertyFP)(EFFFile * file, void * baseAddress, EFFProperty * property);

class EFFBASE_API EFFProperty
{
	friend class EFFClass;
public:
	~EFFProperty();

	inline effUINT				GetOffset() { return offset; }
	inline effUINT				GetSize() { return size; }
	inline SavePropertyFP		GetSavePropertyFP() { return savePropertyFP; }
private:
	EFFProperty();
private:
	effUINT				offset;
	effUINT				size;


	effString			name;
	effVOID *			defaultValue;
	effVOID *			minValue;
	effVOID *			maxValue;
	EFFClass *			Class;
	SavePropertyFP		savePropertyFP;
	
};



#pragma warning(pop)

EFFBASE_END


#endif