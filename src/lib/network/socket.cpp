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

#include <exception>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "debug.h"
#include "socket.h"

using namespace PSCProxy;

Socket::Socket() {
	pDebug("%s...\n", "Creating instance of Socket");
	sockFileDesc = ::socket(AF_INET, SOCK_STREAM, 0);
}

Socket::~Socket() {
	pDebug("%s...\n", "Distroyng instance of Socket");
	if(0 != close(sockFileDesc)) {
		perror("Error while closing client socket");
	}
}

int Socket::write(int fileDescriptor, PacketData const &data) {
	unsigned int written = 0;
	unsigned int s = data.getSize();
	int rc;
	while(written < s) {
		pDebug("%s\n", "Writing...");
		rc = ::write(fileDescriptor, data.getDataBuf(), data.getSize());
		pDebug("%s\n", "Done.");
		if(rc < 0) {
			throw std::exception(); // TODO: Implement exceptions
		}
		written += rc;
		pDebug("Written %u. s=%u, so still %u bytes to go...\n", written, s, s - written);
	}
	pDebug("data.getDataBuf()[0]=%x, data.getDataBuf()[1]=%x, data.getDataBuf()[2]=%x\n",
			data.getDataBuf()[0], data.getDataBuf()[1], data.getDataBuf()[2]);

	return written;
}

int Socket::read(int fileDescriptor, PacketData &data) {
	//char *buf = new char[PacketData::maxLen()];
	char buf[PacketData::maxLen()];
	int rc = ::read(fileDescriptor, buf, PacketData::maxLen());
	if(0 < rc) {
		data.setData(buf, rc);
	} else {
		data.clear();
	}
	//delete[] buf;

	return rc;
}

int Socket::read(int fileDescriptor, PacketData &data, unsigned int size) {
	unsigned int r = 0;
	int rc;
	char buf[size];
	PacketData tmpData;
	time_t startTime = time(NULL);
	while(r < size) {
		if(time(NULL) - startTime >= 10) {
			data.clear();
			pDebug("%s\n", "Timeout!! Returning -1");
			return -1;
		}
		rc = ::read(fileDescriptor, buf + r, size - r);
		if(rc < 0) {
			data.clear();
			pDebug("%s\n", "Read error!! Returning -1");
			//throw std::exception(); // TODO: Implement exceptions
			return -1;
		}
		r += rc;
		pDebug("Got %u. size=%u, so still %u bytes to go... rc=%d\n", r, size, size - r, rc);
		usleep(10e3); // 1 / 1000[sec]
	}

	data.setData(buf, size);
	pDebug("data.getSize()=%u, data.getDataBuf()[0]=%x, data.getDataBuf()[1]=%x, data.getDataBuf()[2]=%x\n",
			data.getSize(), data.getDataBuf()[0], data.getDataBuf()[1], data.getDataBuf()[1]);

	return r;
}

bool Socket::newDataInSocket(int socket) {
	fd_set descriptors;
	struct timeval timev;

	FD_ZERO(&descriptors);
	FD_SET(socket, &descriptors);
	timev.tv_sec = 0;
	timev.tv_usec = 1;

	int rc = select(socket + 2, &descriptors, NULL, NULL, &timev);
	if(-1 == rc) {
		perror("Error checking client socket state");
	}

	return (0 == rc? false : true);
}
