#include <iostream>
#include <cstring>
#include <exception>
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
using namespace PCProxy;

Phoenix::Phoenix(CardReaderConfig const &config)
: CardReader(config), maxBufferLen(512) {
	resetDelay = 1e6;
	dataTXDelay = commandDelay = 2e5;
	timeoutDelay = 1e6;

	init();
}

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

void Phoenix::init2() {
	// This version of ttyS* initialization is currently not used. See init()
	struct termios oldtio,newtio;
	int i = 0;
	
	if(0 > (fileDescriptor = open(config.getDevFilename(), O_RDWR | O_NOCTTY))) {
		perror("CardReader: ");
		throw std::exception(); // TODO: Implement exceptions
	}
	pDebug("i=%d\n", i);
	if(i = tcgetattr(fileDescriptor, &oldtio)) {
		perror("CardReader: ");
		throw std::exception(); // TODO: Implement exceptions
	}
	pDebug("i=%d\n", i);
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = B38400 | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR | ICRNL;
	newtio.c_oflag = 0;
	newtio.c_lflag = ICANON;
	if(i = tcflush(fileDescriptor, TCIFLUSH)) {
		perror("CardReader: ");
		throw std::exception(); // TODO: Implement exceptions
	}
	pDebug("i=%d\n", i);
	if(i = tcsetattr(fileDescriptor,TCSANOW,&newtio)) {
		perror("CardReader: ");
		throw std::exception(); // TODO: Implement exceptions
	}
	pDebug("i=%d\n", i);
	
	if(i = tcgetattr(fileDescriptor, &oldtio)) {
		perror("CardReader: ");
		throw std::exception(); // TODO: Implement exceptions
	}
}

Phoenix::~Phoenix() {
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

	fd.fd = fileDescriptor;
	fd.events = POLLIN;
	result.clear();
	for(int i = 0; i < maxBufferLen; i++) {
		pDebug("a=%d\n", a);
		if(1 > (a = poll(&fd, 1, timeoutDelay / 1000))) {
			pDebug("END!! a=%d\n", a);
			return;
		}

		if(::read(fileDescriptor, &buf, sizeof(buf)) <= 0) {
			throw std::exception(); // TODO: Impelment exceptions
		}
		result.push_back(buf);
	}
	pDebug("Normal end: a=%d\n", a);

	usleep(commandDelay);
}

void Phoenix::setctrl(const int ctrl) {
	int tmpCtrl;
	ioctl(fileDescriptor, TIOCMGET, &tmpCtrl);
	tmpCtrl &= ~(TIOCM_DTR | TIOCM_CTS | TIOCM_RTS);
	tmpCtrl |= ctrl;
	ioctl(fileDescriptor, TIOCMSET, &tmpCtrl);
}
