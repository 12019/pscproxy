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

#include <cstdlib>
#include <unistd.h>

#include "debug.h"
#include "dummy_cardreader.h"

using namespace PSCProxy;

DummyCardReader::DummyCardReader()
: CardReader(CardReaderConfig("")) {
	pDebug("%s\n", "Creating instance of DummyCardReader");
	
	srand(time(NULL));
	resetDelay = 1e6;
	dataTXDelay = commandDelay = 2e5;
	timeoutDelay = 1e6;
}

DummyCardReader::~DummyCardReader() {
	pDebug("%s\n", "Destroing instance of DummyCardReader");
}

void DummyCardReader::read(Data_t &result) {
	result.clear();

	for(unsigned int i = 0; i < readData.size(); i++) {
		result.push_back(readData[i]);
	}

	readData.clear();
	usleep(commandDelay);
}

void DummyCardReader::write(Data_t const &data) {
	for(unsigned int i = 0; i < data.size(); i++) {
		usleep(dataTXDelay);
	}

	// Card normally has to reply
	prepareReadData();
}

Data_t const &DummyCardReader::reset() {
	atr.clear();

	atr.push_back(0x3B);
	atr.push_back(0x24);
	atr.push_back(0x00);
	atr.push_back(0x30);
	atr.push_back(0x42);
	atr.push_back(0x30);
	atr.push_back(0x30);
	usleep(resetDelay);
	usleep(commandDelay);

	return atr;
}

int DummyCardReader::rand(int min, int max) {
	if(max <= min)
		return -1;

	float range = max - min + 1;
	int i = (int) (range * ::rand() / (RAND_MAX+1.0));
	i += min;

	return i;
}

void DummyCardReader::prepareReadData() {
	// Emulate new data
	int len = rand(2, 20);
	readData.clear();
	for(int i = 0; i < len; i++) {
		readData.push_back(rand(0, 255));
	}
}
