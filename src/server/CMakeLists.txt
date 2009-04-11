include_directories(${PSPROXY_SOURCE_DIR}/src/lib/cardreader)

set(PSPROXY_SERVER_SRCS
	main.cpp)

add_executable(psproxy_server ${PSPROXY_SERVER_SRCS})
target_link_libraries(psproxy_server psproxy_cardreader)
install(TARGETS psproxy_server DESTINATION bin)
