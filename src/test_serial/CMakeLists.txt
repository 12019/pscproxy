include_directories(${PSCPROXY_SOURCE_DIR}/src/lib/cardreader
	${PSCPROXY_SOURCE_DIR}/src/lib/cardemulator
	${PSCPROXY_SOURCE_DIR}/src/lib/network)

set(PSCPROXY_TEST_SERIAL_SRCS
	main.cpp
	)

add_executable(pscproxy_test_serial ${PSCPROXY_TEST_SERIAL_SRCS})
target_link_libraries(pscproxy_test_serial pscproxy_cardreader pscproxy_cardemulator pscproxy_network)
install(TARGETS pscproxy_test_serial DESTINATION bin)
