#ifndef CARD_EMULATOR_CONFIG_H
#define CARD_EMULATOR_CONFIG_H

#include <string>

#include "card_emulator.h"

namespace PSCProxy {
	class CardEmulatorConfig {
		public:
			enum EmulatorType {
				DUMMY,
				SEASON
			};

			CardEmulatorConfig(std::string const &file);
			CardEmulatorConfig(CardEmulatorConfig const &c);
			~CardEmulatorConfig();

			CardEmulatorConfig &operator =(const CardEmulatorConfig &rhs);
			const char *getDevName() const { return devName.c_str(); }
			EmulatorType getEmulatorType() const { return emulatorType; }

			unsigned int getResetDelay() const { return resetDelay; }
			unsigned int getDataTXDelay() const { return dataTXDelay; }

			CardEmulator *createNewEmulator();

		private:

			unsigned int strToUInt(std::string const &str);

			void setEmulatorType(std::string const &type);
			void setDevName(std::string const &name) { devName = name; }
			void setDataTXDelay(unsigned int newDataTXDelay) { dataTXDelay = newDataTXDelay; }
			void setResetDelay(unsigned int newResetDelay) { resetDelay = resetDelay; }

			void parseLine(std::string const &line);
			void handleKeyValue(std::string const &key, std::string const &value);

			std::string devName;
			unsigned int resetDelay, dataTXDelay;
			EmulatorType emulatorType;
	};
}

#endif // CARD_EMULATOR_CONFIG_H
