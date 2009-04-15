#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "socket.h"

namespace PSProxy {
	class ServerSocket : public Socket {
		public:
			ServerSocket(Port_t port);
			~ServerSocket();

			bool clientWaitingForConnection();

		protected:
			void init(Port_t port);
	};
}
#endif // SERVER_SOCKET_H
