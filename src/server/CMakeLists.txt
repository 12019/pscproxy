include_directories(${PSCPROXY_SOURCE_DIR}/src/lib/cardreader
	${PSCPROXY_SOURCE_DIR}/src/lib/network)

set(PSCPROXY_SERVER_SRCS
	main.cpp)

add_executable(pscproxy_server ${PSCPROXY_SERVER_SRCS})
target_link_libraries(pscproxy_server pscproxy_cardreader pscproxy_network)
install(TARGETS pscproxy_server DESTINATION bin)
