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

#include <exception>

#include <netinet/in.h>

#include "debug.h"
#include "server_socket.h"

using namespace PSCProxy;

ServerSocket::ServerSocket(Port_t port)
: Socket() {
	pDebug("%s...\n", "Creating instance of ServerSocket");
	init(port);
}

ServerSocket::~ServerSocket() {
	pDebug("%s...\n", "Distroyng instance of ServerSocket");
	//unsigned int size = clientSockets.size();
	//for(unsigned int i = 0; i < size; i++) {
	//	pDebug("%s...\n", "Closing client socket");
	//	if(0 != close(clientSockets[i])) {
	//		perror("Error while closing client socket");
	//	}
	//}
}

bool ServerSocket::newClientWaitingForConnection() {
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

int ServerSocket::connectWaitingClient() {
	if(newClientWaitingForConnection()) {
		struct sockaddr_in client_address;
		socklen_t size = sizeof(client_address);
		int newClientSocket = accept(sockFileDesc, (struct sockaddr *)&client_address, &size);
		pDebug("Socket descriptor is %d\n", newClientSocket);
		return newClientSocket;
		//clientSockets.push_back(newClientSocket);
	}

	return -1;
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

#if 0
int ServerSocket::write(PacketData const &data, unsigned int sockIdx) {
	if(clientSockets.size() < sockIdx) {
		throw std::exception(); // TODO: Implement expeptions
	}

	return Socket::write(clientSockets[sockIdx], data);
}

bool ServerSocket::newDataInSocket(int desc) {
	fd_set descriptors;
	struct timeval timev;

	FD_ZERO(&descriptors);
	FD_SET(desc, &descriptors);
	timev.tv_sec = 0;
	timev.tv_usec = 1;

	pDebug("About to check data in socket %d... ", desc);
	int rc = select(desc + 2, &descriptors, NULL, NULL, &timev);
	if(-1 == rc) {
		perror("Error checking client socket state");
	}

	pDebug("Got rc=%d\n", rc);

	return (0 == rc? false : true);
}

void ServerSocket::checkDataInSockets() {
	unsigned int size = clientSockets.size();
	readReadySockets.clear();

	for(unsigned int i = 0; i < size; i++) {
		if(newDataInSocket(clientSockets[i])) {
			readReadySockets.push_back(clientSockets[i]);
		}
	}
}

void ServerSocket::tick() {
	int newDesc;

	checkDataInSockets();
	do {
		newDesc = connectWaitingClient();
	} while(newDesc > 0);
}
#endif

