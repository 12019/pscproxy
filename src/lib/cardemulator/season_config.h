#ifndef SEASON_CONFIG_H
#define SEASON_CONFIG_H

#include <string>

namespace PSCProxy {
	class SeasonConfig {
		public:
			SeasonConfig(std::string const &file);
			~SeasonConfig();
			const char *getDevFilename() const { return devFilename.c_str(); }

		private:
			std::string devFilename;
	};
}

#endif // SEASON_CONFIG_H
