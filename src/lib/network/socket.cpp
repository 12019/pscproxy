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

#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "debug.h"
#include "socket.h"

using namespace PSCProxy;

Socket::Socket() {
	sockFileDesc = ::socket(AF_INET, SOCK_STREAM, 0);
}

Socket::~Socket() {
	if(0 != close(sockFileDesc)) {
		perror("Error while closing client socket");
	}
}

int Socket::write(int fileDescriptor, PacketData const &data) {
	unsigned int written = 0;
	unsigned int s = data.getSize();
	int rc;
	while(written < s) {
		rc = ::write(fileDescriptor, data.getDataBuf(), data.getSize());
		if(rc < 0) {
			throw std::exception(); // TODO: Implement exceptions
		}
		written += rc;
	}

	return written;
}

int Socket::read(int fileDescriptor, PacketData &data) {
	char buf[PacketData::maxLen()];
	int rc = ::read(fileDescriptor, buf, PacketData::maxLen());
	if(0 < rc) {
		data.setData(buf, rc);
	} else {
		data.clear();
	}

	return rc;
}

int Socket::read(int fileDescriptor, PacketData &data, unsigned int size) {
	unsigned int r = 0;
	int rc;
	char buf[size];
	PacketData tmpData;
	time_t startTime = time(NULL);
	data.clear();
	while(r < size) {
		if(time(NULL) - startTime >= 10) {
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
		if(0 == rc) {
			// If read() returned no data, we better check if the socket is still valid
			if(!valid(fileDescriptor)) {
				return -1;
			}
		}

		usleep(10e3); // 1 / 1000[sec]
	}

	data.setData(buf, size);

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

bool Socket::valid(int socket) {
	pollfd pfd;
	pfd.fd = socket;
	pfd.events = POLLRDHUP;
	int rc = poll(&pfd, 1, 1);
	if(0 > rc) {
		perror("Error while validating socket");
		return false;
	}

	if(0 == rc) {
		pDebug("%s\n", "poll() timed out. Returning: socket invalid");
		return false;
	}

#ifdef DEBUG
	// Compile-out for release...
	if(POLLRDHUP == (pfd.revents & POLLRDHUP)) {
		pDebug("%s\n", "Remote peer closed connection. Socket invalid!");
	}
#endif // DEBUG

	return (0 == pfd.revents)? true : false;
}

