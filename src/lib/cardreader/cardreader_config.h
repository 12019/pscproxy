#ifndef CARDREADER_CONFIG_H
#define CARDREADER_CONFIG_H

#include <string>
namespace PCProxy {
	class CardReaderConfig {
		public:
			CardReaderConfig(std::string const&file);
			~CardReaderConfig();

			const char *getDevFilename() const { return devFilename.c_str(); }

		private:
			std::string devFilename;

	};
}

#endif // CARDREADER_CONFIG_H
