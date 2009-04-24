#include <iostream>
#include <csignal>
#include <execinfo.h>
#include <cstdlib>

#include "phoenix.h"
#include "cardreader_config.h"
#include "server_socket.h"
#include "debug.h"

static void PSCProxySignalHandler(int signal) {
#define SIZE 100
	std::cerr << "Caught " << signal << " signal!!" << std::endl;
	if(SIGSEGV == signal || SIGINT == signal || SIGTERM == signal) {
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

	return 0;
}

