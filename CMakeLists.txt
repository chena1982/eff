cmake_minimum_required(VERSION 3.1)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $(CMAKE_SOURCE_DIR)/Bin)

add_subdirectory(Src/EFFBase)
add_subdirectory(Src/EFFMath)
add_subdirectory(Tools/EFFMyGUIEditor)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET EFFBase PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFFMath PROPERTY FOLDER "EFFEngine")
set_property(TARGET EFFMyGUIEditor PROPERTY FOLDER "Tools")
  