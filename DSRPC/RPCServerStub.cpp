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
			RPCPacketFactory::UpdateRPCPacketStatus(packet, Packet::RPCPacketStatus::WAITING);
			if (deserializeResult)
			{
				LOG_DEBUG_CONSOLE("Packet deserialized:" + packet->DebugString());
				std::string requestid = m_packetManager->AddRequest(packet);

				/// todo ���dispatcher��ʵ��
				/// ex: m_dispatcher->Dispatch(packet);
				if (true)
				{
					/// ���ɹ��ַ�������ִ�����´���
					LOG_DEBUG_CONSOLE("Request dispatched success");
					auto response = RPCPacketFactory::CreateResponseFromOrign(packet, Packet::RPCPacketStatus::WAITING, Packet::RPCPacketError::PKT_NO_ERROR, sender->GetUUID());
					Send(sender, response);
				}
				else
				{
					/// ���ַ�����ʧ����ִ�����´���
					LOG_DEBUG_CONSOLE("Request dispatched failed");
					/// ��������������Ƴ�����
					m_packetManager->RemoveRequest(requestid);
					auto response = RPCPacketFactory::CreateResponseFromOrign(packet, Packet::RPCPacketStatus::WAITING, Packet::RPCPacketError::SERVICE_BUSY, sender->GetUUID());
					Send(sender, response);
				}
				/// todo ���dispatcher��ʵ��
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