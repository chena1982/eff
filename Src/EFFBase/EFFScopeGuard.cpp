/******************************************************************************
	created:	2013-5-1   16:53
	file path:	d:\EFF\Src\EFFBase\EFFScopeGuard.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFScopeGuard.h"

EFFBASE_BEGIN

EFFScopeGuard::EFFScopeGuard(std::function<effVOID ()> onExitScope_)
{
	onExitScope = onExitScope_;
	dismissed = effFALSE;
}

EFFScopeGuard::~EFFScopeGuard()
{
    if ( !dismissed )
    {
        onExitScope();
    }
}

effVOID EFFScopeGuard::Dismiss()
{
    dismissed = effTRUE;
}


EFFBASE_END