set(SEASON_SRCS
	season_config.cpp
	season.cpp)
include_directories(${PSPROXY_SOURCE_DIR}/src/lib/season)
add_library(psproxy_season SHARED ${SEASON_SRCS})
install(TARGETS psproxy_season DESTINATION lib/)
