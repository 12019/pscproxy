include_directories(${PSCPROXY_SOURCE_DIR}/src/lib/cardreader
	${PSCPROXY_SOURCE_DIR}/src/lib/cardemulator)

set(NETWORK_SRCS
	client_socket.cpp
	packet_data.cpp
	proxy_client.cpp
	proxy_server.cpp
	pscproxy_client.cpp
	pscproxy_protocol.cpp
	pscproxy_server.cpp
	server_socket.cpp
	socket.cpp
	)

add_library(pscproxy_network SHARED ${NETWORK_SRCS})
install(TARGETS pscproxy_network DESTINATION lib/)
