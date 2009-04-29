#ifndef COMMON_DEFINES_H
#define COMMON_DEFINES_H

#include <vector>

#include "lib/network/socket.h"

namespace PSCProxy {
	typedef std::vector<unsigned char> Data_t;
	const Port_t defaultPort = 15005;
}

#endif // COMMON_DEFINES_H
