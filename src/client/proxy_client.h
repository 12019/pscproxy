#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H

#include "card_emulator.h"
#include "client_socket.h"

namespace PSCProxy {
	class ProxyClient {
		public:
			ProxyClient(CardEmulator *initEmulator, ClientSocket *initClientSocket);
			virtual ~ProxyClient();

			virtual bool tick() = 0;
			void run();
			void exit() { exitRequested = true; }

		protected:
			CardEmulator *emulator;
			ClientSocket *clientSocket;
			bool exitRequested;
	};
}

#endif // PROXY_CLIENT_H
