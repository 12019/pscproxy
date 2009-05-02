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

#include "debug.h"
#include "packet_data.h"

using namespace PSCProxy;

PacketData::PacketData()
: size(0), data(NULL) {
}

PacketData::PacketData(std::string const &newData)
: size(0), data(NULL) {
	setData(newData);
}

PacketData::PacketData(const char *initData, unsigned int initSize)
: size(initSize) {
	data = new char[size];
	for(unsigned int i = 0; i < size; i++) {
		data[i] = initData[i];
	}
}

PacketData::PacketData(const PacketData &rhs)
: size(rhs.size) {
	data = new char[size];
	for(unsigned int i = 0; i < size; i++) {
		data[i] = rhs.data[i];
	}
}

PacketData::~PacketData() {
	if(data) {
		delete[] data;
	}
}

void PacketData::clear() {
	if(data) {
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

PacketData PacketData::operator +(PacketData const &rhs) {
	unsigned int newSize = size + rhs.size;
	char newData[newSize];
	unsigned int i;
	for(i = 0; i < size; i++) {
		newData[i] = data[i];
	}
	for(i = 0; i < rhs.size; i++) {
		newData[size + i] = rhs.data[i];
	}

	return PacketData(newData, newSize);
}

PacketData &PacketData::operator =(const PacketData &rhs) {
	if(this == &rhs) {
		return *this;
	}

	if(data) {
		delete[] data;
		data = NULL;
	}
	size = rhs.size;
	data = new char[size];
	for(unsigned int i = 0; i < size; i++) {
		data[i] = rhs.data[i];
	}

	return *this;
}

