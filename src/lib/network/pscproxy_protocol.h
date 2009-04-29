#ifndef PSCPROXY_PROTOCOL_H
#define PSCPROXY_PROTOCOL_H

#include <string>

#include "packet_data.h"

namespace PSCProxy {
	class PSCProxyProtocol {
		public:
			enum AuthResult {
				AUTHORIZED,
				UNAUTHORIZED
			};
			static void prepareAuth(PacketData &data, std::string const &user, std::string const &pass);
			static bool parseAuth(PacketData const&data, std::string const &user, std::string const &pass);

			static void prepareAuthReply(PacketData &data, bool authorized);
			static bool parseAuthReply(PacketData const &data);

		private:
			enum PacketType {
				AUTH_REQUEST,
				AUTH_REPLY
			};

			static void setSize(char *buf, uint16_t size);
			static uint16_t getSize(const char *buf);
	};
}

#endif // PSCPROXY_PROTOCOL_H

