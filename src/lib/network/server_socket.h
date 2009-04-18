#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "socket.h"

namespace PSProxy {
	class ServerSocket : public Socket {
		public:
			ServerSocket(Port_t port);
			virtual ~ServerSocket();

			bool clientWaitingForConnection();
			void initClient();
			virtual int write(PacketData const &data);
			virtual int read(PacketData &data);
			friend ServerSocket &operator <<(ServerSocket &socket, PacketData &data);

		protected:
			void init(Port_t port);

			int clientSockFileDesc;
	};

	ServerSocket &operator <<(ServerSocket &socket, PacketData &data);
}
#endif // SERVER_SOCKET_H
