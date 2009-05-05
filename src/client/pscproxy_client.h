#ifndef PSCPROXY_CLIENT_H
#define PSCPROXY_CLIENT_H

#include "proxy_client.h"
#include "pscproxy_protocol.h"

namespace PSCProxy {
	class PSCProxyClient : public ProxyClient {
		public:
			PSCProxyClient(CardEmulator *initEmulator, ClientSocket *initClientSocket);
			virtual ~PSCProxyClient();

			virtual bool tick();

		private:
			enum State {
				INIT,
				AUTH_REQUESTED,
				AUTHORIZED,
				AWAITING_ATR,
				IDLE,
				CLOSED
			};

			int read(PacketData &data);
			void authorize();
			void checkAuthReply();
			void requestATR();
			void cacheATR();
			void handleEmulatorRequests();

			State state;
			std::string user, pass;
			bool connected;
			Data_t atr;
	};
}

#endif // PSCPROXY_CLIENT_H
