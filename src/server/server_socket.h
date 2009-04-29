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

		protected:
			void init(Port_t port);
	};
}
#endif // SERVER_SOCKET_H
