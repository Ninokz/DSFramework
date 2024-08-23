#include "RPCClientStub.h"


namespace DSFramework {
	namespace DSRPC {
		RPCClientStub::RPCClientStub(std::string clientid) : m_clientid(clientid)
		{
		}

		RPCClientStub::~RPCClientStub()
		{
		}

		void RPCClientStub::OnData(std::shared_ptr<Session> session, std::shared_ptr<DSCRecvPacket> packet)
		{
			LOG_DEBUG_CONSOLE("Stub Layer Data received: " + packet->PrintStringFormat());
			bool deserializeResult = false;
			std::shared_ptr<Packet::RPCPacket> rpcPacket = this->Deserialize(packet->m_data, packet->m_dataSize, &deserializeResult);
			if (deserializeResult)
			{
				LOG_DEBUG_CONSOLE("Packet deserialized:\n" + rpcPacket->DebugString());

			}
			else
			{

			}
		}

		std::shared_ptr<RPCPacket> RPCClientStub::Deserialize(const char* data, int datalength, bool* serializeResult)
		{
			std::shared_ptr<RPCPacket> packet = std::make_shared<RPCPacket>();
			*serializeResult = packet->ParseFromArray(data, datalength);
			return packet;
		}
	}
}