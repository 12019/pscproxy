set(CARDEMULATOR_SRCS
	card_emulator.cpp
	card_emulator_config.cpp
	dummy_card_emulator.cpp
	season.cpp)
include_directories(${PSCPROXY_SOURCE_DIR}/src/lib/cardemulator)
add_library(pscproxy_cardemulator SHARED ${CARDEMULATOR_SRCS})
install(TARGETS pscproxy_cardemulator DESTINATION lib/)
