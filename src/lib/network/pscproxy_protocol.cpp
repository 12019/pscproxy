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
	char *c;
        unsigned int i;

	setSize(buf, size);

	c = buf + 2; // packet type
	*(c++) = AUTH_REQUEST;

	for(i = 0; i < userLen; i++) {
            *(c++) = user[i];
	}
        *(c++) = 0;
        for(i = 0; i < passLen; i++) {
            *(c++) = pass[i];
        }
        *(c++) = 0;

	data.setData(buf, size);
}

bool PSCProxyProtocol::parseAuth(PacketData const&data, std::string const &user, std::string const &pass) {
	if(!checkPacketSanity(data, AUTH_REQUEST)) {
		pDebug("%s\n", "Paktet doesn't look sane..");
		return false;
	}

	int i = 0;
	const char *c = data.getDataBuf() + 3;
	while(*c != 0 || user[i] != 0) {
		if(*(c++) != user[i++]) {
			pDebug("Wrong user! Check failed at pos %d. (dataBuf + 3)=%s, user=%s\n", i - 1, data.getDataBuf() + 3, user.c_str());
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

	char *c = buf + 2; // Packet type
	*(c++) = AUTH_REPLY;

	*c = authorized? AUTHORIZED : UNAUTHORIZED; // Authorization result
	data.setData(buf, size);
}

bool PSCProxyProtocol::parseAuthReply(PacketData const &data) {
	if(!checkPacketSanity(data, AUTH_REPLY)) {
		pDebug("%s\n", "Paktet doesn't look sane..");
		return false;
	}

	const char *c = data.getDataBuf() + 3;
	pDebug("Returning: %s\n", (AUTHORIZED == *c)? "AUTHORIZED" : "UNAUTHORIZED");
	return (AUTHORIZED == *c)? true : false;
}

void PSCProxyProtocol::prepareResetRequest(PacketData &data) {
	uint16_t size = sizeof(size) + sizeof(uint8_t);
	char buf[size];

	setSize(buf, size);
	char *c = buf + 2; // Packet type
	*c = RESET_REQUEST;
	data.setData(buf, size);
}

bool PSCProxyProtocol::parseResetRequest(PacketData const &data) {
	return checkPacketSanity(data, RESET_REQUEST);
}

void PSCProxyProtocol::prepareResetReply(PacketData &data, Data_t const &atr) {
	pDebug("%s\n", "Preparing reset reply");
	uint16_t atrSize = atr.size();
	uint16_t size = sizeof(size) + sizeof(uint8_t) + atrSize;
	char buf[size];

	pDebug("ATR size is %u, size is %u\n", atrSize, size);

	setSize(buf, size);
	char *c = buf + 2; // Packet type
	*(c++) = RESET_REPLY;
	for(int i = 0; i < atrSize; i++) {
		pDebug("Adding byte %x\n", atr[i]);
		*(c++) = atr[i];
	}
	data.setData(buf, size);
}

bool PSCProxyProtocol::parseResetReply(PacketData const &data, Data_t &buf) {
	if(!checkPacketSanity(data, RESET_REPLY)) {
		pDebug("%s\n", "Paktet doesn't look sane..");
		return false;
	}

	const char *c = data.getDataBuf() + 3;
	unsigned int atrSize = data.getSize() - 3; // Header is 3 bytes long
	buf.clear();
	for(unsigned int i = 0; i < atrSize; i++) {
		buf.push_back(*(c++));
	}

	return true;
}

void PSCProxyProtocol::setSize(char *buf, uint16_t size) {
	buf[0] = (size >> 8);
	buf[1] = size & 0x00ff;
}

uint16_t PSCProxyProtocol::getSize(const char *buf) {
	return (buf[0] << 8) + buf[1];
}

bool PSCProxyProtocol::checkPacketSanity(PacketData const &data, PacketType type) {
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
	if(type != *(c++)) {
		pDebug("Wrong packet type (%x)!!", *(dataBuf + 2));
		return false;
	}

	return true;
}
