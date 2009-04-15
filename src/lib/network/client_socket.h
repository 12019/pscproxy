#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <string>

#include "socket.h"

namespace PSProxy {
	class ClientSocket : public Socket {
		public:
			ClientSocket(std::string host, Port_t port);
			~ClientSocket();

		protected:
			void init(std::string host, Port_t port);
	};
}
#endif // CLIENT_SOCKET_H
