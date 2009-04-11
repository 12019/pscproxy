#include "cardreader_config.h"
#include "debug.h"

using namespace PSProxy;

CardReaderConfig::CardReaderConfig(std::string const &file) {
	pDebug("%s\n", "Creating instance of CardreaderConfig");

	devFilename = "/dev/ttyS0";
}

CardReaderConfig::~CardReaderConfig() {
}

