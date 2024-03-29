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
				authorize();
				break;

			case AUTH_REQUESTED:
				pDebug("%s\n", " IN AUTH_REQUESTED STATE!!");
				checkAuthReply();
				break;

			case AUTHORIZED:
				pDebug("%s\n", " IN AUTHORIZED STATE!!");
				requestATR();
				break;

			case AWAITING_ATR:
				pDebug("%s\n", " IN AWAITING_ATR STATE!!");
				cacheATR();
				break;

			case IDLE:
				emulator->tick();
				handleEmulatorRequests();
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

int PSCProxyClient::read(PacketData &data) {
	return PSCProxyProtocol::read(data, clientSocket->socket());
}

void PSCProxyClient::authorize() {
	PacketData data;
	PSCProxyProtocol::prepareAuth(data, user, pass);
	clientSocket->write(data);
	state = AUTH_REQUESTED;
}

void PSCProxyClient::checkAuthReply() {
	if(clientSocket->newDataInSocket()) {
		PacketData data;
		read(data);
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

void PSCProxyClient::requestATR() {
	PacketData data;
	PSCProxyProtocol::prepareResetRequest(data);
	if(0 < clientSocket->write(data)) {
		state = AWAITING_ATR;
	} else {
		state = CLOSED;
	}
}

void PSCProxyClient::cacheATR() {
	if(clientSocket->newDataInSocket()) {
		pDebug("%s\n", "Got new data");
		PacketData data;
		if(0 > read(data)) {
			state = CLOSED;
			return;
		}

		if(!PSCProxyProtocol::parseResetReply(data, atr)) {
			pDebug("%s\n", "Failed while parsing reset reply");
			state = CLOSED;
			return;
		}
		state = IDLE;
	}
}

void PSCProxyClient::handleEmulatorRequests() {
	if(emulator->resetRequested()) {
		pDebug("%s\n", "Requested Reset! Handling it...");
		PacketData data;
		pDebug("%s\n", "Preparing Reset Request... ");
		PSCProxyProtocol::prepareResetRequest(data);
		pqDebug("%s\n", "Done.");
		pDebug("%s\n", "Writing Reset Request to server socket... ");
		clientSocket->write(data);
		pqDebug("%s\n", "Done.");
		pDebug("%s\n", "Reading Reset Reply from the server... ");
		read(data);
		pqDebug("%s\n", "Done.");

		Data_t atr;
		if(!PSCProxyProtocol::parseResetReply(data, atr)) {
			pqDebug("%s\n", "Failed to parse reset reply! Changing state to CLOSED");
			state = CLOSED;
			return;
		}

		pDebug("%s\n", "Writing ATR to emulator... ");
		emulator->write(atr);
		pqDebug("%s\n", "Done.");
	} else if(emulator->readDataAvail()) {
		PacketData data;
		Data_t emulatorData;
		emulator->read(emulatorData);
#ifdef DEBUG
		pDebug("%s", "STB is requesting: ");
		for(unsigned int i = 0; i < emulatorData.size(); i++) {
			pqDebug("%X ", emulatorData[i]);
		}
		pqDebug("%s\n", "");
#endif // DEBUG
		bool rstReq = false;
		if(1 == emulatorData.size()) {
			if(0 == emulatorData[0]) {
				rstReq = true;
			}
		}
		if(rstReq) {
			PSCProxyProtocol::prepareResetRequest(data);
		} else {
			PSCProxyProtocol::prepareCmdRequest(data, emulatorData);
		}
		clientSocket->write(data);
		read(data);
		if(!PSCProxyProtocol::parseCmdReply(data, emulatorData)) {
			pqDebug("%s\n", "Failed to parse command reply! Changing state to CLOSED");
			state = CLOSED;
			return;
		}
		emulator->write(emulatorData);
	}
}

