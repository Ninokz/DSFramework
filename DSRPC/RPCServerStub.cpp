#include "RPCServerStub.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServerStub::RPCServerStub()
		{
			m_packetManager = std::make_shared<RPCPacketManager>();
		}

		RPCServerStub::~RPCServerStub()
		{
		}

		void RPCServerStub::OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg)
		{
			LOG_DEBUG_CONSOLE("Stub Layer Data received: " + msg->PrintStringFormat());
			bool deserializeResult = false;
			std::shared_ptr<Packet::RPCPacket> packet = RPCPacketFactory::Deserialize(msg->m_data, msg->m_dataSize, &deserializeResult);
			if (deserializeResult)
			{
				LOG_DEBUG_CONSOLE("Packet deserialized:" + packet->DebugString());
			}
			else
			{
				packet = RPCPacketFactory::CreateErrorResponse(m_serverid, sender->GetUUID(), Packet::RPCPacketStatus::WAITING, Packet::RPCPacketError::PKT_DESERIALIZATION_ERROR, sender->GetUUID());
				const char* data = nullptr;
				size_t size = 0;
				if (RPCPacketFactory::Serialize(packet, &data, &size))
				{
					sender->Send(data, size);
					delete[] data;
					data = nullptr;
				}
				else
				{
					LOG_ERROR_CONSOLE("Failed to serialize error response packet");
				}
			}
		}
	}
}