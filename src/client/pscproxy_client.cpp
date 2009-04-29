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

#include "debug.h"
#include "pscproxy_client.h"

using namespace PSCProxy;

PSCProxyClient::PSCProxyClient(CardEmulator *initEmulator, ClientSocket *initClientSocket)
: ProxyClient(initEmulator, initClientSocket), state(INIT),
	user("Prezu"), pass("PrezuPass"), // TODO: Implement configuration for the Client
	connected(false) {
	pDebug("%s\n", "Creating instance of PSCProxyClient");
}

PSCProxyClient::~PSCProxyClient() {
	pDebug("%s\n", "Destroying instance of PSCProxyClient");
}

bool PSCProxyClient::tick() {
	if(emulator && clientSocket) {
		switch(state) {
			case INIT:
				pDebug("%s\n", " IN INIT STATE!!");
				authenticate();
				break;

			case AUTH_REQUESTED:
				pDebug("%s\n", " IN AUTH_REQUESTED STATE!!");
				checkAuthReply();
				break;

			case AUTHORIZED:
				pDebug("%s\n", " IN AUTHORIZED STATE!!");
				emulator->tick();
				break;

			case CLOSED:
				pDebug("%s\n", " IN CLOSED STATE!!");
				return false;

			default:
				pDebug("%s\n", "Unknown FSM's state! Should never happen!");
				state = CLOSED;
				break;
		}
	}

	return true;
}

void PSCProxyClient::authenticate() {
	PacketData data;
	pDebug("%s\n", "");
	PSCProxyProtocol::prepareAuth(data, user, pass);
	pDebug("getSize()=%d, clientSocket=%p\n", data.getSize(), clientSocket);
	clientSocket->write(data);
	pDebug("%s\n", "");
	state = AUTH_REQUESTED;
}

void PSCProxyClient::checkAuthReply() {
	if(clientSocket->newDataInSocket()) {
		PacketData data;
		clientSocket->read(data);
		if(PSCProxyProtocol::parseAuthReply(data)) {
			pDebug("%s\n", "Changing state to AUTHORIZED");
			state = AUTHORIZED;
		} else {
			pDebug("%s\n", "Changing state to CLOSED");
			state = CLOSED;
		}

		return;
	}
}
