#include "RPCServerStub.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServerStub::RPCServerStub(std::shared_ptr<RPCEventHandler> rpcEventHandler) : m_rpcEventHandler(rpcEventHandler)
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
				/// ���ɹ������л���ִ�����´���
				LOG_DEBUG_CONSOLE("Packet deserialized:\n" + packet->DebugString());
				if (packet->service().empty()) {
					/// ��service�򷵻ش�����Ӧ
					packet.reset();
					packet = RPCPacketFactory::CreateErrorResponse(m_serverid, sender->GetUUID(), Packet::RPCPacketError::PKT_EMPTY_REQUEST, sender->GetUUID());
					Send(sender, packet);
					return;
				}

				/// 1. ��ʼ��������������ID
				std::string requestid = RPCPacketFactory::InitRPCPacket(packet,sender->GetUUID());
				/// 2. ������ID��RPCPacket����EventHandler����: Ŀǰֻ��RPCPacketManagerʵ����IDeserializedEventHandler, �Ὣ����ID��RPCPacket���浽m_requests��, ����OnDeserialized�¼���������һλ
				m_rpcEventHandler->OnDeserialized(requestid, sender->GetUUID(), packet);
				/// 3. dispatcher�ַ����� todo ���dispatcher��ʵ��
				/// todo ���dispatcher��ʵ��
				if (false)
				{
					/// ���ɹ��ַ�������ִ�����´���
					LOG_DEBUG_CONSOLE("Request dispatched success");
					/// 4. ֪ͨEventHandler�����Ѿ��ַ�: Ŀǰֻ��RPCPacketManagerʵ����IDispatchEventHandler, �Ὣ����ID��RPCPacket��״̬����ΪCOMMITED, ����OnDispatched�¼���������һλ
					m_rpcEventHandler->OnDispatched(requestid);
					/// 5. ʹ�� RPCPacketFactory ���ɶ�Ӧ��Ӧ��, ���
					std::shared_ptr<Packet::RPCPacket> response = RPCPacketFactory::CreateResponse(packet);
					/// 5. ������Ӧ����
					RPCPacketFactory::ChangeTypeToResponse(packet);
					/// 6. ���ô�����
					RPCPacketFactory::SetErrorCode(response, Packet::RPCPacketError::PKT_NO_ERROR);
					/// 7. ת���շ���(��Ϊ�ð�û�б�����manager)
					RPCPacketFactory::ChangeRecvSend(response);
					/// 8. ������Ӧ��
					this->Send(sender, response);
				}
				else
				{
					/// ���ַ�����ʧ����ִ�����´���
					LOG_DEBUG_CONSOLE("Request dispatched failed");
					/// 4. ֪ͨEventHandler����ַ�ʧ��: Ŀǰֻ��RPCPacketManagerʵ����IDispatchEventHandler, �Ὣ����ID��RPCPacket��״̬����ΪCOMPLETED,�����Ƴ������ ����OnDispatchFailed�¼���������һλ
					m_rpcEventHandler->OnDispatchFailed(requestid);
					/// 5. ������Ӧ����
					RPCPacketFactory::ChangeTypeToResponse(packet);
					/// 6. ���ô�����
					RPCPacketFactory::SetErrorCode(packet, Packet::RPCPacketError::SERVICE_BUSY);
					/// 7. ת���շ���(��Ϊ�ð�û�б�����manager)
					RPCPacketFactory::ChangeRecvSend(packet);
					/// 8. ������Ӧ��
					this->Send(sender, packet);
				}
			}
			else
			{
				packet.reset();
				packet = RPCPacketFactory::CreateErrorResponse(m_serverid, sender->GetUUID(), Packet::RPCPacketError::PKT_DESERIALIZATION_ERROR, sender->GetUUID());
				Send(sender, packet);
			}
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
					sender->Send(data, (int)size);
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