#ifndef PACKET_DATA_H
#define PACKET_DATA_H

#include <string>

namespace PSCProxy {
	class PacketData {
		public:
			PacketData();
			PacketData(std::string const &newData);
			PacketData(const char *initData, unsigned int initSize);
			PacketData(const PacketData &rhs);
			virtual ~PacketData();
			virtual void clear();

			virtual void setData(std::string const &newData);
			virtual void setData(char const *newData, int newSize);
			virtual std::string getData() const { return data; }
			virtual const char *getDataBuf() const { return data; }
			virtual unsigned int getSize() const { return size; }

			PacketData operator +(PacketData const &rhs);
			PacketData &operator =(const PacketData &rhs);

			static unsigned int maxLen() { return 1024; }

		private:
			unsigned int size;
			char *data;
	};
}
#endif // PACKET_DATA_H
