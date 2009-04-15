#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "debug.h"
#include "socket.h"

using namespace PSProxy;

Socket::Socket() {
	pDebug("%s...\n", "Creating instance of Socket");
	sockFileDesc = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::~Socket() {
	pDebug("%s...\n", "Distroyng instance of Socket");
	close(sockFileDesc);
}
