#pragma once


class EFFMATH_API Vector3
{
public:
	float x;
	float y;
	float z;

	void test()
	{
		float * p = &x;
		SF_DELETE(p);
	}
};