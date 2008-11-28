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

	END_PROPERTY_MAP

	BEGIN_METHOD_MAP
		REGISTER_MEMBER_METHOD(test1)
	END_METHOD_MAP

public:
	int test(int);

	void test1(int,A *) {};


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

//__REGISTER_MEMBER_METHOD__(A,test1)

//__REGISTER_PROPERTY__(m_s,int)
//__REGISTER_PROPERTY__(A,m_z,std::string)


RTTI_IMPLEMENT(B,0)

int A::test(int a)
{
	printf("%d\n",a);
	return a;
}



int _tmain(int argc, _TCHAR* argv[])
{

	int * p = new int[10];

	A * pA = static_cast<A *>(EFFCreateObject("A"));
	EFFClass * pRuntimeInfo = pA->GetThisClass();
	pA->m_s = 4;
	pA->m_z = "hehe,haha,heihei.";


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


	EFFEvent e(&A::test1);

	EFFSTLFile file;
	file.Open("d:\\1.mesh","wb");
	pA->SaveToFile(&file);
	
	int z;

	B b;

	ClassID c = ClassIDFromString(const_cast<char *>("sdfsdfsdfs"));

	size_t n = sizeof(*(e.m_pFunction));

	e(z,&b,1);

	z += 10;

	/*std::ofstream ofs("D:\\1.txt");
	boost::archive::text_oarchive oa(ofs);


	// write class instance to archive
	oa << a;
	// close archive
	ofs.close();*/
}

