project(EFFEditor)

set(EXE_NAME EFFEditor)


include_directories(${CMAKE_SOURCE_DIR}/Include)
include_directories(${CMAKE_SOURCE_DIR}/Include/${EXE_NAME})
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/boost_1_68_0)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib)
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

link_directories(${CMAKE_SOURCE_DIR}/Temp/Lib)

add_definitions(-DEFFEDITOR_EXPORTS)
add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)

foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Pdb)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Bin)
endforeach (OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)


include(${EXE_NAME}.list)
add_executable(${EXE_NAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES})

#启用最小生成
#target_compile_options(${EXE_NAME} PUBLIC "/Gm")

#使用预编译头
set_target_properties(${EXE_NAME} PROPERTIES COMPILE_FLAGS "/Yc")
set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "/YuEFFEditorPCH.h")
set_source_files_properties(${CMAKE_SOURCE_DIR}/Tools/${EXE_NAME}/EFFEditorPCH.cpp PROPERTIES COMPILE_FLAGS "/YcEFFEditorPCH.h")


#设置生产dll的Debug后缀
set_target_properties(${EXE_NAME} PROPERTIES DEBUG_POSTFIX "_d")


target_link_libraries(${EXE_NAME} EASTL.lib)
