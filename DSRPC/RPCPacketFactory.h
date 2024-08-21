#pragma once
#include <memory>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "RPCPacket.pb.h"

using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {
		namespace Packet {
			class RPCPacketFactory
			{
			private:
				static inline std::string GetCurrentTime() {
					return boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
				}
			public:
				
				static void UpdateRPCPacketStatus(std::shared_ptr<RPCPacket> packet, Packet::RPCPacketStatus status) {

					if ((status & Packet::RPCPacketStatus::COMMITED) == Packet::RPCPacketStatus::COMMITED)
					{
						packet->set_status(Packet::RPCPacketStatus::COMMITED);
						packet->set_commited_time(GetCurrentTime());
					}
					else if ((status & Packet::RPCPacketStatus::COMPLETED) == Packet::RPCPacketStatus::COMPLETED)
					{
						packet->set_status(Packet::RPCPacketStatus::COMPLETED);
						packet->set_completed_time(GetCurrentTime());
					}
					else if ((status & Packet::RPCPacketStatus::WAITING) == Packet::RPCPacketStatus::WAITING)
					{
						packet->set_status(Packet::RPCPacketStatus::WAITING);
						packet->set_created_time(GetCurrentTime());
					}
				}

				static std::shared_ptr<RPCPacket> Deserialize(const char* data, size_t datalength, bool* serializeResult)
				{
					std::shared_ptr<RPCPacket> packet = std::make_shared<RPCPacket>();
					*serializeResult = packet->ParseFromArray(data, datalength);
					return packet;
				}

				static bool Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size)
				{
					if (!packet) {
						return false;
					}

					std::string serialized_data;
					if (!packet->SerializeToString(&serialized_data)) {
						return false;
					}

					char* buffer = new char[serialized_data.size()];
					memcpy(buffer, serialized_data.data(), serialized_data.size());

					*data = buffer;
					*size = serialized_data.size();
					return true;
				}
				
			};
		}
	}
}
