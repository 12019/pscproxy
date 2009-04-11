#include <iostream>

#include "cardreader.h"
#include "debug.h"

using namespace PSProxy;

CardReader::CardReader(CardReaderConfig const &initConfig)
: config(initConfig) {
	std::cout << "Creating instance of CardReader..." << std::endl;
}

CardReader::~CardReader() {
	std::cout << "Destroying instance of CardReader..." << std::endl;
}

