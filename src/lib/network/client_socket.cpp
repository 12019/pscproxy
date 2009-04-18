#include <exception>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>


#include "debug.h"
#include "client_socket.h"

using namespace PSProxy;

ClientSocket::ClientSocket(std::string host, Port_t port) {
	pDebug("%s...\n", "Creating instance of ClientSocket");
	init(host, port);
}

int ClientSocket::write(PacketData const &data) {
	pDebug("About to write %d bytes of <%s>\n", data.getSize(), data.getDataBuf());
	return ::write(sockFileDesc, data.getDataBuf(), data.getSize());
}

int ClientSocket::read(PacketData &data) {
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

ClientSocket::~ClientSocket() {
	pDebug("%s...\n", "Distroyng instance of ClientSocket");
}

void ClientSocket::init(std::string host, Port_t port) {
	pDebug("%s\n", "Initiating socket...");
	struct sockaddr_in address;
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(host.c_str());
	address.sin_port = htons(port);

	int rc = connect(sockFileDesc, (struct sockaddr *) &address, sizeof(address));
	if(-1 == rc) {
		perror("Error initiating client socket");
		throw std::exception(); // TODO: Implement exceptions
	}
}

