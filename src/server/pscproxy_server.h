#ifndef PSCPROXY_SERVER_H
#define PSCPROXY_SERVER_H

#include <vector>

#include "proxy_server.h"
#include "pscproxy_protocol.h"

namespace PSCProxy {
	class PSCProxyServer : public ProxyServer {
			class Client {
					enum State {
						INIT,
						AUTHORIZED,
						CLOSED
					};

				public:
					Client(int s, CardReader *initReader, std::string initUser, std::string initPass);
					~Client();

					bool closed() const { return CLOSED == state? true : false; }

					void tick();

				private:
					int read(PacketData &data);
					int write(PacketData const &data);
					bool newDataInSocket() const { return Socket::newDataInSocket(socket); }
					void checkAuth();
					void handleClientRequests();

					State state;
					int  socket;
					bool authenticated;

					std::string user, pass;
					CardReader *reader;
			};

			typedef std::vector<Client *> Clients;

		public:
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
