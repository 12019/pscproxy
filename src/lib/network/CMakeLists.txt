include_directories(${PSCPROXY_SOURCE_DIR}/src/lib/cardreader)

set(NETWORK_SRCS
	client_socket.cpp
	packet_data.cpp
	proxy_server.cpp
	socket.cpp
	server_socket.cpp)

add_library(pscproxy_network SHARED ${NETWORK_SRCS})
install(TARGETS pscproxy_network DESTINATION lib/)
