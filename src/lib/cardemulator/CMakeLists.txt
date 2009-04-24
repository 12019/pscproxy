set(CARDEMULATOR_SRCS
	card_emulator.cpp
	season_config.cpp
	season.cpp)
include_directories(${PSPROXY_SOURCE_DIR}/src/lib/cardemulator)
add_library(psproxy_cardemulator SHARED ${CARDEMULATOR_SRCS})
install(TARGETS psproxy_cardemulator DESTINATION lib/)
