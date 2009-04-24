#ifndef PACKET_DATA_H
#define PACKET_DATA_H

#include <string>

namespace PSProxy {
	class PacketData {
		public:
			PacketData();
			PacketData(std::string const &newData);
			virtual ~PacketData();
			virtual void clear();

			virtual void setData(std::string const &newData);
			virtual void setData(char const *newData, int newSize);
			virtual std::string getData() const { return data; }
			virtual const char *getDataBuf() const { return data; }
			virtual unsigned int getSize() const { return size; }

			static unsigned int maxLen() { return 1024; }

		private:
			unsigned int size;
			char *data;
	};
}
#endif // PACKET_DATA_H
