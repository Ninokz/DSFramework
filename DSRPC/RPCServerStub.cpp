#include "RPCServerStub.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServerStub::RPCServerStub() :
			m_RequestDispatcher(std::make_shared<RequestDispatcher>()), 
			m_ResponseDispatcher(std::make_shared<ResponseDispatcher>()), 
			m_TaskManager(std::make_shared<RPCTaskManager>())
		{

		}

		RPCServerStub::~RPCServerStub()
		{

		}

		void RPCServerStub::OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg)
		{
			bool deserializeResult = false;
			std::shared_ptr<Packet::RPCPacket> packet = RPCPacketSerializer::Deserialize(msg->m_data, msg->m_dataSize, &deserializeResult);
			if (deserializeResult)
			{

			}
			else
			{
				LOG_DEBUG_CONSOLE("Deserialize failed: " + msg->PrintStringFormat());
			}
		}
	}
}