#include "RPCClientStub.h"


namespace DSFramework {
	namespace DSRPC {
		RPCClientStub::RPCClientStub(std::string clientid) : m_clientid(clientid)
		{
		}

		RPCClientStub::~RPCClientStub()
		{
			LOG_DEBUG_CONSOLE(m_cachedRequestPacket->DebugString());
		}

		void RPCClientStub::OnData(const std::shared_ptr<Session> session, const std::shared_ptr<DSCRecvPacket> packet)
		{
			bool deserializeResult = false;
			std::shared_ptr<Packet::RPCPacket> response = this->Deserialize(packet->m_data, packet->m_dataSize, &deserializeResult);
			auto ec = response->error();
			auto type = response->type();
			if (deserializeResult && (type == Packet::TASK_RESPONSE || type == Packet::QUERY_RESPONSE))
			{
				switch (ec)
				{
				case Packet::PKT_DEFAULT:
					break;
				case Packet::PKT_NO_ERROR:
					HandleResponse(response);
					break;
				case Packet::PKT_SERIALIZATION_ERROR:
					LOG_WARN_CONSOLE("Server serialization error");
					break;
				case Packet::PKT_DESERIALIZATION_ERROR:
					LOG_WARN_CONSOLE("Server deserialization error");
					break;
				case Packet::PKT_EMPTY_REQUEST:
					LOG_WARN_CONSOLE("Empty request");
					break;
				case Packet::SERVICE_BUSY:
					LOG_WARN_CONSOLE("Service busy");
					break;
				case Packet::SERVICE_NOT_FOUND:
					LOG_WARN_CONSOLE("Service not found");
					break;
				case Packet::SERVICE_IVAILD_PARAMETERS:
					LOG_WARN_CONSOLE("Invalid parameters");
					break;
				case Packet::SERVICE_ERROR:
					LOG_WARN_CONSOLE("Service error");
					break;
				default:
					break;
				}
			}
			else
			{
				LOG_WARN_CONSOLE("Deserialization error");	
			}
		}

		void RPCClientStub::HandleResponse(std::shared_ptr<RPCPacket> response)
		{
			LOG_INFO_CONSOLE(response->DebugString());
			m_cachedRequestPacket = response;
		}

		std::shared_ptr<RPCPacket> RPCClientStub::Deserialize(const char* data, int datalength, bool* serializeResult)
		{
			std::shared_ptr<RPCPacket> packet = std::make_shared<RPCPacket>();
			*serializeResult = packet->ParseFromArray(data, datalength);
			return packet;
		}
	}
}