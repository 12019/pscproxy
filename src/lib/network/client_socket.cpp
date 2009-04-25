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

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>


#include "debug.h"
#include "client_socket.h"

using namespace PSCProxy;

ClientSocket::ClientSocket(std::string host, Port_t port) {
	pDebug("%s...\n", "Creating instance of ClientSocket");
	init(host, port);
}

ClientSocket::~ClientSocket() {
	pDebug("%s...\n", "Distroyng instance of ClientSocket");
}

int ClientSocket::write(PacketData const &data) {
	return Socket::write(sockFileDesc, data);
}

int ClientSocket::read(PacketData &data) {
	return Socket::read(sockFileDesc, data);
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

PSCProxy::ClientSocket &PSCProxy::operator <<(ClientSocket &socket, PacketData &data) {
	socket.write(data);
	return socket;
}

