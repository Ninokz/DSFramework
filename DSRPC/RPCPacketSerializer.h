#pragma once
#include <string>
#include <memory>

#include "google/protobuf/message.h"

#include "RPCPacket.pb.h"

using DSFramework::DSRPC::Packet::RPCPacket;

namespace DSFramework {
	namespace DSRPC {
		class RPCPacketSerializer
		{
		public:
			static std::shared_ptr<RPCPacket> Deserialize(const char* data, size_t size, bool* result);
			
			static bool Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size);

			static bool CheckPacket(const std::shared_ptr<RPCPacket> packet);
		};
	}
}