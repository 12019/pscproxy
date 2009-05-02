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
			static int read(int fileDescriptor, PacketData &data, unsigned int size);
			static bool newDataInSocket(int socket);

			int socket() const { return sockFileDesc; }

		protected:
			int sockFileDesc;

	};
}

#endif // SOCKET_H
