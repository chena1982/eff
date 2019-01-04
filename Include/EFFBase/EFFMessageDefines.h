/******************************************************************************
	created:	2014-12-06   23:18
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFMessageDefines.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFMessageDefines_2014_12_06__
#define __EFFMessageDefines_2014_12_06__


EFFBASE_BEGIN

#define RequestGameWindowPosAndSize 0


#define SendGameWindowPosAndSize 1

#define RequestApp 2
#define SendApp 3

#define RequestSharedTexture 4
#define SendSharedTexture 5

#define QuitApp 7

struct GameWindowPosAndSize
{
	effINT id;
	effINT x;
	effINT y;
	effINT width;
	effINT height;
};

struct ApplicationInfo
{
	effINT id;
	effVOID * app;
};

struct SharedTextureInfo
{
	SharedTextureInfo()
	{
		for (effUINT i = 0; i < 3; i++)
		{
			sharedTextureHandle[i] = NULL;
		}

		memset(name, 0, sizeof(name));
	}

    effINT id;
    effUINT width;
    effUINT height;
    effUINT format;
    effHANDLE sharedTextureHandle[3];
    effTCHAR name[32];
};


EFFBASE_END

#endif