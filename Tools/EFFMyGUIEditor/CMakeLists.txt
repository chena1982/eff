project(EFFMyGUIEditor)

set(EXE_NAME EFFMyGUIEditor)


include_directories(${CMAKE_SOURCE_DIR}/Include)
include_directories(${CMAKE_SOURCE_DIR}/Include/${EXE_NAME})
include_directories($ENV{DXSDK_DIR}Include)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/mygui/include)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/mygui/Platforms/EFF/include)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/mygui/Common)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/mygui/Common/Base/EFF)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/mygui/Common/Input/Win32)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/mygui/Tools/EditorFramework)  

if (CMAKE_GENERATOR STREQUAL "Visual Studio 2010")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2010)
elseif (CMAKE_GENERATOR STREQUAL "Visual Studio 12 2013")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2013)
endif()

link_directories($ENV{DXSDK_DIR}Lib/x86)
link_directories(${CMAKE_SOURCE_DIR}/Temp/Lib)

add_definitions(-DEFFMYGUIEDITOR_EXPORTS)
add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)
add_definitions(-DMYGUI_DIRECTX_PLATFORM)

foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Pdb)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Bin)
endforeach (OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)


include(${EXE_NAME}.list)
add_executable(${EXE_NAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES})

#启用最小生成
target_compile_options(${EXE_NAME} PUBLIC "/Gm")

#使用预编译头
set_target_properties(${EXE_NAME} PROPERTIES COMPILE_FLAGS "/Yc")
set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "/YuEFFMyGUIEditorPCH.h")
set_source_files_properties(${CMAKE_SOURCE_DIR}/Tools/${EXE_NAME}/EFFMyGUIEditorPCH.cpp PROPERTIES COMPILE_FLAGS "/YcEFFMyGUIEditorPCH.h")


#设置生产dll的Debug后缀
set_target_properties(${EXE_NAME} PROPERTIES DEBUG_POSTFIX "_d")


target_link_libraries(${EXE_NAME} Common_d.lib)
target_link_libraries(${EXE_NAME} EditorFramework_d.lib)
target_link_libraries(${EXE_NAME} MyGUIEngine_d.lib)
target_link_libraries(${EXE_NAME} MyGUI.EFFPlatform_d.lib)
target_link_libraries(${EXE_NAME} d3d9.lib)
target_link_libraries(${EXE_NAME} d3dx9.lib)