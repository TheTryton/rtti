cmake_minimum_required(VERSION 3.8)
project(rtti)

option(RTTI_BUILD_BROWSER_EXAMPLE "Generate rtti browser example project")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
##set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

##rtti
message("Configuring rtti project")
add_subdirectory(rtti)

if(RTTI_BUILD_BROWSER_EXAMPLE)
##rtti_browser
	message("Configuring rtti browser example project")
	add_subdirectory(rtti_browser)
else()
	message("RTTI_BUILD_BROWSER_EXAMPLE is disabled, no example project is configured")
endif()
