set(NETWORK_SRCS
	client_socket.cpp
	packet_data.cpp
	socket.cpp
	server_socket.cpp)

add_library(psproxy_network SHARED ${NETWORK_SRCS})
install(TARGETS psproxy_network DESTINATION lib/)
