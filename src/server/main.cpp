#include <iostream>
#include <csignal>
#include <execinfo.h>
#include <cstdlib>

#include "cardreader.h"
#include "debug.h"

static void PCProxySignalHandler(int signal) {
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
	signal(SIGSEGV, PCProxySignalHandler);
	signal(SIGINT,  PCProxySignalHandler);
	signal(SIGTERM, PCProxySignalHandler);
	signal(SIGUSR1, PCProxySignalHandler);
	signal(SIGPIPE, SIG_IGN);

}

int main(int argc, char *argv[]) {
	std::cout << "Hello World..." << std::endl;
	enableSignalHandling();
	PCProxy::CardReader reader;

	return 0;
}

