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
#include "proxy_client.h"

using namespace PSCProxy;

ProxyClient::ProxyClient(CardEmulator *initEmulator, ClientSocket *initClientSocket) {
	pDebug("%s\n", "Creating instance of ProxyClient");
}

ProxyClient::~ProxyClient() {
	pDebug("%s\n", "Destroying instance of ProxyClient");
}

void ProxyClient::tick() {
	pDebug("%s\n", "Tick...");
}

void ProxyClient::run() {
	while(1) {
		tick();
		usleep(10e4);
	}
}

