#include "card_emulator_config.h"
#include "debug.h"

using namespace PSCProxy;

CardEmulatorConfig::CardEmulatorConfig(std::string const &file) {
	pDebug("%s\n", "Creating instance of CardEmulatorConfig");

	devFilename = "/dev/ttyS0";
	//devFilename = "/dev/ttyUSB0";
}

CardEmulatorConfig::~CardEmulatorConfig() {
}

