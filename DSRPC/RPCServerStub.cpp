#include "RPCServerStub.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServerStub::RPCServerStub(std::string& serverid, RPCEventHandler& m_rpcEventHandler) :
			m_serverid(serverid),
			m_rpcEventHandler(m_rpcEventHandler)
		{

		}

		RPCServerStub::~RPCServerStub()
		{
		}

		void RPCServerStub::OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg)
		{
			bool deserializeResult = false;
			/// 将接收到的数据反序列化为RPCPacket
			std::shared_ptr<Packet::RPCPacket> packet = this->Deserialize(msg->m_data, msg->m_dataSize, &deserializeResult);
			if (deserializeResult)
			{
				m_rpcEventHandler.OnDeserialized(sender, packet);				
			}
			else
			{
				m_rpcEventHandler.OnDeserializedFailed(m_serverid, sender);
			}
		}

		std::shared_ptr<RPCPacket> RPCServerStub::Deserialize(const char* data, int datalength, bool* serializeResult)
		{
			std::shared_ptr<RPCPacket> packet = std::make_shared<RPCPacket>();
			*serializeResult = packet->ParseFromArray(data, datalength);
			return packet;
		}
	}
}