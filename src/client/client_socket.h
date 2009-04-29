#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <string>

#include "socket.h"

namespace PSCProxy {
	class ClientSocket : public Socket {
		public:
			ClientSocket(std::string host, Port_t port);
			virtual ~ClientSocket();

			virtual int write(PacketData const &data);
			virtual int read(PacketData &data);
			friend ClientSocket &operator <<(ClientSocket &socket, PacketData &data);
			bool newDataInSocket() const { return Socket::newDataInSocket(sockFileDesc); }

		protected:
			void init(std::string host, Port_t port);
	};

	ClientSocket &operator <<(ClientSocket &socket, PacketData &data);
}
#endif // CLIENT_SOCKET_H
