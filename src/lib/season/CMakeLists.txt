set(SEASON_SRCS
	season.cpp)
include_directories(${PCPROXY_SOURCE_DIR}/src/lib/season)
add_library(pcproxy_season SHARED ${SEASON_SRCS})
install(TARGETS pcproxy_season DESTINATION lib/)
