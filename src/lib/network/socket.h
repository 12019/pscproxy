#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>

#include "packet_data.h"

namespace PSCProxy {
	typedef uint16_t Port_t;
	class Socket {
		public:
			Socket();
			virtual ~Socket();

			static int write(int fileDescriptor, PacketData const &data);
			static int read(int fileDescriptor, PacketData &data);

		protected:
			int sockFileDesc;

	};
}

#endif // SOCKET_H
