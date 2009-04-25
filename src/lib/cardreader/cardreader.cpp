/*
 * Copyright © 2009 Patryk Cisek
 *
 * This file is part of PSCProxy.
 *
 * PSCProxy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PSCProxy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PSCProxy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "cardreader.h"
#include "debug.h"

using namespace PSCProxy;

CardReader::CardReader(CardReaderConfig const &initConfig)
: config(initConfig) {
	std::cout << "Creating instance of CardReader..." << std::endl;
}

CardReader::~CardReader() {
	std::cout << "Destroying instance of CardReader..." << std::endl;
}

