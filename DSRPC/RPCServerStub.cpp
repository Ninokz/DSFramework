#include "RPCServerStub.h"

namespace DSFramework {
	namespace DSRPC {
		void RPCServerStub::OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg)
		{
			bool deserializeResult = false;
			std::shared_ptr<Packet::RPCPacket> packet = RPCPacketSerializer::Deserialize(msg->m_data, msg->m_dataSize, &deserializeResult);
			if (!deserializeResult)
			{

			}
			else
			{
				LOG_WARN_CONSOLE("Stub Layer Data received: " + msg->PrintStringFormat());
			}
		}
	}
}