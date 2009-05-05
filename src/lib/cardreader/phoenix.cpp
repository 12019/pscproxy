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
#include <cstring>
#include <exception>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <poll.h>
#include <errno.h>

#include "phoenix.h"
#include "debug.h"

#define _POSIX_SOURCE 1
using namespace PSCProxy;

Phoenix::Phoenix(CardReaderConfig const &config)
: CardReader(config), maxBufferLen(512) {
	resetDelay = 10000;
	dataTXDelay = 200;
	commandDelay = 200;
	timeoutDelay = 350000;

	init2();
	reset();
}

#if 0
void Phoenix::init() {
	struct termios ios;
	speed_t inSpeed, outSpeed;

	if(0 > (fileDescriptor = open(config.getDevFilename(), O_NOCTTY | O_SYNC | O_RDWR))) {
		perror(config.getDevFilename());
		throw std::exception(); // TODO: Implement exceptions
	}
	
	if(tcgetattr(fileDescriptor, &ios)) {
		perror(config.getDevFilename());
		throw std::exception(); // TODO: Implement exceptions
	}

	inSpeed = cfgetispeed(&ios);
	outSpeed = cfgetospeed(&ios);
	cfmakeraw(&ios);

	/* Setting the speed */
	if (cfsetispeed(&ios, B9600)) {
		perror(config.getDevFilename());
		throw std::exception(); // TODO: Implement exceptions
	}
	if (cfsetospeed(&ios, B9600)) {
		perror(config.getDevFilename());
		throw std::exception(); // TODO: Implement exceptions
	}

	// One parity bit
	ios.c_cflag |= PARENB;

	// One stop bit
	ios.c_cflag &= ~CSTOPB;

	// 8 data bits
	ios.c_cflag &= ~CSIZE;
	ios.c_cflag |= CS8;

	// No modem control lines
	ios.c_cflag |= CLOCAL;

	// Enable receiver
	ios.c_cflag |= CREAD;

	// Turn off hardware flow control
	ios.c_cflag &= ~CRTSCTS;
	if(!tcsetattr(fileDescriptor, TCSANOW, &ios)) {
		struct termios currentIos;

		pDebug("%s\n", "About to read params again...");
		if(tcgetattr(fileDescriptor, &currentIos)) {
			perror(config.getDevFilename());
			throw std::exception(); // TODO: Implement exceptions
		}

		pDebug("%s\n", "About to check if everything's been set...");
		if(int diff = memcmp(&ios, &currentIos, sizeof(ios))) {
			pDebug("The result of comparison was: %d\n", diff);
			//throw std::exception(); // TODO: Implement exceptions
		}
	}

	inSpeed = cfgetispeed(&ios);
	outSpeed = cfgetospeed(&ios);
	pDebug("inSpeed=%d, outSpeed=%d\n", inSpeed, outSpeed);
	pDebug("fileDescriptor=%d\n", fileDescriptor);
}
#endif

void Phoenix::init2() {
	// This version of ttyS* initialization is currently not used. See init()
	struct termios ios;
	int i = 0;
	
	if(0 > (fileDescriptor = open(config.getDevFilename(), O_RDWR | O_NOCTTY | O_SYNC))) {
		perror("CardReader: ");
		throw std::exception(); // TODO: Implement exceptions
	}
	if((i = tcgetattr(fileDescriptor, &ios))) {
		perror("CardReader: ");
		throw std::exception(); // TODO: Implement exceptions
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
		perror("CardReader: ");
		throw std::exception(); // TODO: Implement exceptions
	}
}

Phoenix::~Phoenix() {
	close(fileDescriptor);
}

Data_t const &Phoenix::reset() {
	pDebug("%s\n", "Resseting card...");
	tcflush(fileDescriptor, TCIOFLUSH);
	setctrl(TIOCM_RTS | TIOCM_CTS | TIOCM_DTR);
	usleep(resetDelay);
	setctrl(TIOCM_CTS | TIOCM_DTR);
	usleep(commandDelay);

	read(atr);
	int len = atr.size();
	for(int i = 0; i < len; i++)
		std::cout << " " << std::hex << (int)atr[i];
		//std::cout << " " << std::hex << 0x2e << std::endl;
	std::cout << std::endl;

	return atr;
}

void Phoenix::read(Data_t &result) {
	char buf = 0;
	int a = 0;
	struct pollfd fd;

	timeval tv, tvRef;
	unsigned long int currTime, prevTime;

	fd.fd = fileDescriptor;
	fd.events = POLLIN;
	result.clear();
	gettimeofday(&tvRef, NULL);
	prevTime = tvRef.tv_sec * 1e6 + tvRef.tv_usec;
	for(int i = 0; i < maxBufferLen; i++) {
		// timeoutDelay is in microseconds. We need to convert to miliseconds for poll()
		if(1 > (a = poll(&fd, 1, timeoutDelay / 1000))) {
			return;
		}

		if(::read(fileDescriptor, &buf, sizeof(buf)) <= 0) {
			throw std::exception(); // TODO: Impelment exceptions
		}
		gettimeofday(&tv, NULL);
		currTime = tv.tv_sec * 1e6 + tv.tv_usec;
#if 1
		std::cout << std::dec << tv.tv_sec  - tvRef.tv_sec << "." << tv.tv_usec << 
			" ( " << currTime - prevTime << " ): " <<
			std::hex << (short unsigned int)buf << " " << std::endl;
#else
		if((currTime - prevTime) > 100) {
			std::cout << std::endl;
		} else {
			std::cout << " ";
		}
		std::cout << std::hex << (short unsigned)buf << std::flush;

#endif
		prevTime = currTime;
		result.push_back(buf);
	}

	usleep(commandDelay);
}

void Phoenix::write(Data_t const &data) {
	tcflush(fileDescriptor, TCIOFLUSH);
	unsigned size = data.size();
	for(unsigned i = 0; i < size; i++) {
		char ch = data[i];
		pDebug("About to write %02X\n", ch);
		if(-1 == ::write(fileDescriptor, &ch, sizeof(ch))) {
			perror("Phoenix");
			throw std::exception(); // TODO: Impelment exceptions
		}

		tcdrain(fileDescriptor);
		usleep(dataTXDelay);
	}
}

void Phoenix::setctrl(const int ctrl) {
	int tmpCtrl;
	ioctl(fileDescriptor, TIOCMGET, &tmpCtrl);
	tmpCtrl &= ~(TIOCM_DTR | TIOCM_CTS | TIOCM_RTS);
	tmpCtrl |= ctrl;
	ioctl(fileDescriptor, TIOCMSET, &tmpCtrl);
}

