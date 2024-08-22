#include "RPCServerStub.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServerStub::RPCServerStub(RPCEventHandler& rpcEventHandler) :
			m_rpcEventHandler(rpcEventHandler)
		{

		}

		RPCServerStub::~RPCServerStub()
		{
		}

		void RPCServerStub::OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg)
		{
			LOG_DEBUG_CONSOLE("Stub Layer Data received: " + msg->PrintStringFormat());
			bool deserializeResult = false;
			/// �����յ������ݷ����л�ΪRPCPacket
			std::shared_ptr<Packet::RPCPacket> packet = this->Deserialize(msg->m_data, msg->m_dataSize, &deserializeResult);
			if (deserializeResult)
			{
				HandleDeserializedSuccess(sender, packet);
			}
			else
			{
				HandleDeserializedFailed(sender, packet);
			}
		}

		void RPCServerStub::HandleDeserializedSuccess(std::shared_ptr<Session> sender, std::shared_ptr<RPCPacket> packet)
		{
			/// ���ɹ������л���ִ�����´���
			if (packet->service().empty()) {
				/// ��service�򷵻ش�����Ӧ
				packet.reset();
				packet = RPCPacketFactory::CreateErrorResponse(m_serverid, sender->GetUUID(), Packet::RPCPacketError::PKT_EMPTY_REQUEST, sender->GetUUID());
				Send(sender, packet);
				return;
			}
			/// 1. ��ʼ��������������ID
			std::string requestid = RPCPacketFactory::InitRPCPacket(packet, sender->GetUUID());
			/// 2. ������ID��RPCPacket����EventHandler����: Ŀǰֻ��RPCPacketManagerʵ����IDeserializedEventHandler, �Ὣ����ID��RPCPacket���浽m_requests��, ����OnDeserialized�¼���������һλ
			m_rpcEventHandler.OnDeserialized(requestid, sender->GetUUID(), packet);
			/// 3. dispatcher�ַ����� todo ���dispatcher��ʵ��
			LOG_DEBUG_CONSOLE("Packet deserialized:\n" + packet->DebugString());
		}

		void RPCServerStub::HandleDeserializedFailed(std::shared_ptr<Session> sender, std::shared_ptr<RPCPacket> packet)
		{
			packet.reset();
			packet = RPCPacketFactory::CreateErrorResponse(m_serverid, sender->GetUUID(), Packet::RPCPacketError::PKT_DESERIALIZATION_ERROR, sender->GetUUID());
			Send(sender, packet);
		}

		void RPCServerStub::Send(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet)
		{
			const char* data = nullptr;
			size_t size = 0;
			if (this->Serialize(packet, &data, &size))
			{
				if (size > INT32_MAX) {
					LOG_ERROR_CONSOLE("Data size is too large");
					delete[] data;
					data = nullptr;
					return;
				}
				else
				{
					sender->Send(data, static_cast<int>(size));
					delete[] data;
					data = nullptr;
				}	
			}
			else
			{
				LOG_ERROR_CONSOLE("Failed to serialize error response packet");
			}
		}

		inline std::shared_ptr<RPCPacket> RPCServerStub::Deserialize(const char* data, int datalength, bool* serializeResult)
		{
			std::shared_ptr<RPCPacket> packet = std::make_shared<RPCPacket>();
			*serializeResult = packet->ParseFromArray(data, datalength);
			return packet;
		}

		inline bool RPCServerStub::Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size)
		{
			if (!packet || !data || !size) {
				return false;
			}

			*size = packet->ByteSizeLong();

			*data = new char[*size];
			if (!*data) {
				return false;
			}

			if (*size > INT32_MAX)
			{
				delete[] * data;
				*data = nullptr;
				*size = 0;
				return false;
			}

			if (!packet->SerializeToArray(const_cast<char*>(*data), (int)*size)) {
				delete[] * data;
				*data = nullptr;
				*size = 0;
				return false;
			}
			return true;
		}
	}
}