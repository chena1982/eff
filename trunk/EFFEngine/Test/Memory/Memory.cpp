// Memory.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>

class B
{
	RTTI_DECLARE_BASE(B)
	int t;
};

class A
{
	RTTI_DECLARE_BASE(A)
public:
	effINT m_z;
	effString m_s;
	B m_b;
};

RTTI_IMPLEMENT_BASE(A, 0)
RTTI_IMPLEMENT_BASE(B, 0)

//RTTI_PROPERTY(A, m_z, _effT("heightz"))
//RTTI_PROPERTY(A, m_b, _effT("zzz"))

int _tmain(int argc, _TCHAR* argv[])
{


	//EFFClass * pClass = effINT::GetThisClass();
	effString * p = (effString *)EFFCreateObject(_effT("effString"));
	*p = effString(_effT("nihao"));


	
	A * a = (A *)EFFCreateObject(_effT("A"));
	a->m_z = 10;
	a->m_s = _effT("sssssssss");
	EFFClass * AClass = a->GetThisClass();
	AClass->AddProperty(&A::m_z, _effT("height"));
	AClass->AddProperty(&A::m_s, _effT("name"));

	a->SaveToFile(_effT("111"));

	effFLOAT * f = (effFLOAT *)EFFCreateObject(_effT("effFLOAT"));
	*f = 0.0f;
}

