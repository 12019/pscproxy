set(NETWORK_SRCS
	client_socket.cpp
	packet_data.cpp
	socket.cpp
	server_socket.cpp)

add_library(pscproxy_network SHARED ${NETWORK_SRCS})
install(TARGETS pscproxy_network DESTINATION lib/)
