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

#include <csignal>
#include <cstdlib>
#include <execinfo.h>
#include <iostream>
#include <memory>

//#include "phoenix.h"
#include "dummy_cardreader.h"
#include "cardreader_config.h"
#include "pscproxy_server.h"
#include "debug.h"

static PSCProxy::ProxyServer *serverGlobalPtr = NULL;

static void PSCProxySignalHandler(int signal) {
#define SIZE 100
	std::cerr << "Caught " << signal << " signal!!" << std::endl;
	pDebug("SIGINT=%d, SIGSEGV=%d, SIGINT=%d, SIGTERM=%d\n",
			SIGINT, SIGSEGV, SIGINT, SIGTERM);
	if(SIGINT == signal) {
		if(serverGlobalPtr) {
			serverGlobalPtr->exit();
		} else {
			abort();
		}
	} else if(SIGSEGV == signal || SIGINT == signal || SIGTERM == signal) {
		int i, numOfBacktraceStrings;
		void *buf[SIZE];
		char **strings;
		numOfBacktraceStrings = backtrace(buf, SIZE);
		std::cerr << "backtrace (" << numOfBacktraceStrings << " addresses):" << std::endl;
		strings = backtrace_symbols(buf, numOfBacktraceStrings);
		for(i = 0; i < numOfBacktraceStrings; ++i) {
			std::cerr << strings[i] << std::endl;
		}

		abort();
	}
}

void enableSignalHandling() {
	signal(SIGSEGV, PSCProxySignalHandler);
	signal(SIGINT,  PSCProxySignalHandler);
	signal(SIGTERM, PSCProxySignalHandler);
	signal(SIGUSR1, PSCProxySignalHandler);
	signal(SIGPIPE, SIG_IGN);
}

int main(int argc, char *argv[]) {
	enableSignalHandling();

	std::auto_ptr<PSCProxy::CardReader> reader(new PSCProxy::DummyCardReader());
	std::auto_ptr<PSCProxy::ServerSocket> socket(new PSCProxy::ServerSocket(PSCProxy::defaultPort));

	std::auto_ptr<PSCProxy::ProxyServer> server(new PSCProxy::PSCProxyServer(reader.get(), socket.get()));

	server->run();

	/*
	PSCProxy::ServerSocket socket(10000);
	do {
		if(socket.clientWaitingForConnection()) {
			pDebug("%s\n", "Client is waiting for a connection!");
			pDebug("%s\n", "Writing to the client...");
			PSCProxy::PacketData data("Hello world");
			socket << data;
			pDebug("%s\n", "Now reading from client...");
			socket.read(data);
			pDebug("Read %d bytes. The data is <%s>\n", data.getSize(), data.getData().c_str());
			break;
		}
	} while(1);
	return 0;
	PSCProxy::Phoenix reader(PSCProxy::CardReaderConfig("aaa.conf"));
	reader.reset();
	*/

	return 0;
}

