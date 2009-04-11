#ifndef PHOENIX_X
#define PHOENIX_X

#include "cardreader.h"

namespace PSProxy {
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
#endif // PHOENIX_X
