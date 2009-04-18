#include <exception>

#include <netinet/in.h>
#include <sys/select.h>

#include "debug.h"
#include "server_socket.h"

using namespace PSProxy;

ServerSocket::ServerSocket(Port_t port)
: Socket(), clientSockFileDesc(-1) {
	pDebug("%s...\n", "Creating instance of ServerSocket");
	init(port);
}

ServerSocket::~ServerSocket() {
	pDebug("%s...\n", "Distroyng instance of ServerSocket");
	if(-1 != clientSockFileDesc) {
		pDebug("%s...\n", "Closing client socket");
		if(0 != close(clientSockFileDesc)) {
			perror("Error while closing client socket");
		}
	}
}

void ServerSocket::init(Port_t port) {
	pDebug("%s...\n", "Initiating server socket");

	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(port);
	if(0 != bind(sockFileDesc, (struct sockaddr *) &server_address, sizeof(server_address))) {
		perror("Error binding the socket");
		pDebug("the port is %d\n", port);
		throw std::exception(); // TODO: Implement exceptions
	}
	if(0 != listen(sockFileDesc, 1)) {
		perror("Error trying to listen on the socket");
		throw std::exception(); // TODO: Implement exceptions
	}
}

void ServerSocket::initClient() {
	if(-1 == clientSockFileDesc) {
		struct sockaddr_in client_address;
		socklen_t size = sizeof(client_address);
		clientSockFileDesc = accept(sockFileDesc, (struct sockaddr *)&client_address, &size);
		pDebug("Socket descriptor is %d\n", clientSockFileDesc);
	}
}

int ServerSocket::write(PacketData const &data) {
	initClient();
	/*
	int rc = ::write(clientSockFileDesc, data.getDataBuf(), data.getSize());
	if(-1 == rc) {
		perror("Error writing data to the socket");
	}
	*/

	return Socket::write(clientSockFileDesc, data);
}

int ServerSocket::read(PacketData &data) {
	//char buf[PacketData::maxLen()];
	initClient();

	/*
	pDebug("%s\n", "About to read data");
	int rc = ::read(clientSockFileDesc, buf, PacketData::maxLen());
	pDebug("read() returned %d\n", rc);
	if(0 < rc) {
		data.setData(buf, rc);
	} else {
		data.clear();
	}
	*/

	return Socket::read(clientSockFileDesc, data);
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

PSProxy::ServerSocket &PSProxy::operator <<(ServerSocket &socket, PacketData &data) {
	socket.write(data);
	
	return socket;
}

