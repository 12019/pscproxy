#ifndef DUMMY_CARD_READER_H
#define DUMMY_CARD_READER_H

#include "cardreader.h"

namespace PSCProxy {
	class DummyCardReader : public CardReader {
		public:
			DummyCardReader();
			virtual ~DummyCardReader();

			virtual void read(Data_t &result);
			virtual void write(Data_t const &result);
			virtual Data_t const &reset();

		private:
			int rand(int min, int max);
			void prepareReadData();

			Data_t readData;
	};
};

#endif // DUMMY_CARD_READER_H
