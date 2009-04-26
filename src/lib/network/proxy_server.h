#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include "cardreader.h"
#include "server_socket.h"

namespace PSCProxy {
	class ProxyServer {
		public:
			ProxyServer(CardReader *initReader, ServerSocket *initServerSocket);
			virtual ~ProxyServer();

			Data_t const &getAtr() const;

			virtual void tick() = 0;
			void run();

		protected:
			CardReader   *reader;
			ServerSocket *serverSocket;
			
	};
}

#endif // PROXY_SERVER_H
