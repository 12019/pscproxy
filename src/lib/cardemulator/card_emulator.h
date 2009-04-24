#ifndef CARD_EMULATOR_H
#define CARD_EMULATOR_H

#include <vector>

namespace PSCProxy {
	typedef std::vector<unsigned char> Data_t;
	class CardEmulator {
		public:
			typedef enum {
				Error = -1,
				ReadDataAvail = 0x1,
				ResetRequested = 0x2,
			} State_t;

			CardEmulator();
			virtual ~CardEmulator();

			virtual bool readDataAvail() = 0;
			virtual void read(Data_t &result) = 0;
			virtual void write(Data_t const &data) = 0;
			virtual bool resetRequested() = 0;
			virtual long unsigned tick() = 0;

		protected:
			unsigned int resetDelay;
			unsigned int commandDelay;
			unsigned int dataTXDelay;
			unsigned int timeoutDelay;
	};
}

#endif // CARD_EMULATOR_H
