/******************************************************************************
	created:	2008-12-15   2:02
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DMaterial.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/


#include "EFF3DPCH.h"

#include "EFF3DMaterial.h"

//#include <boost/python.hpp>

//#define new EFFNEW

EFF3D_BEGIN

 //using namespace boost::python;

EFF3DMaterial::EFF3DMaterial()
{
	shader = NULL;
	//m_pScriptModule = NULL;
}


EFF3DMaterial::~EFF3DMaterial()
{
}

static EFF3DMaterial * g_pCurrentMaterial = NULL;

EFF3DMaterial * GetCurrentMaterial()
{
	return g_pCurrentMaterial;
}

effBOOL EFF3DMaterial::CreateFromFile(const effString & filePath)
{


	g_pCurrentMaterial = this;

	/*m_pScriptModule = PyImport_ImportModule(szFilePath);//������Ҫ���õ��ļ���
	if ( m_pScriptModule != NULL )
	{
		PyObject * pFunc = PyObject_GetAttrString(m_pScriptModule, "InitMaterialScript");//������Ҫ���õĺ�����
		if ( pFunc != NULL )
		{
			call<void>(pFunc,ptr(this));
			//PyEval_CallObject(pFunc, NULL);//���ú���
			PyErr_Print();

			//�����йܵ�python�Ĵ���ĵ���
			//OnMaterialBeforeRender(this);
			PyErr_Print();

			return effTRUE;
		}
	}
	else
	{
		PyErr_Print();
	}*/



	return effFALSE;
}





/*BOOST_PYTHON_MODULE(EFF3D)
{


	class_<EFF3DMaterial>("EFF3DMaterial")
		.def_readonly("OnMaterialBeforeRender",&EFF3DMaterial::OnMaterialBeforeRender)
	;

	def("GetCurrentMaterial",&GetCurrentMaterial,return_value_policy<reference_existing_object>());
}*/





EFF3D_END