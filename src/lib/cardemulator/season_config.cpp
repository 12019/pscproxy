#include "season_config.h"
#include "debug.h"

using namespace PSCProxy;

SeasonConfig::SeasonConfig(std::string const &file) {
	pDebug("%s\n", "Creating instance of SeasonConfig");

	devFilename = "/dev/ttyS0";
	//devFilename = "/dev/ttyUSB0";
}

SeasonConfig::~SeasonConfig() {
}

