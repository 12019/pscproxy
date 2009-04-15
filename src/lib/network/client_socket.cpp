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

