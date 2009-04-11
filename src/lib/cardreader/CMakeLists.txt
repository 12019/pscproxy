set(CARDREADER_SRCS
	cardreader.cpp
	phoenix.cpp
	cardreader_config.cpp)
include_directories(${PSPROXY_SOURCE_DIR}/src/lib/cardreader)
add_library(psproxy_cardreader SHARED ${CARDREADER_SRCS})
install(TARGETS psproxy_cardreader DESTINATION lib/)
