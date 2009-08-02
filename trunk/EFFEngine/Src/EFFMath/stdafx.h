// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>


#include <string>
#include <hash_map>
#include <map>
#include <vector>

/******************************************************************************
	created:	2009-3-13   0:19
	file path:	d:\EFF\EFFEngine\Src\EFFMath\stdafx.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __stdafx_2009_3_13__
#define __stdafx_2009_3_13__




//#include <Python.h>
#include <boost/python/detail/wrap_python.hpp>

#include "EFFBase.h"

USE_EFFBASE



#include "EFFMathBaseDefines.h"

// TODO: 在此处引用程序需要的其他头文件

#endif