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

int PSCProxyServer::Client::read(PacketData &data) {
	return Socket::read(socket, data);
}

int PSCProxyServer::Client::write(PacketData const &data) {
	return Socket::write(socket, data);
}

void PSCProxyServer::Client::checkAuth() {
	if(Socket::newDataInSocket(socket)) {
		pDebug("%s\n", "There's new data in socket. Let's try to authenticate");
		PacketData data;
		read(data);
		if(PSCProxyProtocol::parseAuth(data, user, pass)) {
			PSCProxyProtocol::prepareAuthReply(data, true);
			state = AUTHENTICATED;
			pDebug("%s\n", "Changing state to AUTHENTICATED");
		} else {
			PSCProxyProtocol::prepareAuthReply(data, false);
			state = CLOSED;
			pDebug("%s\n", "Changing state to CLOSED");
		}
		write(data);
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
	Clients::const_iterator end = clients.end();
	for(Clients::iterator it = clients.begin(); it != end; ++it) {
		if((*it)->closed()) {
			pDebug("%s\n", "About to erase the inactive iterator...");
			clients.erase(it);
			delete *it;
		}
	}
}

