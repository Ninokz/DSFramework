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

			/// 将接收到的数据反序列化为RPCPacket
			std::shared_ptr<Packet::RPCPacket> packet = this->Deserialize(msg->m_data, msg->m_dataSize, &deserializeResult);
			if (deserializeResult)
			{
				LOG_DEBUG_CONSOLE("Packet deserialized:\n" + packet->DebugString());
				/// 设置RPCPacket的状态为等待 错误为无错误
				RPCPacketFactory::UpdateRPCPacketStatus(packet, Packet::RPCPacketStatus::WAITING);
				packet->set_error(Packet::RPCPacketError::PKT_NO_ERROR);
				/// 将请求添加到请求管理器中，并获取请求ID
				std::string requestid = m_packetManager->AddRequest(packet);

				/// todo 完成dispatcher的实现
				/// ex: m_dispatcher->Dispatch(packet);
				if (true)
				{
					/// 若成功分发请求则执行以下代码
					LOG_DEBUG_CONSOLE("Request dispatched success");
					auto response = RPCPacketFactory::CreateResponseFromOrign(packet, Packet::RPCPacketStatus::WAITING, Packet::RPCPacketError::PKT_NO_ERROR, sender->GetUUID());
					Send(sender, response);
				}
				else
				{
					/// 若分发请求失败则执行以下代码
					LOG_DEBUG_CONSOLE("Request dispatched failed");
					/// 从请求管理器中移除请求
					m_packetManager->RemoveRequest(requestid);
					auto response = RPCPacketFactory::CreateResponseFromOrign(packet, Packet::RPCPacketStatus::WAITING, Packet::RPCPacketError::SERVICE_BUSY, sender->GetUUID());
					Send(sender, response);
				}
				/// todo 完成dispatcher的实现
			}
			else
			{
				packet.reset();
				packet = RPCPacketFactory::CreateErrorResponse(m_serverid, sender->GetUUID(), Packet::RPCPacketStatus::WAITING, Packet::RPCPacketError::PKT_DESERIALIZATION_ERROR, sender->GetUUID());
				Send(sender, packet);
			}
		}

		void RPCServerStub::Send(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet)
		{
			const char* data = nullptr;
			size_t size = 0;
			if (this->Serialize(packet, &data, &size))
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

		inline std::shared_ptr<RPCPacket> RPCServerStub::Deserialize(const char* data, size_t datalength, bool* serializeResult)
		{
			std::shared_ptr<RPCPacket> packet = std::make_shared<RPCPacket>();
			*serializeResult = packet->ParseFromArray(data, datalength);
			return packet;
		}

		inline bool RPCServerStub::Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size)
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