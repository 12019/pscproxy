#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "socket.h"

namespace PSCProxy {
	class ServerSocket : public Socket {
		public:
			ServerSocket(Port_t port);
			~ServerSocket();

			bool newClientWaitingForConnection();
			int connectWaitingClient();
			void initClient();
			//int write(PacketData const &data, unsigned int sockIdx);

			//bool newDataInSocket(int desc);
			//void checkDataInSockets();
			//void tick();
			//Sockets const &getReadReadySockets() const { return readReadySockets; }

		protected:
			void init(Port_t port);

			//Sockets clientSockets;
			//Sockets readReadySockets;
	};
}
#endif // SERVER_SOCKET_H
