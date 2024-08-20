#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <boost/asio.hpp>

namespace DSFramework {
	namespace DSCommunication {
		const uint8_t DSC_PACKET_HEAD_BYTES_SIZE = 2;
		const uint16_t DSC_PACKET_BODY_MAX_SIZE = 4096;
		class DSCPacket
		{
		public:
			char* m_data;
			uint16_t m_dataSize;
		public:
			DSCPacket();
			DSCPacket(uint16_t dataSize);

			DSCPacket(const DSCPacket&);
			DSCPacket& operator=(const DSCPacket&);

			virtual ~DSCPacket();
			void Clear();
			std::string PrintStringFormat();
		};

		class DSCRecvPacket : public DSCPacket
		{
		public:
			DSCRecvPacket() = delete;
			DSCRecvPacket(uint16_t recvSize);
		};

		class DSCSendPacket : public DSCPacket
		{
		public:
			DSCSendPacket() = delete;
			DSCSendPacket(char* data, uint16_t dataSize);
			DSCSendPacket(const char* data, uint16_t dataSize);
		};
	}
}