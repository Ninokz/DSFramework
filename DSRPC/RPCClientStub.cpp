#include "RPCClientStub.h"


namespace DSFramework {
	namespace DSRPC {
		RPCClientStub::RPCClientStub(std::string clientid) : m_clientid(clientid)
		{
			
		}

		RPCClientStub::~RPCClientStub()
		{

		}

		void RPCClientStub::OnData(const std::shared_ptr<Session> session, const std::shared_ptr<DSCRecvPacket> packet)
		{
			LOG_DEBUG_CONSOLE("Stub Layer Data received: " + packet->PrintStringFormat());
			bool deserializeResult = false;
			std::shared_ptr<Packet::RPCPacket> packet = this->Deserialize(packet->m_data, packet->m_dataSize, &deserializeResult);
			if (deserializeResult)
			{

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