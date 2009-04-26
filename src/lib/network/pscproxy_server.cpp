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

#include <sys/select.h>
#include <unistd.h>

#include "debug.h"
#include "pscproxy_server.h"

using namespace PSCProxy;

PSCProxyServer::Client::Client(int s, std::string initUser, std::string initPass)
: state(INIT), socket(s), user(initUser), pass(initPass) {
	pDebug("%s\n", "Creating instance of PSCProxyServer::Client");
}

PSCProxyServer::Client::~Client() {
	pDebug("%s\n", "Destroying instance of PSCProxyServer::Client");
	if(-1 == close(socket)) {
		perror("Error closing socket\n");
	}
}

void PSCProxyServer::Client::tick() {
	switch(state) {
		case INIT:
			checkAuth();
			break;
			
		case AUTHENTICATED:
			break;

		case CLOSED:
			break;

		default:
			break;
	}
}

bool PSCProxyServer::Client::newDataInSocket() {
	fd_set descriptors;
	struct timeval timev;

	FD_ZERO(&descriptors);
	FD_SET(socket, &descriptors);
	timev.tv_sec = 0;
	timev.tv_usec = 1;

	int rc = select(socket + 2, &descriptors, NULL, NULL, &timev);
	if(-1 == rc) {
		perror("Error checking client socket state");
	}

	return (0 == rc? false : true);
}

int PSCProxyServer::Client::read(PacketData &data) {
	return Socket::read(socket, data);
}

void PSCProxyServer::Client::checkAuth() {
	if(newDataInSocket()) {
		pDebug("%s\n", "There's new data in socket. Let's try to authenticate");
		PacketData data;
		read(data);
		unsigned int len = data.getDataBuf()[0] + data.getDataBuf()[1] * 0x100;
		pDebug("len=%d, data.getSize()=%d\n", len, data.getSize());
		if(PSCProxyProtocol::parseAuth(data, user, pass)) {
			state = AUTHENTICATED;
		} else {
			state = CLOSED;
		}
	}
}

PSCProxyServer::PSCProxyServer(CardReader *initReader, ServerSocket *initServerSocket)
: ProxyServer(initReader, initServerSocket) {
	pDebug("%s\n", "Creating instance of PSCProxyServer");
}

PSCProxyServer::~PSCProxyServer() {
	pDebug("%s\n", "Destroying instance of PSCProxyServer");
	while(!clients.empty()) {
		clients.pop_back();
	}
}

void PSCProxyServer::tick() {
	// Handle any possible waiting client
	handleNewClients();

	// Tick clients
	tickClients();

	// Cliean closed clients
	closeInactiveClients();
}

void PSCProxyServer::handleNewClients() {
	if(serverSocket) {
		int newDesc;
		while(0 < (newDesc = serverSocket->connectWaitingClient())) {
			clients.push_back(new Client(newDesc, std::string("Prezu"),
						std::string("PrezuPass"))); // TODO: Implement client's configuration
		}
	}
}

void PSCProxyServer::tickClients() {
	Clients::const_iterator e = clients.end();
	for(Clients::iterator it = clients.begin(); it != e; ++it) {
		(*it)->tick();
	}
}

void PSCProxyServer::closeInactiveClients() {
	unsigned int i = 0;
	while(i <= clients.size()) {
		if(clients[i]->closed()) {
		}
	}
}

