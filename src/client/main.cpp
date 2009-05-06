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
#include "card_emulator_config.h"
//#include "season.h"

static PSCProxy::ProxyClient *client;
static void PSCProxySignalHandler(int signal) {
#define SIZE 100
	std::cerr << "Caught " << signal << " signal!!" << std::endl;

	if(SIGINT == signal) {
		client->exit();
	} else if(SIGSEGV == signal  || SIGTERM == signal) {
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

	PSCProxy::CardEmulator *emulator = PSCProxy::CardEmulatorConfig("/home/patryk/.pscproxy_client.conf").createNewEmulator();
	PSCProxy::ClientSocket *socket = new PSCProxy::ClientSocket("127.0.0.1", PSCProxy::defaultPort);
	client = new PSCProxy::PSCProxyClient(emulator, socket);
	client->run();
}
