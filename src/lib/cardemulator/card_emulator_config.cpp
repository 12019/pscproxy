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

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "card_emulator_config.h"
#include "dummy_card_emulator.h"
#include "season.h"
#include "debug.h"

using namespace PSCProxy;

CardEmulatorConfig::CardEmulatorConfig(std::string const &file) {
	pDebug("Creating instance of CardEmulatorConfig. File is %s\n", file.c_str());

	// First off... Setting defaults
	devName = "/dev/ttyS0";
	resetDelay = 10000;
	dataTXDelay = 200;
	emulatorType = DUMMY;

	std::ifstream configFile;
	configFile.exceptions(std::ifstream::badbit);
	std::string errMsg("Error while opening ");
	errMsg += file;
	try {
		configFile.open(file.c_str());
	} catch (...) {
		throw std::runtime_error(errMsg);
	}

	errMsg = "Error while reading " + file;
	std::string line;
	while(!configFile.eof()) {
		try {
			getline(configFile, line);
		} catch(...) {
			throw std::runtime_error(errMsg);
		}
		pDebug("parsing <%s>\n", line.c_str());
		parseLine(line);
	}
}

CardEmulatorConfig::CardEmulatorConfig(const CardEmulatorConfig &c) {
	pDebug("%s\n", "Copy ctor!");

	devName = c.devName;
	resetDelay = c.resetDelay;
	dataTXDelay = c.dataTXDelay;
	emulatorType = c.emulatorType;
}

CardEmulatorConfig &CardEmulatorConfig::operator =(const CardEmulatorConfig &rhs) {
	if(&rhs == this) {
		return *this;
	}

	devName = rhs.devName;
	resetDelay = rhs.resetDelay;
	dataTXDelay = rhs.dataTXDelay;
	emulatorType = rhs.emulatorType;

	return *this;
}

CardEmulatorConfig::~CardEmulatorConfig() {
}

CardEmulator *CardEmulatorConfig::createNewEmulator() {
	switch(emulatorType) {
		case DUMMY:
			pDebug("%s\n", "About to create DummyCardEmulator instance...");
			return new DummyCardEmulator();

		case SEASON:
			pDebug("%s\n", "About to create Season instance...");
			return new Season(*this);

		default:
			throw std::runtime_error("Unknown emulator type");
	}
}

unsigned int CardEmulatorConfig::strToUInt(std::string const &str) {
	std::string::const_iterator it;
	std::string::const_iterator endIt = str.end();
	std::string errMsg;
	unsigned int ret = 0;
	for(it = str.begin(); it != endIt; ++it) {
		if(*it < '0' || *it > '9') {
			errMsg = "Non-numeric character in " + str;
			throw std::runtime_error(errMsg);
		}
		ret = ret * 10 + (*it - '0');
	}

	pDebug("Returning %u\n", ret);
	return ret;
}

void CardEmulatorConfig::setEmulatorType(std::string const &type) {
	pDebug("Setting emulator type to %s\n", type.c_str());
	if("season" == type) {
		emulatorType = SEASON;
	} else if("dummy" == type) {
		emulatorType = DUMMY;
	} else {
		throw std::runtime_error("Unknown card emulator type in config file!");
	}
}

void CardEmulatorConfig::parseLine(std::string const &line) {
	std::string cleanedLine;
	std::string::const_iterator it;
	std::string::const_iterator endIt = line.end();
	for(it = line.begin(); it != endIt; ++it) {
		if('#' == *it ||
				'\t' == *it ||
				' ' == *it) {
			break;
		}
		cleanedLine += *it;
	}
	if(0 == cleanedLine.size()) {
		return;
	}

	std::string key, value;
	endIt = cleanedLine.end();
	for(it = cleanedLine.begin(); it != endIt && '=' != *it; ++it) {
		key += *it;
	}
	if('=' != *it) {
		pDebug("Got %x\n", (unsigned short)(*it));
		std::string errMsg("Error while parsing config file line: <" + line + ">");
		throw std::runtime_error(errMsg);
	}

	for(++it; it != endIt; ++it) {
		value += *it;
	}

	handleKeyValue(key, value);
}

void CardEmulatorConfig::handleKeyValue(std::string const &key, std::string const &value) {
	// If it's not emulator's key, just forget it
	const std::string prefix("card_emulator.");
	if(prefix != key.substr(0, prefix.size())) {
		return;
	}

	std::string configKey = key.substr(prefix.size());
	if("type" == configKey) {
		setEmulatorType(value);
	} else if("dev_name" == configKey) {
		setDevName(value);
	} else if("data_tx_delay" == configKey) {
		setDataTXDelay(strToUInt(value));
	} else if("reset_delay" == configKey) {
		setResetDelay(strToUInt(value));
	}
}

