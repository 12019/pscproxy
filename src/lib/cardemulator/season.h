#ifndef SEASON_H
#define SEASON_H

#include <vector>

#include "card_emulator_config.h"
#include "card_emulator.h"

namespace PSCProxy {
	class Season : public CardEmulator{
		public:
			Season(CardEmulatorConfig const &initConfig);
			~Season();

			bool readDataAvail();
			bool getLines2();
			virtual void read(Data_t &result);
			virtual void write(Data_t const &data);

		private:
			void init();

			CardEmulatorConfig const &config;
			int fileDescriptor;

			const int maxBufferLen;
	};
}
#endif // SEASON_H
