// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
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
