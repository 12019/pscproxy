#ifndef CARDREADER_H
#define CARDREADER_H

#include <vector>

#include "cardreader_config.h"

namespace PSProxy {
	typedef std::vector<unsigned char> Data_t;
	class CardReader {
		public:
			CardReader(CardReaderConfig const &initConfig);
			~CardReader();
			virtual Data_t const &reset() = 0;
			Data_t const &getAtr() const { return atr; }

		protected:
			CardReaderConfig const &config;
			Data_t atr;
			
			unsigned int resetDelay;
			unsigned int commandDelay;
			unsigned int dataTXDelay;
			unsigned int timeoutDelay;

			int fileDescriptor;
			
	};
}
#endif // CARDREADER_H
