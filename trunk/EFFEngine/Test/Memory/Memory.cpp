// Memory.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>

#define new EFFNEW



class A
{
	/*DECLARE_CLASS(A)
	{
		MEMBER(m_s)
	}*/

	RTTI_DECLARE(A,A)
	{
		__PROPERTY_METADATA__(m_s,int)
	}

public:
	int test(int);

	void test1(int,A *) {};

	//__MEMBER_METHOD__(2,test1,A)
	int m_s;
	std::string m_z;
};



RTTI_IMPLEMENT(A,0)

__REGISTER_MEMBER_METHOD__(A,test1)

__REGISTER_PROPERTY__(A,m_s,int)
__REGISTER_PROPERTY__(A,m_z,std::string)

int A::test(int a)
{
	printf("%d\n",a);
	return a;
}

class B
{
};

int _tmain(int argc, _TCHAR* argv[])
{

	int * p = new int[10];

	A a;
	EFFClass * pRuntimeInfo = a.GetThisClass();
	a.m_s = 4;

	EFFEvent e(&A::test1);

	EFFWinFile file;
	file.Open("d:\\1.mesh","wb");
	a.SaveToFile(&file);
	
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

