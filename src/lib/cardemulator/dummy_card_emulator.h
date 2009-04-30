#ifndef DUMMY_CARD_EMULATOR_H
#define DUMMY_CARD_EMULATOR_H

#include <time.h>

#include "card_emulator.h"

namespace PSCProxy {
	class DummyCardEmulator : public CardEmulator {
		public:
			DummyCardEmulator();
			virtual ~DummyCardEmulator();

			virtual bool readDataAvail();
			virtual void read(Data_t &result);
			virtual void write(Data_t const &data);
			virtual bool resetRequested();
			virtual long unsigned tick();

		private:
			int rand(int min, int max);
			void prepareReadData(bool force = false);

			time_t lastDataAvailTime;
			bool resetAlreadyRequested;

			Data_t readData;
	};
}

#endif // DUMMY_CARD_EMULATOR_H
