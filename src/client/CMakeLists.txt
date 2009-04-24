include_directories(${PSCPROXY_SOURCE_DIR}/src/lib/cardemulator
	${PSCPROXY_SOURCE_DIR}/src/lib/network)

set(PSCPROXY_CLIENT_SRCS
	main.cpp)

add_executable(pscproxy_client ${PSCPROXY_CLIENT_SRCS})

if(LIBGNUTLS_FOUND)
	message(STATUS "Compiling client with libgnutls support.")
	target_link_libraries(pscproxy_client ${LIBGNUTLS_LIBRARIES})
	include_directories(${LIBGNUTLS_INCLUDE_DIR})
	add_definitions(-DHAVE_GNUTLS)
else(LIBGNUTLS_FOUND)
	message(STATUS "Compiling client without libgnutls")
endif(LIBGNUTLS_FOUND)

target_link_libraries(pscproxy_client pscproxy_cardemulator pscproxy_network)
install(TARGETS pscproxy_client DESTINATION bin)
