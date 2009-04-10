cmake_minimum_required(VERSION 2.6)
project(PCPROXY) 
set(CMAKE_SKIP_RPATH TRUE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG 1)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

configure_file (${CMAKE_SOURCE_DIR}/pcproxy-config.h.cmake ${CMAKE_BINARY_DIR}/pcproxy-config.h)

add_subdirectory(src)

