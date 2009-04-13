#ifndef SEASON_H
#define SEASON_H

#include <vector>

#include "season_config.h"
namespace PSProxy {
	typedef std::vector<unsigned char> Data_t;
	class Season {
		public:
			Season(SeasonConfig const &initConfig);
			~Season();

			bool readDataAvail();
			bool getLines2();
			virtual void read(Data_t &result);
			virtual void write(Data_t const &data);

		private:
			void init();

			SeasonConfig const &config;
			int fileDescriptor;

			const int maxBufferLen;

			unsigned int resetDelay;
			unsigned int commandDelay;
			unsigned int dataTXDelay;
			unsigned int timeoutDelay;
	};
}
#endif // SEASON_H
