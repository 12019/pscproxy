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

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#include "debug.h"

void init(int &fileDescriptor) {
	struct termios ios;
	int i = 0;
	
	if(0 > (fileDescriptor = open("/dev/ttyS0", O_RDWR))) {
		perror("Season: ");
		return;
	}
	if((i = tcgetattr(fileDescriptor, &ios))) {
		perror("Season: ");
		return;
	}

	cfsetispeed(&ios, B9600);
	cfsetospeed(&ios, B9600);
	ios.c_cflag |= (CLOCAL | CREAD);
	ios.c_cflag &= ~PARENB;
	ios.c_cflag &= ~CSTOPB;
	ios.c_cflag &= ~CSIZE;
	ios.c_cflag |= CS8;
	ios.c_cflag &= ~CRTSCTS;
	cfmakeraw(&ios);

	if((i = tcsetattr(fileDescriptor,TCSANOW,&ios))) {
		perror("Season: ");
	}
}

int main(int argc, char *argv[]) {
	int serialFileDesc;

	init(serialFileDesc);

	timeval tv, tvRef;

	unsigned long int prev, curr;

	char buf = 0;
	int a = 0;
	struct pollfd fd;

	fd.fd = serialFileDesc;
	fd.events = POLLIN;
	gettimeofday(&tvRef, NULL);
	prev = tvRef.tv_sec * 1e6 + tvRef.tv_usec;
	//for(int i = 0; i < maxBufferLen; i++) {
	while(1) {
		//pDebug("a=%d\n", a);
		if(1 > (a = poll(&fd, 1, -1))) {
			pDebug("END!! a=%d\n", a);
			return 1;
		}

		if(::read(serialFileDesc, &buf, sizeof(buf)) <= 0) {
			return 1;
		}
		gettimeofday(&tv, NULL);
		curr = tv.tv_sec * 1e6 + tv.tv_usec;
#if 1
		std::cout << std::dec << tv.tv_sec  - tvRef.tv_sec << "." << tv.tv_usec << 
			" ( " << curr - prev << " ): " <<
			std::hex << (short unsigned int)buf << " " << std::endl;
#else
		if((curr - prev) > 100) {
			std::cout << std::endl;
		} else {
			std::cout << " ";
		}
		std::cout << std::hex << (short unsigned)buf << std::flush;

#endif

		prev = curr;
	}
	pDebug("Normal end: a=%d\n", a);

	return 0;
}

