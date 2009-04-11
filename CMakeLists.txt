cmake_minimum_required(VERSION 2.6)
project(PSPROXY) 
set(CMAKE_SKIP_RPATH TRUE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG 1)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

configure_file (${CMAKE_SOURCE_DIR}/psproxy-config.h.cmake ${CMAKE_BINARY_DIR}/psproxy-config.h)

add_subdirectory(src)

