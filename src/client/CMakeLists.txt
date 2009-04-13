include_directories(${PSPROXY_SOURCE_DIR}/src/lib/season)

set(PSPROXY_CLIENT_SRCS
	main.cpp)

add_executable(psproxy_client ${PSPROXY_CLIENT_SRCS})

if(LIBGNUTLS_FOUND)
	message(STATUS "Compiling client with libgnutls support.")
	target_link_libraries(psproxy_client ${LIBGNUTLS_LIBRARIES})
	include_directories(${LIBGNUTLS_INCLUDE_DIR})
	add_definitions(-DHAVE_GNUTLS)
else(LIBGNUTLS_FOUND)
	message(STATUS "Compiling client without libgnutls")
endif(LIBGNUTLS_FOUND)

target_link_libraries(psproxy_client psproxy_season)
install(TARGETS psproxy_client DESTINATION bin)
