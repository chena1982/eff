/******************************************************************************
	created:	2012-10-22   23:37
	file path:	d:\EFF\EFFEngine\Include\MMOEncapsulation\MMOApplication.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __MMOApplication_2012_10_22__
#define __MMOApplication_2012_10_22__





class MMO_API MMOApplication
{
public:
	MMOApplication();
	~MMOApplication();

public:
	effBOOL Init(effBOOL window, effINT width, effINT height);
	effVOID Run();

protected:
	effBOOL	CreateAppWindow(effBOOL window, effINT width, effINT height);
	effVOID	Update();
	effVOID Render(effFLOAT elapsedTime);
protected:
	EFF3DDevice * device;
};


#endif
