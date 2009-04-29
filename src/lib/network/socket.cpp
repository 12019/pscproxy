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

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "debug.h"
#include "socket.h"

using namespace PSCProxy;

Socket::Socket() {
	pDebug("%s...\n", "Creating instance of Socket");
	sockFileDesc = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::~Socket() {
	pDebug("%s...\n", "Distroyng instance of Socket");
	if(0 != close(sockFileDesc)) {
		perror("Error while closing client socket");
	}
}

int Socket::write(int fileDescriptor, PacketData const &data) {
	pDebug("About to write %d bytes of <%s>\n", data.getSize(), data.getDataBuf());
	return ::write(fileDescriptor, data.getDataBuf(), data.getSize());
}

int Socket::read(int fileDescriptor, PacketData &data) {
	char *buf = new char[PacketData::maxLen()];
	int rc = ::read(fileDescriptor, buf, PacketData::maxLen());
	if(0 < rc) {
		data.setData(buf, rc);
	} else {
		data.clear();
	}
	delete[] buf;

	return rc;
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
