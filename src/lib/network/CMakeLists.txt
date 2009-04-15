set(NETWORK_SRCS
	socket.cpp)
add_library(psproxy_network SHARED ${NETWORK_SRCS})
install(TARGETS psproxy_network DESTINATION lib/)
