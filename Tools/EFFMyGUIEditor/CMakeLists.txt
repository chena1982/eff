project(EFFMyGUIEditor)

set(LIB_NAME EFFMyGUIEditor)


include_directories(${CMAKE_SOURCE_DIR}/Include)
include_directories(${CMAKE_SOURCE_DIR}/Include/${LIB_NAME})
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
    #set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Pdb)
    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Bin)
    
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)
    set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Pdb)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Temp/Lib)    
endforeach (OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)


include(${LIB_NAME}.list)
#add_executable(${LIB_NAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES})
add_library(${LIB_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

#启用最小生成
target_compile_options(${LIB_NAME} PUBLIC "/Gm")

#使用预编译头
set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "/Yc")
set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "/YuEFFMyGUIEditorPCH.h")
set_source_files_properties(${CMAKE_SOURCE_DIR}/Tools/${LIB_NAME}/EFFMyGUIEditorPCH.cpp PROPERTIES COMPILE_FLAGS "/YcEFFMyGUIEditorPCH.h")


#设置生产dll的Debug后缀
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")


target_link_libraries(${LIB_NAME} Common_d.lib)
target_link_libraries(${LIB_NAME} EditorFramework_d.lib)
target_link_libraries(${LIB_NAME} MyGUIEngine_d.lib)
target_link_libraries(${LIB_NAME} MyGUI.EFFPlatform_d.lib)
target_link_libraries(${LIB_NAME} d3d9.lib)
target_link_libraries(${LIB_NAME} d3dx9.lib)