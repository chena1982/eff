project(EFFBase)

cmake_minimum_required(VERSION 3.13)

set(LIB_NAME EFFBase)

include_directories(${CMAKE_SOURCE_DIR}/Include/${LIB_NAME})
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/boost_1_68_0)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/EASTL-3.12.04/include)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/EASTL-3.12.04/test/packages/EABase/include/Common)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/yaml-cpp-0.5.3/include)

#include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Common)
#include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Common/Lib)
#include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Runtime)

if (CMAKE_GENERATOR STREQUAL "Visual Studio 2010")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2010)
elseif (CMAKE_GENERATOR STREQUAL "Visual Studio 12 2013")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2013)
elseif (CMAKE_GENERATOR STREQUAL "Visual Studio 12 2013 Win64")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2013/${CMAKE_CFG_INTDIR}/x64)
elseif (CMAKE_GENERATOR STREQUAL "Visual Studio 15 2017 Win64")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2017/${CMAKE_CFG_INTDIR}/x64)    
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




foreach (HEADER_FILE ${HEADER_FILES})
    string(FIND ${HEADER_FILE} ../../Include/${LIB_NAME}/ POS)
    if (${POS} EQUAL 0)
        string(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${HEADER_FILE})

        if (EXISTS ${CMAKE_SOURCE_DIR}/Src/${LIB_NAME}/${FILE_NAME}.cpp)
            
            set(GENERATED_HEADER ${CMAKE_SOURCE_DIR}/Include/${LIB_NAME}/${FILE_NAME}.generated.h)

            if (NOT EXISTS GENERATED_HEADER)
                file(TOUCH ${GENERATED_HEADER})
            endif()

            list(APPEND GENERATED_HEADERS ${GENERATED_HEADER})

            add_custom_command(OUTPUT ${GENERATED_HEADER}
                COMMAND ${CMAKE_SOURCE_DIR}/Bin/EFFReflect ARGS ${FILE_NAME}.cpp
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Src/${LIB_NAME}
                DEPENDS ${CMAKE_SOURCE_DIR}/Include/${LIB_NAME}/${FILE_NAME}.h
            )
            
        endif()
    endif()
endforeach(HEADER_FILE)


source_group("GeneratedHeaders" FILES ${GENERATED_HEADERS})
add_library(${LIB_NAME} SHARED ${GENERATED_HEADERS} ${HEADER_FILES} ${SOURCE_FILES})



#启用最小生成
target_compile_options(${LIB_NAME} PUBLIC "/Gm")

#使用预编译头
set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "/Yc")
set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "/YuEFFBasePCH.h")
set_source_files_properties(${CMAKE_SOURCE_DIR}/Src/${LIB_NAME}/EFFBasePCH.cpp PROPERTIES COMPILE_FLAGS "/YcEFFBasePCH.h")

#设置生产dll的Debug后缀
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")


target_link_libraries(${LIB_NAME} EASTL.lib jsoncpp.lib libzmq.lib yaml.lib Dbghelp.lib)





