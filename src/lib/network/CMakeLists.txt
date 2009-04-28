include_directories(${PSCPROXY_SOURCE_DIR}/src/lib/cardreader
	${PSCPROXY_SOURCE_DIR}/src/lib/cardemulator)

set(NETWORK_SRCS
	packet_data.cpp
	pscproxy_protocol.cpp
	socket.cpp
	)

add_library(pscproxy_network SHARED ${NETWORK_SRCS})
install(TARGETS pscproxy_network DESTINATION lib/)
