include_directories(${PSPROXY_SOURCE_DIR}/src/lib/cardreader
	${PSPROXY_SOURCE_DIR}/src/lib/network)

set(PSPROXY_SERVER_SRCS
	main.cpp)

add_executable(psproxy_server ${PSPROXY_SERVER_SRCS})
target_link_libraries(psproxy_server psproxy_cardreader psproxy_network)
install(TARGETS psproxy_server DESTINATION bin)
