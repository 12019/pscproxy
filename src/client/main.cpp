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

#include <iostream>
#include <csignal>
#include <execinfo.h>
#include <cstdlib>

#ifdef HAVE_GNUTLS
#include <gnutls/gnutls.h>
#include <gcrypt.h>
#include <errno.h>
#include <pthread.h>

GCRY_THREAD_OPTION_PTHREAD_IMPL;
#endif // HAVE_GNUTLS

#include "client_socket.h"
#include "debug.h"
#include "dummy_card_emulator.h"
#include "pscproxy_client.h"
#include "season.h"

static void PSCProxySignalHandler(int signal) {
#define SIZE 100
	std::cerr << "Caught " << signal << " signal!!" << std::endl;
	if(SIGINT == signal) {
		exit(0);
	}

	if(SIGSEGV == signal  || SIGTERM == signal) {
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
#ifdef HAVE_GNUTLS
	gcry_control (GCRYCTL_SET_THREAD_CBS, &gcry_threads_pthread);
	gnutls_global_init();
#endif // HAVE_GNUTLS
	enableSignalHandling();

	PSCProxy::CardEmulator *emulator = new PSCProxy::DummyCardEmulator();
	PSCProxy::ClientSocket *socket = new PSCProxy::ClientSocket("127.0.0.1", PSCProxy::defaultPort);
	PSCProxy::ProxyClient  *client = new PSCProxy::PSCProxyClient(emulator, socket);
	client->run();

	/*
	int rc;
	PSCProxy::Data_t atr, a;
	atr.push_back(0x3B);
	atr.push_back(0x24);
	atr.push_back(0x00);
	atr.push_back(0x30);
	atr.push_back(0x42);
	atr.push_back(0x30);
	atr.push_back(0x30);

	while(1) {
		rc = emulator->tick();
		if(0 != (rc & PSCProxy::CardEmulator::ResetRequested)) {
			pDebug("%s\n", "Requested reset... Writing ATR to card emulator");
			emulator->write(atr);
		}

		if(0 != (rc & PSCProxy::CardEmulator::ReadDataAvail)) {
			pDebug("%s\n", "There's some data in emulator's buffer waiting to be read... Reading");
			emulator->read(a);
		}
		usleep(10e4);
	}

	return 0;

	PSCProxy::Season season(PSCProxy::CardEmulatorConfig("~/.pscproxy-client.conf"));

	while(!season.getLines2()) {
		pDebug("%s\n", "Yet offline");
	}
	
	// 3B 24 00 30 FF 3B 24 00 30 42 30 30
	PSCProxy::Data_t tmp;
	season.read(tmp);

	PSCProxy::Data_t a;
	a.push_back(0x3B);
	a.push_back(0x24);
	a.push_back(0x00);
	a.push_back(0x30);
	a.push_back(0x42);
	a.push_back(0x30);
	a.push_back(0x30);
	season.write(a);
	if(season.readDataAvail()) {
		pDebug("%s\n", "Got answer to ATR");
		season.read(a);
		for(unsigned i = 0; i < a.size(); i++) {
			pDebug("Got byte: %X\n", a[i]);
		}

	}

	return 0;
	*/
}
