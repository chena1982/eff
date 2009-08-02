// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 在此处引用程序需要的其他头文件
#pragma warning (disable : 4996)

#include "wx/wxprec.h"


#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <string>
#include <hash_map>
#include <map>
#include <vector>
#include <cassert>
#include <cstdio>
#include <iostream>

//#include <Python.h>
#include <boost/python/detail/wrap_python.hpp>

#include "EFFBase.h"
USE_EFFBASE
#include "EFFMath.h"
#include "EFF3D.h"
USE_EFF3D

#include <wx/grid.h>
#include <wx/aui/aui.h>
#include <wx/aui/dockart.h>
#include <wx/spinctrl.h>
#include <wx/artprov.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>
