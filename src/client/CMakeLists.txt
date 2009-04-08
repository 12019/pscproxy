include_directories(${PCPROXY_SOURCE_DIR}/src/lib/season)

set(PCPROXY_CLIENT_SRCS
	main.cpp)

add_executable(pcproxy_client ${PCPROXY_CLIENT_SRCS})
target_link_libraries(pcproxy_client pcproxy_season)
install(TARGETS pcproxy_client DESTINATION bin)
