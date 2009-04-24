#ifndef PHOENIX_H
#define PHOENIX_H

#include "cardreader.h"

namespace PSCProxy {
	class Phoenix : public CardReader {
		public:
			Phoenix(CardReaderConfig const &config);
			~Phoenix();

			virtual Data_t const &reset();
			virtual void read(Data_t &result);

		protected:
			void init();
			void init2();
			void setctrl(const int ctrl);
			const int maxBufferLen;
	};
}
#endif // PHOENIX_H
