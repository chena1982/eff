project(EFFReflection)

set(LIB_NAME EFFReflection)



set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/CMake")




# add boost includes
include_directories(${Boost_INCLUDE_DIRS})

find_package(LLVM REQUIRED)

# add LLVM includes
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIR})



include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Common)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Common/Lib)

include_directories(${CMAKE_SOURCE_DIR}/Include)
include_directories(${CMAKE_SOURCE_DIR}/Src/${LIB_NAME})
include_directories($ENV{BOOST_ROOT})
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/EASTL-3.05.07/include)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/EASTL-3.05.07/test/packages/EABase/include/Common)
include_directories(${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Runtime)


include(MetaParser)

include_directories(${META_RUNTIME_INCLUDE_DIRS})



set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

set(${LIB_NAME}_META_SOURCE_ROOT "${CMAKE_SOURCE_DIR}")
set(${LIB_NAME}_META_GENERATED_DIR "${CMAKE_CURRENT_LIST_DIR}/Generated")

set(${LIB_NAME}_META_GENERATED_HEADERS "")
set(${LIB_NAME}_META_GENERATED_SOURCES "")

set(${LIB_NAME}_REFLECTION_HEADER Src/EFFReflection/${LIB_NAME}Header.h)
set(${LIB_NAME}_MODULE_HEADER Src/EFFReflection/${LIB_NAME}Module.h)


set(${LIB_NAME}_HEADERS ${CMAKE_SOURCE_DIR}/Include/EFFGUI/EFFGUILayoutInfo.h)

set($(BUILD_META_PCH_NAME) EFFReflectionPCH)

meta_parser_prebuild(
    TARGET ${LIB_NAME}
    GENERATED_DIR ${${LIB_NAME}_META_GENERATED_DIR}
    SOURCE_ROOT ${${LIB_NAME}_META_SOURCE_ROOT}
    HEADER_FILES ${${LIB_NAME}_HEADERS}
    MODULE_HEADER ${${LIB_NAME}_MODULE_HEADER}
    OUT_MODULE_SOURCE META_MODULE_SOURCE
    OUT_GENERATED_FILES META_GENERATED_FILES
    OUT_INC ${LIB_NAME}_META_GENERATED_HEADERS
    OUT_SRC ${LIB_NAME}_META_GENERATED_SOURCES
)

message(STATUS "meta module source ${META_MODULE_SOURCE}")
message(STATUS "meta generated files ${META_GENERATED_FILES}")   


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
set_source_files_properties(${SOURCE_FILES} ${META_GENERATED_FILES} PROPERTIES COMPILE_FLAGS "/YuEFFReflectionPCH.h")
set_source_files_properties(${CMAKE_SOURCE_DIR}/Src/${LIB_NAME}/EFFReflectionPCH.cpp PROPERTIES COMPILE_FLAGS "/YcEFFReflectionPCH.h")

#设置生产dll的Debug后缀
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "_d")


target_link_libraries(${LIB_NAME} EASTL.lib libyaml-cppmdd.lib)



# statically link with Boost & LibClang
target_link_libraries(${LIB_NAME} ${Boost_LIBRARIES} ${LIBCLANG_LIBRARY})

target_link_libraries(${LIB_NAME} MetaRuntime)

# copy resources on post build
add_custom_command(TARGET ${LIB_NAME} POST_BUILD
    # mustache templates directory
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/ThirdPartyLib/Cpp-Reflection/Resources"
        ${CMAKE_SOURCE_DIR}/Bin
    # LibClang shared library
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LIBCLANG_SHARED_LIBRARY}
        ${CMAKE_SOURCE_DIR}/Bin
)

meta_parser_build(
    TARGET ${LIB_NAME}
    SOURCE_ROOT ${${LIB_NAME}_META_SOURCE_ROOT}
    SOURCE_FILE ${${LIB_NAME}_REFLECTION_HEADER}    
    GENERATED_DIR ${${LIB_NAME}_META_GENERATED_DIR}
    GENERATED_FILES ${META_GENERATED_FILES}
    MODULE_HEADER ${${LIB_NAME}_MODULE_HEADER}
    MODULE_SOURCE_FILE ${META_MODULE_SOURCE}
    HEADER_FILES ${${LIB_NAME}_HEADERS}
    PCH_NAME ${LIB_NAME}PCH
    PARSER_EXECUTABLE "${CMAKE_SOURCE_DIR}/Bin/MetaParser.exe"
)