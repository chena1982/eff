project(EFFRender)

set(EXE_NAME EFFRender)



include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Common)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Common/Lib)

include_directories(${CMAKE_SOURCE_DIR}/Include)
include_directories(${CMAKE_SOURCE_DIR}/Include/${EXE_NAME})
include_directories($ENV{BOOST_ROOT})
include_directories($ENV{DXSDK_DIR}Include)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/EASTL-3.05.07/include)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/EASTL-3.05.07/test/packages/EABase/include/Common)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Runtime)
include_directories(${CMAKE_SOURCE_DIR}/Src/EFFReflection)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/yaml-cpp-0.5.3/include)


if (CMAKE_GENERATOR STREQUAL "Visual Studio 15 2017 Win64")
    link_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Lib/VS2017/${CMAKE_CFG_INTDIR}/x64)    
endif()

link_directories($ENV{DXSDK_DIR}Lib/x64)
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

target_link_libraries(${EXE_NAME} EASTL.lib MetaRuntime.lib libyaml-cppmdd.lib)

# statically link with Boost & LibClang
target_link_libraries(${EXE_NAME} ${Boost_LIBRARIES} ${LIBCLANG_LIBRARY})

