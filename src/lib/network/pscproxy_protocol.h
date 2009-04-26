#ifndef PSCPROXY_PROTOCOL_H
#define PSCPROXY_PROTOCOL_H

#include <string>

#include "packet_data.h"

namespace PSCProxy {
	class PSCProxyProtocol {
		public:
			static void prepareAuth(PacketData &data);
			static bool parseAuth(PacketData const&data, std::string &user, std::string &pass);
			static void prepareAuthReply(PacketData &data);
			static bool parseAuthReply(PacketData const &data);
	};
}

#endif // PSCPROXY_PROTOCOL_H

