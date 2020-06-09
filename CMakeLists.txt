
project(EFF)
cmake_minimum_required(VERSION 3.1)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $(CMAKE_SOURCE_DIR)/Bin)

add_subdirectory(Src/EFFBase)
add_subdirectory(Src/EFFGUI)
add_subdirectory(Src/EFFMath)
add_subdirectory(Src/EFF3D)
add_subdirectory(Src/EFFD3D9)
add_subdirectory(Src/EFFD3D12)
add_subdirectory(Src/EFFFramework)
#add_subdirectory(Src/EFFReflection)

#add_subdirectory(Tools/EFFMyGUIEditor)
add_subdirectory(Tools/EFFEditor)
add_subdirectory(Tools/EFFRender)
#add_subdirectory(Tools/EFFShaderCompiler)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET EFFBase PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFFGUI PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFFMath PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFF3D PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFFD3D9 PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFFD3D12 PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFFFramework PROPERTY FOLDER "EFFEngine")
#set_property(TARGET EFFReflection PROPERTY FOLDER "EFFEngine")

#set_property(TARGET EFFMyGUIEditor PROPERTY FOLDER "Tools")
set_property(TARGET EFFEditor PROPERTY FOLDER "Tools")
set_property(TARGET EFFRender PROPERTY FOLDER "Tools")  
#set_property(TARGET EFFShaderCompiler PROPERTY FOLDER "Tools")  