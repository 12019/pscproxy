#ifndef PSCPROXY_SERVER_H
#define PSCPROXY_SERVER_H

#include <vector>

#include "proxy_server.h"
#include "pscproxy_protocol.h"

namespace PSCProxy {
	class PSCProxyServer : public ProxyServer {
		public:
			class Client {
				public:
					enum State {
						INIT,
						AUTHENTICATED,
						//AUTH_FAILED,
						CLOSED
					};

					Client(int s, std::string initUser, std::string initPass);
					~Client();

					bool closed() const { return CLOSED == state? true : false; }

					void tick();

				private:
					bool newDataInSocket();
					int read(PacketData &data);
					void checkAuth();

					State state;
					int  socket;
					bool authenticated;

					std::string user, pass;
			};

			typedef std::vector<Client *> Clients;

			PSCProxyServer(CardReader *initReader, ServerSocket *initServerSocket);
			virtual ~PSCProxyServer();

			virtual void tick();

		private:
			void handleNewClients();
			void closeInactiveClients();
			void tickClients();

			Clients clients;
	};
}

#endif // PSCPROXY_SERVER_H
