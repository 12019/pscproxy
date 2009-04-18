#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>

#include "packet_data.h"

namespace PSProxy {
	typedef uint16_t Port_t;
	class Socket {
		public:
			Socket();
			virtual ~Socket();

			virtual int write(PacketData const &data) = 0;
			virtual int read(PacketData &data) = 0;

		protected:
			int sockFileDesc;

	};
}

#endif // SOCKET_H
