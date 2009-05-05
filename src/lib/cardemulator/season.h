#ifndef SEASON_H
#define SEASON_H

#include <vector>

#include "card_emulator_config.h"
#include "card_emulator.h"

namespace PSCProxy {
	class Season : public CardEmulator{
		public:
			Season(CardEmulatorConfig const &initConfig);
			virtual ~Season();

#if 0
			bool getLines2();
#endif
			virtual bool readDataAvail();
			virtual void read(Data_t &result);
			virtual void write(Data_t const &data);
			virtual bool resetRequested();
			virtual long unsigned tick(); // TODO: Change long unsigned to void!!

		private:
			void init();
			bool isCARLineUp();

			CardEmulatorConfig const &config;
			int fileDescriptor;

			const int maxBufferLen;

			bool rstRequested;
			bool prevCARLineUp;
	};
}
#endif // SEASON_H
