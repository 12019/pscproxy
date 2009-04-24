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

			virtual int write(PacketData const &data) = 0;
			virtual int read(PacketData &data) = 0;

		protected:
			int write(int fileDescriptor, PacketData const &data);
			int read(int fileDescriptor, PacketData &data);

			int sockFileDesc;

	};
}

#endif // SOCKET_H
