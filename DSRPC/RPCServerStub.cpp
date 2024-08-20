#include "RPCServerStub.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServerStub::RPCServerStub() :
			m_RequestDispatcher(std::make_shared<RequestDispatcher>()), 
			m_ResponseDispatcher(std::make_shared<ResponseDispatcher>()), 
			m_taskManager(std::make_shared<RPCTaskManager>())
		{

		}

		RPCServerStub::~RPCServerStub()
		{

		}

		void RPCServerStub::OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg)
		{
			bool deserializeResult = false;
			std::shared_ptr<Packet::RPCPacket> packet = RPCPacketSerializer::Deserialize(msg->m_data, msg->m_dataSize, &deserializeResult);
			bool checkResult = RPCPacketSerializer::CheckPacket(packet);
			if (deserializeResult && checkResult)
			{
				std::string taskid = m_taskManager->AddTask(packet);
				bool postResult = m_RequestDispatcher->PostRequestToQueue(packet);
				if (postResult)
				{
					LOG_DEBUG_CONSOLE("Post success: " + msg->PrintStringFormat());
					HandlePostSuccess(sender, packet);
				}
				else
				{
					LOG_DEBUG_CONSOLE("Post failed: " + msg->PrintStringFormat());
					this->m_taskManager->RemoveTask(taskid);
					HandlePostFailed(sender);
				}
			}
			else
			{
				LOG_DEBUG_CONSOLE("Deserialize failed: " + msg->PrintStringFormat());
				HandleDeserializeFailed(sender);
			}
		}

		void RPCServerStub::HandleDeserializeFailed(std::shared_ptr<Session> sender)
		{
			/// Create a response packet
			std::shared_ptr<Packet::RPCPacket> responsePacket = std::make_shared<Packet::RPCPacket>();
			responsePacket->set_pkt_type(Packet::RPCPacketType::RPC_TASK_RESPONSE);
			responsePacket->set_pkt_error(Packet::RPCPacketError::PKT_DESERIALIZATION_ERROR);
			responsePacket->set_from(m_ServerUid);
			responsePacket->set_to(sender->GetUUID());
			responsePacket->clear_task();

			const char* data = nullptr;
			size_t size = 0;
			if (RPCPacketSerializer::Serialize(responsePacket, &data, &size))
			{
				// sender->Send(data, size);
				LOG_DEBUG_CONSOLE("Response send");
			}
			else
			{
				LOG_ERROR_CONSOLE_DETAIL("HandleDeserializeFailed Serialize Failed");
			}
		}

		void RPCServerStub::HandlePostFailed(std::shared_ptr<Session> sender)
		{
			/// Create a response packet
			std::shared_ptr<Packet::RPCPacket> responsePacket = std::make_shared<Packet::RPCPacket>();
			responsePacket->set_pkt_type(Packet::RPCPacketType::RPC_TASK_RESPONSE);
			responsePacket->set_pkt_error(Packet::RPCPacketError::PKT_POST_FAILED);
			responsePacket->set_from(m_ServerUid);
			responsePacket->set_to(sender->GetUUID());
			responsePacket->clear_task();

			const char* data = nullptr;
			size_t size = 0;
			if (RPCPacketSerializer::Serialize(responsePacket, &data, &size))
			{
				// sender->Send(data, size);
				LOG_DEBUG_CONSOLE("Response send");
			}
			else
			{
				LOG_ERROR_CONSOLE_DETAIL("HandlePostFailed Serialize Failed");
			}
		}

		void RPCServerStub::HandlePostSuccess(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet)
		{
			// deep copy packet
			std::shared_ptr<Packet::RPCPacket> responsePacket = std::make_shared<Packet::RPCPacket>();
			responsePacket->CopyFrom(*packet);
			responsePacket->set_pkt_type(Packet::RPCPacketType::RPC_TASK_RESPONSE);
			responsePacket->set_pkt_error(Packet::RPCPacketError::PKT_NO_ERROR);

			auto from = responsePacket->from();
			auto to = responsePacket->to();
			responsePacket->set_from(to);
			responsePacket->set_to(from);

			const char* data = nullptr;
			size_t size = 0;
			if (RPCPacketSerializer::Serialize(responsePacket, &data, &size))
			{
				// sender->Send(data, size);
				LOG_DEBUG_CONSOLE("Response send");
			}
			else
			{
				LOG_ERROR_CONSOLE_DETAIL("HandlePostSuccess Serialize Failed");
			}
		}
	}
}