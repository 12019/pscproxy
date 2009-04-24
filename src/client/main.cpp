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

#include "debug.h"
#include "season.h"
#include "client_socket.h"

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

	std::cout << "Hello world!!" << std::endl;
	PSCProxy::ClientSocket socket("127.0.0.1", 10000);
	PSCProxy::PacketData data;
	//do {
		pDebug("%s\n", "Reading data from the socket...");
		socket.read(data);
		pDebug("Read %d bytes...\n", data.getSize());
		if(0 < data.getSize()) {
			pDebug("Read the following str: <%s>\n", data.getData().c_str());
		} else {
			pDebug("%s\n", "Read shit...");
		}
		data.setData("Bye");
		//socket.write(data);
		socket << data;
	//} while(1);

	return 0;

	PSCProxy::Season season(PSCProxy::SeasonConfig("~/.pscproxy-client.conf"));

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
}
