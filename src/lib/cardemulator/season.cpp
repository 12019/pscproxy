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

#include "season.h"
#include "debug.h"

using namespace PSCProxy;

Season::Season(CardEmulatorConfig const &initConfig)
: config(initConfig), maxBufferLen(512), rstRequested(false), prevCARLineUp(true) {
	pDebug("%s\n", "Creating Season instance...");
	resetDelay = 10000;
	dataTXDelay = 100;
	commandDelay = 200;
	timeoutDelay = 350000;

	init();
}

Season::~Season() {
	pDebug("%s\n", "Destroying Season instance...");
	close(fileDescriptor);
}

bool Season::readDataAvail() {
	fd_set rfds;
	struct timeval tv;
	int retval;

	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&rfds);
	FD_SET(fileDescriptor, &rfds);

	/* Wait up to five seconds. */
	tv.tv_sec = 0;
	tv.tv_usec = 1;

	retval = select(fileDescriptor + 2, &rfds, NULL, NULL, &tv);
	/* Don't rely on the value of tv now! */

	if (retval == -1)
		perror("select()");
	else if (retval) {
		printf("Data is available now.\n");
		Data_t a;
		for(unsigned i = 0; i < a.size(); i++) {
			pDebug("%02X\n", a[i]);
		}
		return true;
	/* FD_ISSET(0, &rfds) will be true. */
	}

	return false;
}

#if 0
bool Season::getLines2() {
	int stat;
	int le, dtr, rts, st, sr, cts, car, cd, rng, ri, dsr;
	ioctl(fileDescriptor, TIOCMGET, &stat);
	le  = (stat & TIOCM_LE);//? 1 : 0;
	dtr = (stat & TIOCM_DTR);//? 1 : 0;
	rts = (stat & TIOCM_RTS);//? 1 : 0;
	st  = (stat & TIOCM_ST);//? 1 : 0;
	sr  = (stat & TIOCM_SR);//? 1 : 0;
	cts = (stat & TIOCM_CTS);//? 1 : 0;
	car = (stat & TIOCM_CAR);//? 1 : 0;
	cd  = (stat & TIOCM_CD);//? 1 : 0;
	rng = (stat & TIOCM_RNG);//? 1 : 0;
	ri  = (stat & TIOCM_RI);//? 1 : 0;
	dsr = (stat & TIOCM_DSR);//? 1 : 0;
	if(216486 != stat)
		pDebug("LE=%d, DTR=%d, RTS=%d, ST=%d, SR=%d, CTS=%d, CAR=%d, CD=%d, RNG=%d, RI=%d, DSR=%d, stat=%d\n",
			le,    dtr,    rts,    st,    sr,    cts,    car,    cd,    rng,    ri,    dsr,    stat);
	return (car? true : false);
}
#endif

void Season::read(Data_t &result) {
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

void Season::write(Data_t const &data) {
	tcflush(fileDescriptor, TCIOFLUSH);
	unsigned size = data.size();
	for(unsigned i = 0; i < size; i++) {
		char ch = data[i];
		pDebug("About to write %02X\n", ch);
		if(-1 == ::write(fileDescriptor, &ch, sizeof(ch))) {
			perror("Season");
			throw std::exception(); // TODO: Impelment exceptions
		}

		tcdrain(fileDescriptor);
		usleep(dataTXDelay);
	}
	Data_t tmpData;
	read(tmpData); // Read echo
}

bool Season::resetRequested() {
	bool ret = rstRequested;
	rstRequested = false;

	return ret;
}

long unsigned Season::tick() {
	bool CARLineUp = isCARLineUp();
	if(CARLineUp && !prevCARLineUp) {
		rstRequested = true;
	}

	prevCARLineUp = CARLineUp;

	return 1;
}

void Season::init() {
	pDebug("Initiating %s serial port\n", config.getDevFilename());
	struct termios ios;
	int i = 0;
	
	if(0 > (fileDescriptor = open(config.getDevFilename(), O_RDWR))) {
		perror("Season: ");
		throw std::exception(); // TODO: Implement exceptions
	}
	if((i = tcgetattr(fileDescriptor, &ios))) {
		perror("Season: ");
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
		perror("Season: ");
		throw std::exception(); // TODO: Implement exceptions
	}
}

bool Season::isCARLineUp() {
	int stat;
	bool CARLineUp;
	ioctl(fileDescriptor, TIOCMGET, &stat);
	CARLineUp = (stat & TIOCM_CAR)? true : false;

	return CARLineUp;
}


