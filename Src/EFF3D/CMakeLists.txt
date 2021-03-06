project(EFF3D)

set(LIB_NAME EFF3D)


include_directories(${CMAKE_SOURCE_DIR}/Include)
include_directories(${CMAKE_SOURCE_DIR}/Include/${LIB_NAME})
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/boost_1_68_0)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib)
#include_directories("${CMAKE_SOURCE_DIR}/ThirdPartyLib/Awesomium SDK/1.7.4.2/include")
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/freetype-2.8/include)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/EASTL-3.12.04/include)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/EASTL-3.12.04/test/packages/EABase/include/Common)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/yaml-cpp-0.5.3/include)

if (CMAKE_GENERATOR STREQUAL "Visual Studio 2010")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2010)
elseif (CMAKE_GENERATOR STREQUAL "Visual Studio 12 2013")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2013)
elseif (CMAKE_GENERATOR STREQUAL "Visual Studio 15 2017")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2017/${CMAKE_CFG_INTDIR}/x64)    
endif()

link_directories($ENV{DXSDK_DIR}Lib/x64)
link_directories(${CMAKE_SOURCE_DIR}/Temp/Lib)

add_definitions(-DEFF3D_EXPORTS)
add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)

foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)
    set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Pdb)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)
endforeach (OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)



#include(${LIB_NAME}.list)


file(GLOB_RECURSE EFF3D_SRCS
    "*.h"
    "*.cpp"
)


foreach(FILE ${EFF3D_SRCS}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    #message(STATUS ${FILE})

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

add_library(${LIB_NAME} SHARED ${EFF3D_SRCS})

#启用最小生成
#target_compile_options(${LIB_NAME} PUBLIC "/Gm")

#使用预编译头
#set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "/Yc")
#set_source_files_properties(${EFF3D_SRCS} PROPERTIES COMPILE_FLAGS "/YuEFF3DPCH.h")
#set_source_files_properties(${CMAKE_SOURCE_DIR}/Src/${LIB_NAME}/EFF3DPCH.cpp PROPERTIES COMPILE_FLAGS "/YcEFF3DPCH.h")

#设置生产dll的Debug后缀
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")


target_link_libraries(${LIB_NAME} DevIL.lib EASTL.lib freetype28d.lib slang.lib)
#target_link_libraries(${LIB_NAME} awesomium.lib)
#target_link_libraries(${LIB_NAME} wke.lib)
