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
				static inline std::string CurrentTime() {
					return boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
				}
			public:
				static std::shared_ptr<RPCPacket> CreateErrorResponse(std::string from, std::string to, Packet::RPCPacketStatus status, Packet::RPCPacketError error, std::string innerID);

				static std::shared_ptr<RPCPacket> CreateResponseFromOrign(std::shared_ptr<RPCPacket> orign, Packet::RPCPacketStatus status, Packet::RPCPacketError error,
					std::string innerID);
				
				static void UpdateRPCPacketStatus(std::shared_ptr<RPCPacket> packet, Packet::RPCPacketStatus status);

				static std::shared_ptr<RPCPacket> Deserialize(const char* data, size_t datalength, bool* serializeResult);

				static bool Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size);
			};
		}
	}
}
