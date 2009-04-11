include_directories(${PSPROXY_SOURCE_DIR}/src/lib/season)

set(PSPROXY_CLIENT_SRCS
	main.cpp)

add_executable(psproxy_client ${PSPROXY_CLIENT_SRCS})
target_link_libraries(psproxy_client psproxy_season)
install(TARGETS psproxy_client DESTINATION bin)
