#ifndef CARD_EMULATOR_CONFIG_H
#define CARD_EMULATOR_CONFIG_H

#include <string>

namespace PSCProxy {
	class CardEmulatorConfig {
		public:
			CardEmulatorConfig(std::string const &file);
			~CardEmulatorConfig();
			const char *getDevFilename() const { return devFilename.c_str(); }

		private:
			std::string devFilename;
	};
}

#endif // CARD_EMULATOR_CONFIG_H
