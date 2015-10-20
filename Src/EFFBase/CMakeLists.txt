project(EFFBase)

set(LIB_NAME EFFBase)

include_directories(${CMAKE_SOURCE_DIR}/Include/${LIB_NAME})
#include_directories($ENV{BOOST_ROOT})
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib)

if (CMAKE_GENERATOR STREQUAL "Visual Studio 2010")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2010)
elseif (CMAKE_GENERATOR STREQUAL "Visual Studio 12 2013")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2013)
endif()

add_definitions(-DEFFBASE_EXPORTS)
add_definitions(-DUSE_DL_PREFIX)
add_definitions(-DNO_MALLINFO)
add_definitions(-DYAML_DECLARE_STATIC)
add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)

foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)
    set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Pdb)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)
endforeach (OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)



include(${LIB_NAME}.list)
add_library(${LIB_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

#启用最小生成
target_compile_options(${LIB_NAME} PUBLIC "/Gm")

#使用预编译头
set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "/Yc")
set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "/YuEFFBasePCH.h")
set_source_files_properties(${CMAKE_SOURCE_DIR}/Src/${LIB_NAME}/EFFBasePCH.cpp PROPERTIES COMPILE_FLAGS "/YcEFFBasePCH.h")

#设置生产dll的Debug后缀
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")


target_link_libraries(${LIB_NAME} lib_json.lib libzmq_d.lib yaml.lib Dbghelp.lib)