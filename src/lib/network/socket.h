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

			virtual int write(PacketData const &data);
			virtual int read(PacketData &data);

		protected:
			int sockFileDesc;

	};
}

#endif // SOCKET_H
