include_directories(${PCPROXY_SOURCE_DIR}/src/lib/cardreader)

set(PCPROXY_SERVER_SRCS
	main.cpp)

add_executable(pcproxy_server ${PCPROXY_SERVER_SRCS})
target_link_libraries(pcproxy_server pcproxy_cardreader)
install(TARGETS pcproxy_server DESTINATION bin)
