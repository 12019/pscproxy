#include "cardreader_config.h"
#include "debug.h"

using namespace PSCProxy;

CardReaderConfig::CardReaderConfig(std::string const &file) {
	pDebug("%s\n", "Creating instance of CardReaderConfig");

	devFilename = "/dev/ttyS0";
	//devFilename = "/dev/ttyUSB0";
}

CardReaderConfig::~CardReaderConfig() {
}

