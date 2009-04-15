#include <exception>

#include <netinet/in.h>
#include <sys/select.h>

#include "debug.h"
#include "server_socket.h"

using namespace PSProxy;

ServerSocket::ServerSocket(Port_t port)
: Socket() {
	pDebug("%s...\n", "Creating instance of ServerSocket");
	init(port);
}

ServerSocket::~ServerSocket() {
	pDebug("%s...\n", "Distroyng instance of ServerSocket");
}

void ServerSocket::init(Port_t port) {
	pDebug("%s...\n", "Initiating server socket");

	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(port);
	if(0 != bind(sockFileDesc, (struct sockaddr *) &server_address, sizeof(server_address))) {
		perror("Error binding the socket");
		throw std::exception(); // TODO: Implement exceptions
	}
	if(0 != listen(sockFileDesc, 1)) {
		perror("Error trying to listen on the socket");
		throw std::exception(); // TODO: Implement exceptions
	}
}

bool ServerSocket::clientWaitingForConnection() {
	struct timeval time_limit;
	fd_set inputs;
	
	time_limit.tv_sec = 0;
	time_limit.tv_usec = 1;
	FD_ZERO(&inputs);
	FD_SET(sockFileDesc, &inputs);

	int rc = select(FD_SETSIZE, &inputs, NULL, NULL, &time_limit);
	if(0 == rc)
		return false;
	else if(-1 == rc) {
		perror("Error while checking socket state");
		throw std::exception(); // TODO: Implement exceptions
	}
	if(FD_ISSET(sockFileDesc, &inputs)) {
		return true;
	}

	return false;
}

