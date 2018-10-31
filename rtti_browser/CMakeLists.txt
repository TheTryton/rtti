cmake_minimum_required(VERSION 3.8)
project(rtti_browser)

find_package(Qt5 COMPONENTS Widgets Gui Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(rtti_browser_Sources
	main.cpp

    include/rtti_browser.hpp
    include/rtti_browser.cpp

    include/model/rtti_model.hpp
    include/model/rtti_model.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${rtti_browser_Sources})

add_executable(rtti_browser ${rtti_browser_Sources})

target_link_libraries(rtti_browser Qt5::Widgets Qt5::Gui Qt5::Core rtti)