cmake_minimum_required(VERSION 2.6)
project(PSPROXY) 
set(CMAKE_SKIP_RPATH TRUE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
	${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(GNUTLS)
find_package(Threads REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG 1)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

configure_file (${CMAKE_SOURCE_DIR}/psproxy-config.h.cmake ${CMAKE_BINARY_DIR}/psproxy-config.h)

add_subdirectory(src)

