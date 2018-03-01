project(EFFReflection)

set(LIB_NAME EFFReflection)



# add boost includes
include_directories(${Boost_INCLUDE_DIRS})


include_directories(${CMAKE_SOURCE_DIR}/Include)
include_directories(${CMAKE_SOURCE_DIR}/Src/${LIB_NAME})
include_directories($ENV{BOOST_ROOT})
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/EASTL-3.05.07/include)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/EASTL-3.05.07/test/packages/EABase/include/Common)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/yaml-cpp-0.5.3/include)

#include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Common)
#include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Common/Lib)
#include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Runtime)




set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")




link_directories(${CMAKE_SOURCE_DIR}/Temp/Lib)

if (CMAKE_GENERATOR STREQUAL "Visual Studio 2010")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2010)
elseif (CMAKE_GENERATOR STREQUAL "Visual Studio 12 2013")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2013)
elseif (CMAKE_GENERATOR STREQUAL "Visual Studio 15 2017 Win64")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2017/${CMAKE_CFG_INTDIR}/x64)    
endif()

add_definitions(-DMODULE_EXPORTS)
add_definitions(-DUSE_DL_PREFIX)
add_definitions(-DNO_MALLINFO)
add_definitions(-DYAML_DECLARE_STATIC)
add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)
add_definitions(-DEFFGUI_NOT_IMPORTS)


foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)
    set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Pdb)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)
endforeach (OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

include(${LIB_NAME}.list)
add_library(${LIB_NAME} SHARED ${META_GENERATED_FILES} ${HEADER_FILES} ${SOURCE_FILES})

#启用最小生成
target_compile_options(${LIB_NAME} PUBLIC "/Gm")

#使用预编译头
set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "/Yc")
set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "/YuEFFReflection.h")
set_source_files_properties(${CMAKE_SOURCE_DIR}/Src/${LIB_NAME}/EFFReflectionPCH.cpp PROPERTIES COMPILE_FLAGS "/YcEFFReflectionPCH.h")

#设置生产dll的Debug后缀
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")


target_link_libraries(${LIB_NAME} EASTL.lib libyaml-cppmdd.lib)

