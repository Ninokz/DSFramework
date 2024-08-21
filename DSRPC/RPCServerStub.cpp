#include "RPCServerStub.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServerStub::RPCServerStub()
		{
		}

		RPCServerStub::~RPCServerStub()
		{
		}

		void RPCServerStub::OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg)
		{
			LOG_DEBUG_CONSOLE("Stub Layer Data received: " + msg->PrintStringFormat());
			bool res = false;
			auto pkt = RPCPacketFactory::Deserialize(msg->m_data, msg->m_dataSize, &res);
			if (res)
				LOG_DEBUG_CONSOLE("Stub Layer Data deserialize: " + pkt->DebugString());
		}
	}
}