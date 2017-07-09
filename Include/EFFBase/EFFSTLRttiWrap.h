/******************************************************************************
	created:	2012-02-23   20:38
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFSTLRttiWrap.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFSTLRttiWrap_2012_02_23__
#define __EFFSTLRttiWrap_2012_02_23__


EFFBASE_BEGIN

class EFFFile;

/*class EFFBASE_API effVector_Wrap
{
	RTTI_DECLARE_BASE(effVector_Wrap)
};


template<typename T>
class effVector : public effVector_Wrap
{
public:
	effVector()
	{
	}
public:
	inline T operator [](effINT index) { return VECTOR[index]; }
	inline effVOID push_back(const T & element) { VECTOR.push_back(element); }

	virtual effVOID SaveToFile(EFFFile * file)
	{
		if ( VECTOR.size() == 0 )
		{
			return;
		}

		if ( GetRuntimeClass()->IsPODType() )
		{
			file->Write(&VECTOR[0], sizeof(T) * VECTOR.size());
		}
		else
		{
			SaveProperty(file, VECTOR[0]);
		}
	}
protected:
	VECTOR<T> VECTOR;
};


template<typename T>
class effVector<T *> : public effVector_Wrap
{
public:
	effVector()
	{
	}
public:
	inline T * operator [](effINT index) { return VECTOR[index]; }

	effVOID push_back(T * element) { VECTOR.push_back(element); }

	virtual effVOID SaveToFile(EFFFile * file)
	{
		if ( VECTOR.size() == 0 )
		{
			return;
		}

		if ( GetRuntimeClass()->IsPODType() )
		{
			file->Write(&VECTOR[0], sizeof(T) * VECTOR.size());
		}
		else
		{
			SaveProperty(file, *VECTOR[0]);
		}
	}
protected:
	VECTOR<T *> VECTOR;
};*/


EFFBASE_END

#endif