cmake_minimum_required(VERSION 3.8)
project(rtti)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

##rtti
add_subdirectory(rtti)
##rtti_browser
add_subdirectory(rtti_browser)