#ifndef CARDREADER_H
#define CARDREADER_H

#include "cardreader_config.h"
#include "common_defines.h"

namespace PSCProxy {
	class CardReader {
		public:
			CardReader(CardReaderConfig const &initConfig);
			virtual ~CardReader();
			virtual Data_t const &reset() = 0;
			virtual void read(Data_t &result) = 0;
			virtual void write(Data_t const &result) = 0;
			Data_t const &getAtr() const { return atr; }

		protected:
			CardReaderConfig config;
			Data_t atr;
			
			unsigned int resetDelay;
			unsigned int commandDelay;
			unsigned int dataTXDelay;
			unsigned int timeoutDelay;

			int fileDescriptor;
	};
}
#endif // CARDREADER_H
