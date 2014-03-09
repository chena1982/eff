/******************************************************************************
	created:	2013-7-16   23:31
	file path:	d:\EFF\Src\EFFBase\EFFInputManager.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFInputManager.h"



EFFBASE_BEGIN

#ifdef _WIN32

static effUINT VKMap[256] =
{
0,              //0x00
VK_ESCAPE,      //0x01
'1',            //0x02
'2',            //0x03
'3',            //0x04
'4',            //0x05
'5',            //0x06
'6',            //0x07
'7',            //0x08
'8',            //0x09
'9',            //0x0A
'0',            //0x0B
VK_OEM_MINUS,   //0x0C
VK_OEM_PLUS,    //0x0D
VK_BACK,        //0x0E
VK_TAB,         //0x0F
'Q',            //0x10
'W',            //0x11
'E',            //0x12
'R',            //0x13
'T',            //0x14
'Y',            //0x15
'U',            //0x16
'I',            //0x17
'O',            //0x18
'P',            //0x19
VK_OEM_4,       //0x1A
VK_OEM_6,       //0x1B
VK_RETURN,      //0x1C
VK_CONTROL,     //0x1D
'A',            //0x1E
'S',            //0x1F
'D',            //0x20
'F',            //0x21
'G',            //0x22
'H',            //0x23
'J',            //0x24
'K',            //0x25
'L',            //0x26
VK_OEM_1,       //0x27
VK_OEM_7,       //0x28
VK_OEM_3,       //0x29
VK_SHIFT,       //0x2A
VK_OEM_5,       //0x2B
'Z',            //0x2C
'X',            //0x2D
'C',            //0x2E
'V',            //0x2F
'B',            //0x30
'N',            //0x31
'M',            //0x32
VK_OEM_COMMA,   //0x33
VK_OEM_PERIOD,  //0x34
VK_OEM_2,       //0x35
VK_SHIFT,       //0x36
VK_MULTIPLY,    //0x37
VK_MENU,        //0x38
VK_SPACE,       //0x39
VK_CAPITAL,     //0x3A
VK_F1,          //0x3B
VK_F2,          //0x3C
VK_F3,          //0x3D
VK_F4,          //0x3E
VK_F5,          //0x3F
VK_F6,          //0x40
VK_F7,          //0x41
VK_F8,          //0x42
VK_F9,          //0x43
VK_F10,         //0x44
VK_NUMLOCK,     //0x45
VK_SCROLL,      //0x46
VK_NUMPAD7,     //0x47
VK_NUMPAD8,     //0x48
VK_NUMPAD9,     //0x49
VK_SUBTRACT,    //0x4A
VK_NUMPAD4,     //0x4B
VK_NUMPAD5,     //0x4C
VK_NUMPAD6,     //0x4D
VK_ADD,         //0x4E
VK_NUMPAD1,     //0x4F
VK_NUMPAD2,     //0x50
VK_NUMPAD3,     //0x51
VK_NUMPAD0,     //0x52
VK_DECIMAL,     //0x53
0,              //0x54
0,              //0x55
VK_OEM_102,     //0x56
VK_F11,         //0x57
VK_F12,         //0x58
0,              //0x59
0,              //0x5A
0,              //0x5B
0,              //0x5C
0,              //0x5D
0,              //0x5E
0,              //0x5F
0,              //0x60
0,              //0x61
0,              //0x62
0,              //0x63
VK_F13,         //0x64
VK_F14,         //0x65
VK_F15,         //0x66
0,              //0x67
0,              //0x68
0,              //0x69
0,              //0x6A
0,              //0x6B
0,              //0x6C
0,              //0x6D
0,              //0x6E
0,              //0x6F
VK_KANA,        //0x70
0,              //0x71
0,              //0x72
0,              //0x73
0,              //0x74
0,              //0x75
0,              //0x76
0,              //0x77
0,              //0x78
VK_CONVERT,     //0x79
0,              //0x7A
VK_NONCONVERT,  //0x7B
0,              //0x7C
0,              //0x7D
0,              //0x7E
0,              //0x7F
0,              //0x80
0,              //0x81
0,              //0x82
0,              //0x83
0,              //0x84
0,              //0x85
0,              //0x86
0,              //0x87
0,              //0x88
0,              //0x89
0,              //0x8A
0,              //0x8B
0,              //0x8C
0,              //0x8D
0,              //0x8E
0,              //0x8F
VK_MEDIA_PREV_TRACK,    //0x90
0,              //0x91
0,              //0x92
0,              //0x93
0,              //0x94
0,              //0x95
0,              //0x96
VK_MEDIA_NEXT_TRACK,    //0x97
0,              //0x98
0,              //0x99
0,              //0x9A
0,              //0x9B
0,              //0x9C
VK_CONTROL,     //0x9D
0,              //0x9E
0,              //0x9F
VK_VOLUME_MUTE, //0xA0
0,              //0xA1
VK_MEDIA_PLAY_PAUSE,    //0xA2
0,              //0xA3
VK_MEDIA_STOP,  //0xA4
0,              //0xA5
0,              //0xA6
0,              //0xA7
0,              //0xA8
0,              //0xA9
0,              //0xAA
0,              //0xAB
0,              //0xAC
0,              //0xAD
VK_VOLUME_DOWN, //0xAE
0,              //0xAF
VK_VOLUME_UP,   //0xB0
0,              //0xB1
VK_BROWSER_HOME,    //0xB2
0,              //0xB3
0,              //0xB4
VK_DIVIDE,      //0xB5
0,              //0xB6
VK_PRINT,       //0xB7
VK_RMENU,       //0xB8
0,              //0xB9
0,              //0xBA
0,              //0xBB
0,              //0xBC
0,              //0xBD
0,              //0xBE
0,              //0xBF
0,              //0xC0
0,              //0xC1
0,              //0xC2
0,              //0xC3
0,              //0xC4
VK_PAUSE,       //0xC5
0,              //0xC6
VK_HOME,        //0xC7
VK_UP,          //0xC8
VK_PRIOR,       //0xC9
0,              //0xCA
VK_LEFT,        //0xCB
0,              //0xCC
VK_RIGHT,       //0xCD
0,              //0xCE
VK_END,         //0xCF
VK_DOWN,        //0xD0
VK_NEXT,        //0xD1
VK_INSERT,      //0xD2
VK_DELETE,      //0xD3
0,              //0xD4
0,              //0xD5
0,              //0xD6
0,              //0xD7
0,              //0xD8
0,              //0xD9
0,              //0xDA
VK_LWIN,        //0xDB
VK_RWIN,        //0xDC
VK_APPS,        //0xDD
0,              //0xDE
VK_SLEEP,       //0xDF
0,              //0xE0
0,              //0xE1
0,              //0xE2
0,              //0xE3
0,              //0xE4
VK_BROWSER_SEARCH,      //0xE5
VK_BROWSER_FAVORITES,   //0xE6
VK_BROWSER_REFRESH,     //0xE7
VK_BROWSER_STOP,        //0xE8
VK_BROWSER_FORWARD,     //0xE9
VK_BROWSER_BACK,        //0xEA
0,              //0xEB
VK_LAUNCH_MAIL, //0xEC
VK_LAUNCH_MEDIA_SELECT, //0xED
0,              //0xEE
0,              //0xEF
0,              //0xF0
0,              //0xF1
0,              //0xF2
0,              //0xF3
0,              //0xF4
0,              //0xF5
0,              //0xF6
0,              //0xF7
0,              //0xF8
0,              //0xF9
0,              //0xFA
0,              //0xFB
0,              //0xFC
0,              //0xFD
0,              //0xFE
0,              //0xFF
};

#endif


RTTI_IMPLEMENT_NO_SAVE(EFFInputManager, 0)

EFFInputManager::EFFInputManager()
{
	x = 0;
	y = 0;
	lastX = 0;
	lastY = 0;

	leftButtonDown = effFALSE;
	middleButtonDown = effFALSE;
	rightButtonDown = effFALSE;

	lastLeftButtonDown = effFALSE;
	lastMiddleButtonDown = effFALSE;
	lastRightButtonDown = effFALSE;

	keyState = EFFNEW effBOOL[256];
	memset(keyState, 0, sizeof(effBOOL) * 256);

	lastKeyState = EFFNEW effBOOL[256];
	memset(lastKeyState, 0, sizeof(effBOOL) * 256);
}

EFFInputManager::~EFFInputManager()
{
	SFT_DELETE(keyState);
	SFT_DELETE(lastKeyState);
}

effVOID EFFInputManager::Update()
{
	for ( effINT i = 0; i < 256; i++ )
	{
		if ( keyState[i] && !lastKeyState[i] )
		{
			OnKeyPressed(ConvertEFFVKey2OsVKey(i));
		}

		if ( !keyState[i] && lastKeyState[i] )
		{
			OnKeyReleased(ConvertEFFVKey2OsVKey(i));
		}
	}

	memcpy(lastKeyState, keyState, sizeof(effUINT) * 256);

	if ( x != lastX || y != lastY )
	{
		OnMouseMove(this);
		lastX = x;
		lastY = y;
	}

	if ( (leftButtonDown && !lastLeftButtonDown) ||
		 (middleButtonDown && !lastMiddleButtonDown) ||
		 (rightButtonDown && !lastRightButtonDown) )
	{
		OnMouseButtonPressed(this);
	}

	if ( (!leftButtonDown && lastLeftButtonDown) ||
		 (!middleButtonDown && lastMiddleButtonDown) ||
		 (!rightButtonDown && lastRightButtonDown) )
	{
		OnMouseButtonReleased(this);
	}

	lastLeftButtonDown = leftButtonDown;
	lastMiddleButtonDown = middleButtonDown;
	lastRightButtonDown = rightButtonDown;
}


effUINT	EFFInputManager::ConvertEFFVKey2OsVKey(effUINT effVKey)
{
    if ( effVKey >= 0 && effVKey < 256 )
    {
		return VKMap[effVKey];
	}

    return 0;
}


effUINT	EFFInputManager::ConvertOsVKey2EFFVKey(effUINT osVKey)
{
    for ( effUINT i = 0; i < 256; ++i )
    {
        if ( VKMap[i] == osVKey )
        {
			return i;
		}
    }

    return 0;
}


EFFBASE_END