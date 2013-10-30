/******************************************************************************
	created:	2008-12-23   2:12
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFEvent.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFEvent.h"

//#include <boost/python.hpp>

//#define new EFFNEW

EFFBASE_BEGIN

EFFEvent::EFFEvent()
{
}


EFFEvent::~EFFEvent()
{
	for ( effUINT i = 0; i < m_aryFunction.size(); i++ )
	{
		SF_DELETE(m_aryFunction[i]);
	}

	m_aryFunction.clear();
}

//using namespace boost::python;


/*BOOST_PYTHON_MODULE(EFFBase)
{
	class_<EFFEvent>("EFFEvent")
		.def("AddScriptEventCall",&EFFEvent::AddScriptEventCall)
		.def("AddScriptMemberEventCall",&EFFEvent::AddScriptMemberEventCall)
		;
}*/

EFFBASE_END