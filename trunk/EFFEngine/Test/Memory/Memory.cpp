// Memory.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>


#define new EFFNEW


class B
{
	RTTI_DECLARE(B,B)
	{
		PROPERTY(m_t)
	}

public:
	BEGIN_PROPERTY_MAP

	END_PROPERTY_MAP


	BEGIN_METHOD_MAP
	END_METHOD_MAP
public:
	std::string m_t;

};

class A
{
	/*DECLARE_CLASS(A)
	{
		MEMBER(m_s)
	}*/

	RTTI_DECLARE(A,A)
	{
		PROPERTY(m_s)
		PROPERTY(m_z)
		PROPERTY_STL_CONTAINER(m_v)
		PROPERTY_STL_CONTAINER(m_t)
		PROPERTY(b)
		PROPERTY_ARRAY(m_pT,m_nNum)
		PROPERTY_ARRAY(m_pB,m_nBNum)
	}
public:
	BEGIN_PROPERTY_MAP
		//REGISTER_PROPERTY_MINMAX(A, m_s, sssssss, int, 0, 1, 10)
	END_PROPERTY_MAP

	BEGIN_METHOD_MAP
		REGISTER_MEMBER_METHOD(test1)
	END_METHOD_MAP

public:
	virtual int test(int);

	int test1(A * pA,int t)
	{
		t += 1;
		return t;
	}


	int m_s;
	std::string m_z;
	std::vector<int> m_v;
	std::vector<std::string> m_t;
	B b;
	int * m_pT;
	int m_nNum;
	B * m_pB;
	int m_nBNum;
};



RTTI_IMPLEMENT(A,0)

//REGISTER_MEMBER_METHOD(A,test1)
REGISTER_PROPERTY_MINMAX(A, m_s, sssssss, int, 0, 1, 10)

//__REGISTER_PROPERTY__(m_s,int)
//__REGISTER_PROPERTY__(A,m_z,std::string)


RTTI_IMPLEMENT(B,0)


class C;

int __stdcall testC(C * pC,int c)
{
	printf("hook c,haha.");
	return c;
}

class IC : public EFFIUnknown
{
	RTTI_DECLARE_PURE(IC,EFFIUnknown)
public:
	virtual ~IC() {}
public:
	virtual  int __stdcall test(int c) = 0;
};

RTTI_IMPLEMENT_PURE(IC,0)



class C : public IC
{
	RTTI_DECLARE(C,IC)
	{
	}

	EFFIUNKNOWN_DECLARE

	BEGIN_INTERFACE_TABLE(C)
		IMPLEMENTS_INTERFACE(EFFIUnknown)
		IMPLEMENTS_INTERFACE(IC)
	END_INTERFACE_TABLE()

public:
	BEGIN_PROPERTY_MAP

	END_PROPERTY_MAP


	BEGIN_METHOD_MAP
	END_METHOD_MAP
public:

public:

	virtual  int __stdcall test(int c);

};

EFFIUNKNOWN_IMPLEMENT(C)

RTTI_IMPLEMENT(C,0)

int C::test(int c)
{
	return c;
}



PROXY_DECLARE(C)
{
	RTTI_DECLARE(CProxy,C)
	{
	}

public:
	BEGIN_PROPERTY_MAP
	END_PROPERTY_MAP

	BEGIN_METHOD_MAP
	END_METHOD_MAP
public:
	PROXY_CONSTRUCTOR(C)

	BEGIN_REGISTER_PROXY_FUNCTION
		REGISTER_PROXY_FUNCTION(5,testC)
	END_REGISTER_PROXY_FUNCTION
};

RTTI_IMPLEMENT(CProxy,0)



int A::test(int a)
{
	printf("%d\n",a);
	return a;
}


class D
{
public:
	void test(D * pD,int z)
	{
		int t = 0;
	}
};


class teststatic
{
public:
	teststatic()
	{
		int z = 0;
		static teststatic a;
	}

};

int _tmain(int argc, _TCHAR* argv[])
{


	D d;
	EFFEvent ed;
	ed += EFFEventCall(&d,&D::test);

	ed(&d,1);

	int * p = new int[10];

	std::vector<int> a;
	a.push_back(1);
	C * pC = new CProxy;
	pC->test(2);
	IC * pI;
	pC->QueryInterface(IC::GetThisClass()->GetID(),(effVOID **)&pI);

	
	A * pA = static_cast<A *>(EFFCreateObject("A"));
	EFFClass * pRuntimeInfo = pA->GetThisClass();
	pA->m_s = 4;
	pA->m_z = "hehe,haha,heihei.";

	pA->GetThisClass()->setProperty(pA, "sssssss", 5);


	pA->m_v.push_back(1);
	pA->m_v.push_back(24);

	pA->m_t.push_back(std::string("memory"));
	pA->m_t.push_back(std::string("kao,kao."));

	pA->b.m_t = "wo shi b,haha.";

	pA->m_nNum = 100;
	pA->m_pT = new int[pA->m_nNum];
	memset(pA->m_pT,20,sizeof(int) * pA->m_nNum);

	pA->m_nBNum = 2;
	pA->m_pB = new B[pA->m_nBNum];
	pA->m_pB[0].m_t = "boost is cool.";
	pA->m_pB[1].m_t = "nb.";


	EFFEvent e;
	e += EFFEventCall(pA,&A::test1);

	EFFSTLFile file;
	file.Open("d:\\1.mesh","wb");
	pA->SaveToFile(&file);
	
	int z = 0;

	B b;

	ClassID c = ClassIDFromString(const_cast<char *>("sdfsdfsdfs"));



	e(pA,z);


	z += 10;

	/*std::ofstream ofs("D:\\1.txt");
	boost::archive::text_oarchive oa(ofs);


	// write class instance to archive
	oa << a;
	// close archive
	ofs.close();*/
}

