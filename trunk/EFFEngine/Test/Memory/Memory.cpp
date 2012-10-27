// Memory.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>

class A;

class B
{
	RTTI_DECLARE_BASE(B)
	int t;

	void * test(int * z, A & d, A * s)
	{
		return NULL;
	}

	void test1()
	{
	}
};

class A
{
	RTTI_DECLARE_BASE(A)
public:
	effINT m_z;
	effString m_s;
	B m_b;
	std::vector<effString> m_v;
	std::vector<B *> m_t;
	std::vector<B>	m_r;
	std::vector<effINT>	m_u;

	std::vector<effString *> m_n;
};

RTTI_IMPLEMENT_BASE(A, 0)
RTTI_IMPLEMENT_BASE(B, 0)

//RTTI_PROPERTY(A, m_z, _effT("heightz"))
//RTTI_PROPERTY(A, m_b, _effT("zzz"))

template<typename containerType>
class C
{
public:
	int z;
};

int _tmain(int argc, _TCHAR* argv[])
{


	//EFFClass * pClass = effINT::GetThisClass();
	effString * p = (effString *)EFFCreateObject(_effT("effString"));
	*p = effString(_effT("nihao"));

	char tttt = 0xc8;
	effUINT yu = ((effUINT)tttt);

	effString str = _effT("哈哈");
	effUINT hash = EFFStringHash(str.c_str()).GetHash();

	effUINT hash2 = EFFStringHash(_effT("哈哈")).GetHash();
	if ( hash == hash2 )
	{
		int z = 4;
	}
	
	A * a = (A *)EFFCreateObject(_effT("A"));
	a->m_z = 10;
	a->m_s = _effT("sssssssss");
	EFFClass * AClass = a->GetThisClass();
	//AClass->AddProperty(&A::m_z, _effT("height"));

	//AClass->AddProperty(&A::m_u, _effT("sss"));
	a->m_u.push_back(1);
	AClass->AddProperty(&A::m_s, _effT("name"));
	//AClass->AddProperty(&A::m_b, _effT("Bzz"));
	//AClass->AddProperty(&A::m_v, _effT("vector"));
	//AClass->AddProperty(&A::m_n, _effT("string pointer Vector"));

	AClass->AddProperty(&A::m_t, _effT("BVector"));
	//AClass->AddProperty(&A::m_r, _effT("B"));

	effString name;
	//A::GetThisClass()->GetProperty(_effT("name"), a, name);

	B * b = (B *)EFFCreateObject(_effT("B"));
	b->t = 100;

	a->m_v.push_back(_effT("chenwu"));
	a->m_t.push_back(b);

	a->SaveToFile(_effT("111"));

	EFFEvent test;
	test += EFFEventCall(b, &B::test1);
	test();

	std::vector<B *> z;
	z.push_back(new B());

	B * t = z[0];

	effFLOAT * f = (effFLOAT *)EFFCreateObject(_effT("effFLOAT"));
	*f = 0.0f;
}

