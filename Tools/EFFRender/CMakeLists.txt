project(EFFRender)

set(EXE_NAME EFFRender)



include_directories(${CMAKE_SOURCE_DIR}/Include)
include_directories(${CMAKE_SOURCE_DIR}/Include/${EXE_NAME})
include_directories($ENV{BOOST_ROOT})
include_directories($ENV{DXSDK_DIR}Include)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib)


link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2010)
link_directories($ENV{DXSDK_DIR}Lib/x86)
link_directories(${CMAKE_SOURCE_DIR}/Temp/Lib)

add_definitions(-DGAME_EXPORTS)
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
target_compile_options(${EXE_NAME} PUBLIC "/Gm")

#使用预编译头
set_target_properties(${EXE_NAME} PROPERTIES COMPILE_FLAGS "/Yc")
set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "/YuEFFRenderPCH.h")
set_source_files_properties(${CMAKE_SOURCE_DIR}/Tools/${EXE_NAME}/EFFRenderPCH.cpp PROPERTIES COMPILE_FLAGS "/YcEFFRenderPCH.h")


#设置生产dll的Debug后缀
set_target_properties(${EXE_NAME} PROPERTIES DEBUG_POSTFIX "_d")