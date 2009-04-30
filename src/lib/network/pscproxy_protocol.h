#ifndef PSCPROXY_PROTOCOL_H
#define PSCPROXY_PROTOCOL_H

#include <string>

#include "common_defines.h"
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

			static void prepareResetRequest(PacketData &data);
			static bool parseResetRequest(PacketData const &data);

			static void prepareResetReply(PacketData &data, Data_t const &atr);
			static bool parseResetReply(PacketData const &data, Data_t &buf);

		private:
			enum PacketType {
				AUTH_REQUEST,
				AUTH_REPLY,
				RESET_REQUEST,
				RESET_REPLY
			};

			static void setSize(char *buf, uint16_t size);
			static uint16_t getSize(const char *buf);
			static bool checkPacketSanity(PacketData const &data, PacketType type);
	};
}

#endif // PSCPROXY_PROTOCOL_H

