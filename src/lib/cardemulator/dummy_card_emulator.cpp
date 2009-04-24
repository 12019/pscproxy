#include <cstdlib>
#include <unistd.h>

#include "debug.h"
#include "dummy_card_emulator.h"

using namespace PSCProxy;

DummyCardEmulator::DummyCardEmulator() {
	pDebug("%s\n", "Creating instance of DummyCardReader");
	lastDataAvailTime = time(NULL);
	srand(lastDataAvailTime);

	resetDelay = 1e6;
	dataTXDelay = commandDelay = 2e5;
	timeoutDelay = 1e6;
}

DummyCardEmulator::~DummyCardEmulator() {
	pDebug("%s\n", "Destroing instance of DummyCardReader");
}

bool DummyCardEmulator::readDataAvail() {
	if(readData.size() > 0) {
		return true;
	}

	return false;
}

void DummyCardEmulator::read(Data_t &result) {
	result.clear();

	for(unsigned int i = 0; i < readData.size(); i++) {
		result.push_back(readData[i]);
	}

	readData.clear();
	usleep(commandDelay);
}

void DummyCardEmulator::write(Data_t const &data) {
	// TODO: Implement it!!
	for(unsigned int i = 0; i < data.size(); i++) {
		usleep(dataTXDelay);
	}

	prepareReadData(true);

	return;
}

bool DummyCardEmulator::resetRequested() {
	// TODO: Implement it!!
	return false;
}

long unsigned DummyCardEmulator::tick() {
	// TODO: Implement it!!
	long unsigned ret = 0;

	prepareReadData();
	if(readDataAvail())
		ret |= ReadDataAvail;

	return ret;
}

int DummyCardEmulator::rand(int min, int max) {
	if(max <= min)
		return -1;

	float range = max - min + 1;
	int i = (int) (range * ::rand() / (RAND_MAX+1.0));
	i += min;

	return i;
}

void DummyCardEmulator::prepareReadData(bool force) {
	time_t currTime = time(NULL);
	if( ((currTime - lastDataAvailTime) >= 5) || force ) {
		// Emulate new data
		int len = rand(0, 512);
		readData.clear();
		for(int i = 0; i < len; i++) {
			readData.push_back(rand(0, 255));
		}

		lastDataAvailTime = currTime;
	}
}

