#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H

#include "card_emulator.h"
#include "client_socket.h"

namespace PSCProxy {
	class ProxyClient {
		public:
			ProxyClient(CardEmulator *initEmulator, ClientSocket *initClientSocket);
			virtual ~ProxyClient();

			void tick();
			void run();

		protected:
			CardEmulator *emulator;
			ClientSocket *clientSocket;
	};
}

#endif // PROXY_CLIENT_H
