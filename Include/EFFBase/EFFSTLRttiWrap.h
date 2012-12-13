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
	inline T operator [](effINT index) { return vector[index]; }
	inline effVOID push_back(const T & element) { vector.push_back(element); }

	virtual effVOID SaveToFile(EFFFile * file)
	{
		if ( vector.size() == 0 )
		{
			return;
		}

		if ( GetRuntimeClass()->IsPODType() )
		{
			file->Write(&vector[0], sizeof(T) * vector.size());
		}
		else
		{
			SaveProperty(file, vector[0]);
		}
	}
protected:
	std::vector<T> vector;
};


template<typename T>
class effVector<T *> : public effVector_Wrap
{
public:
	effVector()
	{
	}
public:
	inline T * operator [](effINT index) { return vector[index]; }

	effVOID push_back(T * element) { vector.push_back(element); }

	virtual effVOID SaveToFile(EFFFile * file)
	{
		if ( vector.size() == 0 )
		{
			return;
		}

		if ( GetRuntimeClass()->IsPODType() )
		{
			file->Write(&vector[0], sizeof(T) * vector.size());
		}
		else
		{
			SaveProperty(file, *vector[0]);
		}
	}
protected:
	std::vector<T *> vector;
};*/


EFFBASE_END

#endif