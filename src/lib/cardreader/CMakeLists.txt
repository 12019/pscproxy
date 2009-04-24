set(CARDREADER_SRCS
	cardreader.cpp
	phoenix.cpp
	cardreader_config.cpp)
include_directories(${PSCPROXY_SOURCE_DIR}/src/lib/cardreader)
add_library(pscproxy_cardreader SHARED ${CARDREADER_SRCS})
install(TARGETS pscproxy_cardreader DESTINATION lib/)
