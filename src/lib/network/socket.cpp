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

