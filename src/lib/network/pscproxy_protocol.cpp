/*
 * Copyright © 2009 Patryk Cisek
 *
 * This file is part of PSCProxy.
 *
 * PSCProxy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PSCProxy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PSCProxy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "debug.h"
#include "pscproxy_protocol.h"

using namespace PSCProxy;

void PSCProxyProtocol::prepareAuth(PacketData &data, std::string const &user, std::string const &pass) {
	unsigned int userLen = user.size();
	unsigned int passLen = pass.size();
	uint16_t size = sizeof(uint16_t) + 1 + (user.size() + 1) + (pass.size() + 1);
	char buf[size];
	char *s;
        unsigned int i;

	setSize(buf, size);

	s = buf + 2; // packet type
	*(s++) = AUTH_REQUEST;

	for(i = 0; i < userLen; i++) {
            *(s++) = user[i];
	}
        *(s++) = 0;
        for(i = 0; i < passLen; i++) {
            *(s++) = pass[i];
        }
        *(s++) = 0;

	data.setData(buf, size);
}

bool PSCProxyProtocol::parseAuth(PacketData const&data, std::string const &user, std::string const &pass) {
	if(0 == data.getSize()) {
		pDebug("%s\n", "data empty! Auth failed!");
		return false;
	}

        const char *dataBuf = data.getDataBuf();
        const char *c;
        uint16_t size = getSize(dataBuf);
	int i;

        if(size != data.getSize()) {
            pDebug("Wrong data size! size=%u, data.getSize()=%u\n", size, data.getSize());
            return false;
        }
        
	c = dataBuf + 2;
	if(*(c++) != AUTH_REQUEST) {
		pDebug("%s\n", "Wrong packet type on handshake!! Should be AUTH_REQUEST");
		return false;
	}

	i = 0;
	while(*c != 0 || user[i] != 0) {
		if(*(c++) != user[i++]) {
			pDebug("Wrong user! Check failed at pos %d. (dataBuf + 3)=%s, user=%s\n", i - 1, dataBuf + 3, user.c_str());
			return false;
		}
	}

	const char *p = ++c;
	i = 0;
	while(*c != 0 || pass[i] != 0) {
		if(*(c++) != pass[i++]) {
			pDebug("Wrong pass! Check failed at pos %d. p=%s, pass=%s\n", i - 1, p, pass.c_str());
			return false;
		}
	}

	return true;
}

void PSCProxyProtocol::prepareAuthReply(PacketData &data, bool authorized) {
	uint16_t size = sizeof(size) + sizeof(uint8_t) + sizeof(uint8_t);
	char buf[size];

	setSize(buf, size);

	char *s = buf + 2; // Packet type
	*(s++) = AUTH_REPLY;

	*s = authorized? AUTHORIZED : UNAUTHORIZED; // Authorization result
	data.setData(buf, size);
}

bool PSCProxyProtocol::parseAuthReply(PacketData const &data) {
	if(0 == data.getSize()) {
		pDebug("%s\n", "data empty! Auth failed!");
		return false;
	}

        const char *dataBuf = data.getDataBuf();
        const char *c;
        uint16_t size = getSize(dataBuf);

	if(size != data.getSize()) {
		pDebug("%s\n", "Wrong packet size!!");
		return false;
	}

	c = dataBuf + 2; // Packet type
	if(AUTH_REPLY != *(c++)) {
		pDebug("Wrong packet type (%x)!!", *(dataBuf + 2));
		return false;
	}

	pDebug("Returning: %s\n", (AUTHORIZED == *c)? "AUTHORIZED" : "UNAUTHORIZED");
	return (AUTHORIZED == *c)? true : false;
}

void PSCProxyProtocol::setSize(char *buf, uint16_t size) {
	buf[0] = (size >> 8);
	buf[1] = size & 0x00ff;
}

uint16_t PSCProxyProtocol::getSize(const char *buf) {
	return (buf[0] << 8) + buf[1];
}
