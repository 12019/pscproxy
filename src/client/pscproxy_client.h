#ifndef PSCPROXY_CLIENT_H
#define PSCPROXY_CLIENT_H

#include "proxy_client.h"
#include "pscproxy_protocol.h"

namespace PSCProxy {
	class PSCProxyClient : public ProxyClient {
		public:
			PSCProxyClient(CardEmulator *initEmulator, ClientSocket *initClientSocket);
			virtual ~PSCProxyClient();
	};
}

#endif // PSCPROXY_CLIENT_H
