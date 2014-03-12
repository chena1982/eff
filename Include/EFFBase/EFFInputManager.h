/******************************************************************************
	created:	2013-7-16   23:29
	file path:	d:\EFF\Include\EFFBase\EFFInputManager.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFInputManager_2013_7_16__
#define __EFFInputManager_2013_7_16__

#include "EFFEvent.h"
#include "EFFComponent.h"

EFFBASE_BEGIN

enum EFFMouseButtonID
{
	Left_Button,
	Middle_Button,
	Right_Button
};

class EFFBASE_API EFFInputManager : public EFFComponent
{
	RTTI_DECLARE(EFFInputManager, EFFComponent)
public:
	EFFInputManager();
	~EFFInputManager();

public:
	effVOID		SetKeyDown(effINT key) { keyState[key] = effTRUE; }
	effVOID		SetKeyUp(effINT key) { keyState[key] = effFALSE; }
	effVOID		SetAllKeyUp() { memset(keyState, 0, sizeof(effBOOL) * 256); }

	effVOID		SetPos(effINT x, effINT y) { this->x = x; this->y = y; }
	effINT		GetPosX() { return x; }
	effINT		GetPosY() { return y; }

	effVOID		SetLeftButtonDown(effBOOL down) { leftButtonDown = down; }
	effBOOL		IsLeftButtonDown() { return leftButtonDown; }

	effVOID		SetMiddleButtonDown(effBOOL down) { middleButtonDown = down; }
	effBOOL		IsMiddleButtonDown() { return middleButtonDown; }

	effVOID		SetRightButtonDown(effBOOL down) { rightButtonDown = down; }
	effBOOL		IsRightButtonDown() { return rightButtonDown; }

	effVOID		SetAllMouseButtonUp() { leftButtonDown = effFALSE; middleButtonDown = effFALSE; rightButtonDown = effFALSE; }

	effVOID		Update();


	effUINT		ConvertEFFVKey2OsVKey(effUINT effVKey);
	effUINT		ConvertOsVKey2EFFVKey(effUINT osVKey);


public:
	EFFEvent	OnKeyPressed;
	EFFEvent	OnKeyReleased;

	EFFEvent	OnMouseMove;
	EFFEvent	OnMouseButtonPressed;
	EFFEvent	OnMouseButtonReleased;
protected:

	effINT		x;
	effINT		y;
	effINT		lastX;
	effINT		lastY;

	effBOOL		leftButtonDown;
	effBOOL		middleButtonDown;
	effBOOL		rightButtonDown;

	effBOOL		lastLeftButtonDown;
	effBOOL		lastMiddleButtonDown;
	effBOOL		lastRightButtonDown;

	effBOOL *	keyState;
	effBOOL	*	lastKeyState;
};



EFFBASE_END

#endif