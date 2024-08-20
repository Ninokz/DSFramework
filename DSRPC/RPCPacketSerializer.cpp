#include "RPCPacketSerializer.h"


namespace DSFramework {
	namespace DSRPC {
		std::shared_ptr<RPCPacket> RPCPacketSerializer::Deserialize(const char* data, size_t size, bool* result)
		{
            auto packet = std::make_shared<RPCPacket>();

            if (packet->ParseFromArray(data, size)) {
                if (result) {
                    *result = true;
                }
                return packet;
            }
            else {
                if (result) {
                    *result = false;
                }
                return nullptr;
            }
		}

		bool RPCPacketSerializer::Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size)
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
	}
}