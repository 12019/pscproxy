#include "debug.h"
#include "socket.h"

using namespace PSProxy;

Socket::Socket() {
	pDebug("%s...\n", "Creating instance of Socket");
}

Socket::~Socket() {
	pDebug("%s...\n", "Distroyng instance of Socket");
}
