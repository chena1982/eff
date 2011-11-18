/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef CSTDMF_PCH_HPP
#define CSTDMF_PCH_HPP

#ifdef _WIN32

// identifier was truncated to '255' characters in the browser information
#pragma warning(disable: 4786)

// class name too long
#pragma warning(disable: 4503)

// no matching delete operator for placement new operators, which can't have
// one in any case. applies to the Aligned class
#pragma warning(disable: 4291)

#include "config.hpp"
#include "aligned.hpp"
#include "binaryfile.hpp"
#include "binary_stream.hpp"
#include "debug.hpp"
#include "dogwatch.hpp"
#include "dprintf.hpp"
#include "intrusive_object.hpp"
#include "md5.hpp"
#include "memory_counter.hpp"
#include "memory_stream.hpp"
//#include "profile.hpp"
#include "smartpointer.hpp"
#include "stdmf.hpp"
#include "stpwatch.hpp"
#include "stringmap.hpp"
#include "timestamp.hpp"
#include "vectornodest.hpp"
//#include "watcher.hpp"

#endif // _WIN32

// some D3D commonly used macros
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

//==========================================================================================================
//Singleness
template<class T>
class Singleness
{
public:
	Singleness()
	{
		assert( m_bNew == TRUE );
		m_bNew = FALSE;
	}

	~Singleness()
	{
	}
	static T s_oInstance;
	static BOOL m_bNew;

	static T* instance()
	{
		//if( s_pInstance == NULL )
		//	s_pInstance = new T;
		return &s_oInstance;
	}
};

#define DEF_SINGLENESS(T) \
template<> T Singleness<T>::s_oInstance;\
template<> BOOL Singleness<T>::m_bNew = TRUE;

#define SHADOWMAP  1

////==========================================================================================================
////Singleness
//template<class T>
//class Singleness
//{
//public:
//	Singleness()
//	{
//		assert( m_bNew == TRUE );
//		m_bNew = FALSE;
//	}
//
//	~Singleness()
//	{
//	}
//	static T* s_oInstance;
//	static BOOL m_bNew;
//
//	static T* instance()
//	{
//		if( s_oInstance == NULL )
//			s_oInstance = new T;
//		return s_oInstance;
//	}
//
//	static void releaseSelf()
//	{
//		SAFE_DELETE( s_oInstance );
//	}
//};
//
//#define DEF_SINGLENESS(T) \
//template<> T* Singleness<T>::s_oInstance = 0;\
//template<> BOOL Singleness<T>::m_bNew = TRUE;
#endif // CSTDMF_PCH_HPP
