// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <versionhelpers.h>

// TODO: 在此处引用程序需要的其他头文件
//#include "malloc.h"
#include "md5.h"

//#include <string>
//#include <hash_map>
//#include <map>
//#include <VECTOR>
//#include <iostream>

//#include <Python.h>
//因为要使用boost.python，所以用下面的这个头文件代替Python.h，不然会链接到python26_d.lib
//如果要使用调试版的python26_d.lib，boost.python也需要使用python26_d.lib，这时需要定义BOOST_DEBUG_PYTHON，否则会链接到使用python26.lib的boost.python库
//#include <boost/python/detail/wrap_python.hpp>



#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/map.h>
#include <EASTL/deque.h>
#include <EASTL/algorithm.h>
#include <EASTL/fixed_allocator.h>

#include <zeromq-4.2.2/include/zmq.h>
#include <jsoncpp-1.8.1/include/json/json.h>
#include <yaml/yaml.h>
#include <yaml-cpp/yaml.h>

#include "EFFBaseDefines.h"
#include "EFFMemory.h"
#include "EFFUtility.h"
#include "EFFRtti.h"
#include "EFFFastIdMap.h"
#include "YAMLWrap.h"






