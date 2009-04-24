#include "debug.h"
#include "packet_data.h"

using namespace PSCProxy;

PacketData::PacketData()
: size(0), data(NULL) {
	pDebug("%s\n", "Creating instance of PacketData");
}

PacketData::PacketData(std::string const &newData)
: size(0), data(NULL) {
	pDebug("%s\n", "Creating instance of PacketData with initialization");
	setData(newData);
}

PacketData::~PacketData() {
	pDebug("%s\n", "Destroyng  instance of PacketData");
	if(data) {
		pDebug("%s\n", "DataPacket not empty. Deleting it's data");
		delete[] data;
	}
}

void PacketData::clear() {
	pDebug("About to clear...data=%p\n", data);
	if(data) {
		pDebug("Not NULL (%p), deleting\n", data);
		delete[] data;
		data = NULL;
	}

	size = 0;
}

void PacketData::setData(std::string const &newData) {
	clear();
	size = newData.size();
	data = new char[size];
	for(unsigned int i = 0; i < size; i++) {
		data[i] = newData[i];
	}
}

void PacketData::setData(char const *newData, int newSize) {
	clear();
	size = newSize;
	data = new char[size];
	for(unsigned int i = 0; i < size; i++) {
		data[i] = newData[i];
	}
}

