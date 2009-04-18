#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "debug.h"
#include "socket.h"

using namespace PSProxy;

Socket::Socket() {
	pDebug("%s...\n", "Creating instance of Socket");
	sockFileDesc = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::~Socket() {
	pDebug("%s...\n", "Distroyng instance of Socket");
	close(sockFileDesc);
}

int Socket::write(PacketData const &data) {
	pDebug("About to write %d bytes of <%s>\n", data.getSize(), data.getDataBuf());
	return ::write(sockFileDesc, data.getDataBuf(), data.getSize());
}

int Socket::read(PacketData &data) {
	pDebug("%s\n", "tick...");
	char *buf = new char[PacketData::maxLen()];
	pDebug("%s\n", "tick...");
	int rc = ::read(sockFileDesc, buf, PacketData::maxLen());
	pDebug("%s\n", "tick...");
	if(0 < rc) {
		pDebug("%s\n", "tick...");
		data.setData(buf, rc);
		pDebug("%s\n", "tick...");
	} else {
		pDebug("%s\n", "tick...");
		data.clear();
		pDebug("%s\n", "tick...");
	}
	delete[] buf;
	pDebug("%s\n", "tick...");

	return rc;
}

