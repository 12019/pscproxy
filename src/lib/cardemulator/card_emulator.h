#ifndef CARD_EMULATOR_H
#define CARD_EMULATOR_H

#include <vector>

namespace PSCProxy {
	typedef std::vector<unsigned char> Data_t;
	class CardEmulator {
		public:
			CardEmulator();
			virtual ~CardEmulator();

			virtual bool readDataAvail() = 0;
			virtual void read(Data_t &result) = 0;
			virtual void write(Data_t const &data) = 0;
			virtual bool resetRequested() = 0;
			virtual void tick() = 0;
	};
}

#endif // CARD_EMULATOR_H
