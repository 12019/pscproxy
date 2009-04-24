#ifndef PHOENIX_H
#define PHOENIX_H

#include "cardreader.h"

namespace PSCProxy {
	class Phoenix : public CardReader {
		public:
			Phoenix(CardReaderConfig const &config);
			virtual ~Phoenix();

			virtual Data_t const &reset();
			virtual void read(Data_t &result);
			virtual void write(Data_t const &data);

		protected:
#if 0
			void init();
#endif
			void init2();
			void setctrl(const int ctrl);
			const int maxBufferLen;
	};
}
#endif // PHOENIX_H
