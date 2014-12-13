cmake_minimum_required(VERSION 3.1)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $(CMAKE_SOURCE_DIR)/Bin)

add_subdirectory(Src/EFFBase)
add_subdirectory(Src/EFFMath)
add_subdirectory(Src/EFF3D)
add_subdirectory(Src/EFFD3D9)
add_subdirectory(Src/EFFFramework)
add_subdirectory(Tools/EFFMyGUIEditor)
add_subdirectory(Tools/Game)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET EFFBase PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFFMath PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFF3D PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFFD3D9 PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFFFramework PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFFMyGUIEditor PROPERTY FOLDER "Tools")
set_property(TARGET Game PROPERTY FOLDER "Tools")  