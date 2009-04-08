#include <iostream>

#include "cardreader.h"

using namespace PCProxy;

CardReader::CardReader() {
	std::cout << "Creating instance of CardReader..." << std::endl;
}

CardReader::~CardReader() {
	std::cout << "Destroying instance of CardReader..." << std::endl;
}
