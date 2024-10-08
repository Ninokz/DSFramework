#include "DSCPacket.h"

namespace DSFramework {
	namespace DSCommunication {
		DSCPacket::DSCPacket() : m_data(nullptr), m_dataSize(0) {}

		DSCPacket::DSCPacket(uint16_t dataSize)
		{
			m_dataSize = dataSize;
			m_data = new char[dataSize]();
		}

		DSCPacket::~DSCPacket()
		{
			delete[] m_data;
		}

		void DSCPacket::Clear()
		{
			::memset(m_data, 0, m_dataSize);
			m_dataSize = 0;
		}

		std::string DSCPacket::GetDataString() {
			std::string str;
			str.append("DataSize: ");
			str.append(std::to_string(m_dataSize));
			std::string data = std::string(m_data, m_dataSize);
			str.append(" Data: ");
			str.append(data);
			return str;
		}

		std::string DSCPacket::GetDataStringLength()
		{
			std::string str;
			str.append("DataSize: ");
			str.append(std::to_string(m_dataSize));
			return str;
		}

		DSCPacket::DSCPacket(const DSCPacket& other)
		{
			m_dataSize = other.m_dataSize;
			m_data = new char[m_dataSize];
			memcpy(m_data, other.m_data, m_dataSize);
		}

		DSCPacket& DSCPacket::operator=(const DSCPacket& other)
		{
			if (this != &other)
			{
				m_dataSize = other.m_dataSize;
				m_data = new char[m_dataSize];
				memcpy(m_data, other.m_data, m_dataSize);
			}
			return *this;
		}

		DSCRecvPacket::DSCRecvPacket(uint16_t recvSize) : DSCPacket(recvSize)
		{
		}

		DSCSendPacket::DSCSendPacket(char* data, uint16_t dataSize) :
			DSCPacket(dataSize + DSC_PACKET_HEAD_BYTES_SIZE)
		{
			/// convert the dataSize to network byte order
			unsigned short size = boost::asio::detail::socket_ops::host_to_network_short(dataSize);

			/// write the size num to the first DSC_PACKET_HEAD_BYTES_SIZE bytes
			memcpy(m_data, &size, DSC_PACKET_HEAD_BYTES_SIZE);
			/// copy the data to the rest of the array
			memcpy(m_data + DSC_PACKET_HEAD_BYTES_SIZE, data, dataSize);
		}

		DSCSendPacket::DSCSendPacket(const char* data, uint16_t dataSize) :
			DSCPacket(dataSize + DSC_PACKET_HEAD_BYTES_SIZE)
		{
			/// convert the dataSize to network byte order
			unsigned short size = boost::asio::detail::socket_ops::host_to_network_short(dataSize);

			/// write the size num to the first DSC_PACKET_HEAD_BYTES_SIZE bytes
			memcpy(m_data, &size, DSC_PACKET_HEAD_BYTES_SIZE);
			/// copy the data to the rest of the array
			memcpy(m_data + DSC_PACKET_HEAD_BYTES_SIZE, data, dataSize);
		}
	}
}