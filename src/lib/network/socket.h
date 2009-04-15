#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>

namespace PSProxy {
	typedef uint16_t Port_t;
	class Socket {
		public:
			Socket();
			~Socket();

		protected:
			int sockFileDesc;

	};
}

#endif // SOCKET_H
