set(CARDREADER_SRCS
	cardreader.cpp
	phoenix.cpp
	cardreader_config.cpp)
include_directories(${PCPROXY_SOURCE_DIR}/src/lib/cardreader)
add_library(pcproxy_cardreader SHARED ${CARDREADER_SRCS})
install(TARGETS pcproxy_cardreader DESTINATION lib/)
